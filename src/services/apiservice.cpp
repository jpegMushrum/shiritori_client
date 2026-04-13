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
}

void ApiService::setTcpClient(TcpClient *tcpClient)
{
    if (m_tcpClient) {
        disconnect(m_tcpClient, &TcpClient::dataReceived, this, &ApiService::onGetResponse);
    }

    m_tcpClient = tcpClient;
    if (m_tcpClient) {
        connect(m_tcpClient, &TcpClient::dataReceived, this, &ApiService::onGetResponse);
        qDebug() << "Api Service Set TcpClient " << m_tcpClient;
    }
}

// ==================== Authentication ====================

void ApiService::loginAsync(const QString &username)
{
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
        emit loginError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 login %2").arg(requestId).arg(username);
    m_pendingRequests[requestId] = [this](QString response){ loginResponse(response); };
    sendCommand(command);

    qDebug() << "Login command: " << command;
}

void ApiService::logoutAsync(const QString &sessionId)
{
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 logout %2").arg(requestId).arg(sessionId);
    m_pendingRequests[requestId] = [this](QString response){ qDebug() << "Logout response: " << response; };

    sendCommand(command);

    qDebug() << "Logout command: " << command;
}


void ApiService::getUserInfoAsync(const QString &sessionId) {
    if (!m_tcpClient) {
        m_lastError = "TCP client not set";
        emit userInfoError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 getUserInfo %2").arg(requestId).arg(sessionId);
    m_pendingRequests[requestId] = [this](QString response){ getUserInfoResponse(response); };

    sendCommand(command);

    qDebug() << "GetUserInfo command: " << command;
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

void ApiService::loginResponse(const QString& response) {
    if (ServerProtocolParser::isError(response)) {
        m_lastError = response;
        emit loginError(response);
        return;
    }

    QString sessionId = response.trimmed();
    emit loginSuccess(sessionId);
}

void ApiService::getUserInfoResponse(const QString& response) {
    if (ServerProtocolParser::isError(response)) {
        m_lastError = response;
        emit userInfoError(response);
        return;
    }

    std::optional<UserInfo> user = ServerProtocolParser::parseUserInfo(response);
    if (!user.has_value()) {
        emit userInfoError(response);
        return;
    }

    emit userInfoReceived(*user);
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

    auto process_it = m_pendingRequests.find(id);
    if (process_it != m_pendingRequests.end()) {
        (*process_it)(response.slice(idString.size()).trimmed());
        m_pendingRequests.erase(process_it);
    } else {
        qDebug() << "Ignoring response" << response;
    }
}
