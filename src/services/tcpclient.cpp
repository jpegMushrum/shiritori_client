#include "tcpclient.h"
#include <QTcpSocket>
#include <QDebug>

TcpClient::TcpClient(QObject *parent)
    : QObject(parent), m_socket(std::make_unique<QTcpSocket>(this))
{
    connect(m_socket.get(), &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &TcpClient::onError);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

TcpClient::~TcpClient()
{
    if (isConnected())
    {
        disconnect();
    }
}

bool TcpClient::connectToHost(const QString &host, int port)
{
    if (isConnected())
    {
        disconnect();
    }

    m_host = host;
    m_port = port;
    m_socket->connectToHost(host, port);
    return m_socket->waitForConnected(3000);
}

void TcpClient::disconnect()
{
    if (isConnected())
    {
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected(3000);
    }
}

bool TcpClient::isConnected() const
{
    return m_socket && m_socket->state() == QTcpSocket::ConnectedState;
}

bool TcpClient::sendData(const QString &data)
{
    if (!isConnected())
    {
        return false;
    }

    QByteArray byteArray = data.toUtf8();
    qint64 bytesWritten = m_socket->write(byteArray);
    m_socket->flush();
    return bytesWritten == byteArray.size();
}

QString TcpClient::receiveData()
{
    if (!isConnected())
    {
        return QString();
    }

    return QString::fromUtf8(m_socket->readAll());
}

QString TcpClient::getHost() const
{
    return m_host;
}

int TcpClient::getPort() const
{
    return m_port;
}

void TcpClient::onConnected()
{
    qDebug() << "TCP Connected";
    emit connected();
}

void TcpClient::onDisconnected()
{
    qDebug() << "TCP Disconnected";
    emit disconnected();
}

void TcpClient::onError()
{
    QString errorMessage = m_socket->errorString();
    qDebug() << "TCP Error:" << errorMessage;
    emit connectionError(errorMessage);
}

void TcpClient::onReadyRead()
{
    QString data = receiveData();
    if (!data.isEmpty())
    {
        emit dataReceived(data);
    }
}
