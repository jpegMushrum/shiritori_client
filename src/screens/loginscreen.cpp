#include "loginscreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include "../services/tcpclient.h"
#include "../services/apiservice.h"
#include "../utils/appstate.h"

LoginScreen::LoginScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

LoginScreen::~LoginScreen() = default;

void LoginScreen::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    auto *titleLabel = new QLabel("Shiritori Game", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(titleLabel);

    layout->addSpacing(20);

    // Server Address
    auto *serverAddressLabel = new QLabel("Server Address:", this);
    layout->addWidget(serverAddressLabel);

    m_serverAddressInput = new QLineEdit(this);
    m_serverAddressInput->setPlaceholderText("localhost");
    m_serverAddressInput->setText("localhost");
    layout->addWidget(m_serverAddressInput);

    // Server Port
    auto *serverPortLabel = new QLabel("Server Port:", this);
    layout->addWidget(serverPortLabel);

    m_serverPortInput = new QLineEdit(this);
    m_serverPortInput->setPlaceholderText("3000");
    m_serverPortInput->setText("3000");
    layout->addWidget(m_serverPortInput);

    layout->addSpacing(10);

    // Username
    auto *usernameLabel = new QLabel("Username:", this);
    layout->addWidget(usernameLabel);

    m_usernameInput = new QLineEdit(this);
    m_usernameInput->setPlaceholderText("Enter your username");
    layout->addWidget(m_usernameInput);

    layout->addSpacing(10);

    // Error label
    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setWordWrap(true);
    layout->addWidget(m_errorLabel);

    layout->addSpacing(10);

    // Login button
    auto *loginButton = new QPushButton("Log In", this);
    loginButton->setMinimumHeight(40);
    connect(loginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
    layout->addWidget(loginButton);

    // Api Service
    AppState &appState = AppState::getInstance();
    m_apiService = appState.getApiService();
    connect(m_apiService, &ApiService::loginSuccess, this, &LoginScreen::onApiLoginSuccess);
    connect(m_apiService, &ApiService::loginError, this, &LoginScreen::onApiLoginError);

    m_tcpClient = appState.getTcpClient();
    connect(m_tcpClient, &TcpClient::connected, this, &LoginScreen::onTcpConnectSuccess);
    connect(m_tcpClient, &TcpClient::connectionError, this, &LoginScreen::onTcpConnectFailed);

    layout->addStretch();
}

bool LoginScreen::validateInput()
{
    QString username = m_usernameInput->text().trimmed();
    QString serverAddress = m_serverAddressInput->text().trimmed();
    QString serverPortStr = m_serverPortInput->text().trimmed();

    if (username.isEmpty())
    {
        m_errorLabel->setText("Username cannot be empty");
        return false;
    }

    if (serverAddress.isEmpty())
    {
        m_errorLabel->setText("Server address cannot be empty");
        return false;
    }

    bool ok;
    int serverPort = serverPortStr.toInt(&ok);
    if (!ok || serverPort <= 0 || serverPort > 65535)
    {
        m_errorLabel->setText("Invalid server port");
        return false;
    }

    m_errorLabel->clear();
    return true;
}

void LoginScreen::onLoginButtonClicked()
{
    if (!validateInput())
    {
        return;
    }

    QString username = m_usernameInput->text().trimmed();
    QString serverAddress = m_serverAddressInput->text().trimmed();
    int serverPort = m_serverPortInput->text().trimmed().toInt();

    m_errorLabel->setText("Connecting...");

    AppState &appState = AppState::getInstance();
    appState.setServerAddress(serverAddress);
    appState.setServerPort(serverPort);
    appState.setUsername(username);

    // Create TCP client and connect to server
    m_loginRequested = true;
    m_tcpClient->connectToHost(serverAddress, serverPort);
}

void LoginScreen::onTcpConnectFailed(const QString &errorMessage) {
    onApiLoginError(QString("Failed to connect to server.\nTcp error: %1").arg(errorMessage));
}

void LoginScreen::onTcpConnectSuccess()
{
    if (m_loginRequested) {
        AppState &appState = AppState::getInstance();
        QString username = appState.getUsername();

        // after connection, send login request
        m_errorLabel->setText("Logging in...");
        m_apiService->loginAsync(username);
        m_loginRequested = false;
    }
}

void LoginScreen::onApiLoginSuccess(const QString &sessionId)
{
    AppState &appState = AppState::getInstance();
    appState.setSessionId(sessionId);
    m_errorLabel->clear();

    qDebug() << "Login successful! Session ID:" << sessionId;
    navigate(ScreenNavigator::MainScreen);
}

void LoginScreen::onApiLoginError(const QString &errorMessage)
{
    m_errorLabel->setText("Login failed: " + errorMessage);
}

void LoginScreen::onOpen(ScreenNavigator::ScreenType screen)
{
    // TODO: Submit word to server
}