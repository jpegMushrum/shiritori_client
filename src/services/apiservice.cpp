#include "apiservice.h"
#include "tcpclient.h"

#include <QTcpSocket>
#include <QDebug>

ApiService::ApiService(QObject *parent)
    : QObject(parent)
{
}

ApiService::~ApiService()
{
    if (m_gameSocket) {
        m_gameSocket->disconnectFromHost();
    }
}

void ApiService::setTcpClient(TcpClient *tcpClient)
{
    m_tcpClient = tcpClient;
    connect(tcpClient, &TcpClient::dataReceived, this, &ApiService::onGetResponse);
}

// ==================== Authentication ====================

void ApiService::loginAsync(const QString &username)
{
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
    }

    QString command = QString("login %1").arg(username);
    sendCommand(command);

    qDebug() << "Login command: " << command;
}

void ApiService::logoutAsync(const QString &sessionId)
{
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
    }

    QString command = QString("logout %1").arg(sessionId);
    sendCommand(command);

    qDebug() << "Logout command: " << command;
}

// ==================== Error Handling ====================

QString ApiService::getLastError() const
{
    return m_lastError;
}

void ApiService::clearLastError()
{
    m_lastError.clear();
}

// ==================== Private Methods ====================

void ApiService::sendCommand(const QString &command)
{
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
    }

    if (!m_tcpClient->sendData(command + "\n")) {
        m_lastError = "Failed to send command";
    }
}

bool ApiService::isBooleanSuccess(const QString &response)
{
    return response.contains("successfully", Qt::CaseInsensitive) ||
           response == "OK" ||
           response == "Player added successfully";
}

void ApiService::loginResponse(QString response) {
    if (ServerProtocolParser::isError(response)) {
        m_lastError = response;
        emit loginError(response);
        return;
    }

    QString sessionId = response.trimmed();
    emit loginSuccess(sessionId);
}
// ==================== Slots ====================

void ApiService::onGetResponse(QString response) {
    if (response.isEmpty()) {
        qDebug() << "Got empty response";
        return;
    }

    bool ok = false;
    QString idString = response.split(' ')[0];
    int id = idString.toInt(&ok, 10);

    if (!ok) {
        qDebug() << "Got incorrect response id";
        return;
    }

    std::function<void(QString)> action = m_pendingRequests[id];
    action(response.slice(idString.size()).trimmed());
}
