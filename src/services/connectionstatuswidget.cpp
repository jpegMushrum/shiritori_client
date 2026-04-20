#include "connectionstatuswidget.h"
#include "tcpclient.h"
#include "../utils/appstate.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPainter>
#include <QStyle>

ConnectionStatusWidget::ConnectionStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    AppState &appState = AppState::getInstance();
    m_tcpClient = appState.getApiTcpClient();
    if (m_tcpClient)
    {
        connect(m_tcpClient, &TcpClient::connected, this, &ConnectionStatusWidget::onConnected);
        connect(m_tcpClient, &TcpClient::disconnected, this, &ConnectionStatusWidget::onDisconnected);
        connect(m_tcpClient, &TcpClient::connectionError, this, &ConnectionStatusWidget::onConnectionError);
        qDebug() << "Status Widget TcpClient connected " << m_tcpClient;
    }

    setupUI();
}

ConnectionStatusWidget::~ConnectionStatusWidget() = default;

void ConnectionStatusWidget::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(5);

    m_statusLabel = new QLabel("Server: Disconnected", this);
    m_statusLabel->setObjectName("connectionStatusLabel");
    m_statusLabel->setAlignment(Qt::AlignHCenter);
    layout->addWidget(m_statusLabel);

    m_reconnectButton = new QPushButton(this);
    QIcon icon = recolorIcon(":/icons/reconnect.png", Qt::white);
    m_reconnectButton->setIcon(icon);
    m_reconnectButton->setIconSize(QSize(24, 24));
    m_reconnectButton->setFixedSize(36, 32);
    m_reconnectButton->setToolTip("Reconnect to server");
    connect(m_reconnectButton, &QPushButton::clicked, this, &ConnectionStatusWidget::onReconnectClicked);
    layout->addWidget(m_reconnectButton, 0, Qt::AlignCenter);

    AppState &appState = AppState::getInstance();
    m_apiService = appState.getApiService();

    setFixedSize(140, 70);
}

QIcon ConnectionStatusWidget::recolorIcon(const QString &path, const QColor &color)
{
    QPixmap pixmap(path);

    QPixmap result(pixmap.size());
    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, pixmap);

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(result.rect(), color);

    painter.end();

    return QIcon(result);
}

void ConnectionStatusWidget::setTcpClient(TcpClient *tcpClient)
{
    if (m_tcpClient)
    {
        disconnect(m_tcpClient, &TcpClient::connected, this, &ConnectionStatusWidget::onConnected);
        disconnect(m_tcpClient, &TcpClient::disconnected, this, &ConnectionStatusWidget::onDisconnected);
        disconnect(m_tcpClient, &TcpClient::connectionError, this, &ConnectionStatusWidget::onConnectionError);
    }

    m_tcpClient = tcpClient;
    if (m_tcpClient)
    {
        connect(m_tcpClient, &TcpClient::connected, this, &ConnectionStatusWidget::onConnected);
        connect(m_tcpClient, &TcpClient::disconnected, this, &ConnectionStatusWidget::onDisconnected);
        connect(m_tcpClient, &TcpClient::connectionError, this, &ConnectionStatusWidget::onConnectionError);
        qDebug() << "Status Widget TcpClient connected " << m_tcpClient;
    }
    updateStatus();
}

void ConnectionStatusWidget::onConnected()
{
    updateStatus();

    if (m_loginRequested)
    {
        AppState &appState = AppState::getInstance();
        if (appState.isLoggedIn())
        {
            qDebug() << "Reconnection logout";
            m_apiService->logoutAsync(appState.getSessionId());
            appState.logout();
        }

        qDebug() << "Reconnection login" << appState.getUsername();
        m_apiService->loginAsync(appState.getUsername());
        m_loginRequested = false;
    }
}

void ConnectionStatusWidget::onDisconnected()
{
    updateStatus();
}

void ConnectionStatusWidget::onReconnectClicked()
{
    if (!m_tcpClient)
    {
        return;
    }

    AppState &appState = AppState::getInstance();

    m_loginRequested = true;
    m_tcpClient->connectToHost(appState.getServerAddress(), appState.getServerPort());
}

void ConnectionStatusWidget::onConnectionError(const QString &errorMessage)
{
    updateStatus();
}

void ConnectionStatusWidget::updateStatus()
{
    if (!m_tcpClient)
    {
        m_statusLabel->setText("Server: Error");
        m_statusLabel->style()->unpolish(m_statusLabel);
        m_statusLabel->style()->polish(m_statusLabel);
        return;
    }

    if (m_tcpClient->isConnected())
    {
        m_statusLabel->setText("Server: Connected");
        m_statusLabel->style()->unpolish(m_statusLabel);
        m_statusLabel->style()->polish(m_statusLabel);
    }
    else
    {
        m_statusLabel->setText("Server: Disconnected");
        m_statusLabel->style()->unpolish(m_statusLabel);
        m_statusLabel->style()->polish(m_statusLabel);
    }
}
