#include "apiservice.h"
#include "tcpclient.h"
#include "../utils/appstate.h"

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
    if (m_tcpClient)
    {
        disconnect(m_tcpClient, &TcpClient::dataReceived, this, &ApiService::onGetResponse);
    }

    m_tcpClient = tcpClient;
    if (m_tcpClient)
    {
        connect(m_tcpClient, &TcpClient::dataReceived, this, &ApiService::onGetResponse);
        qDebug() << "Api Service Set TcpClient " << m_tcpClient;
    }
}

// ==================== Authentication ====================

void ApiService::loginAsync(const QString &username)
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit loginError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 login %2").arg(requestId).arg(username);
    m_pendingRequests[requestId] = [this](QString response)
    { loginResponse(response); };
    sendCommand(command);

    qDebug() << "Login command: " << command;
}

void ApiService::logoutAsync(const QString &sessionId)
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 logout %2").arg(requestId).arg(sessionId);
    m_pendingRequests[requestId] = [this](QString response)
    { qDebug() << "Logout response: " << response; };

    sendCommand(command);

    qDebug() << "Logout command: " << command;
}

void ApiService::getUserInfoAsync(const QString &sessionId)
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit userInfoError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 getUserInfo %2").arg(requestId).arg(sessionId);
    m_pendingRequests[requestId] = [this](QString response)
    { getUserInfoResponse(response); };

    sendCommand(command);

    qDebug() << "GetUserInfo command: " << command;
}

void ApiService::getActiveGamesAsync()
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit userInfoError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 getActiveGames").arg(requestId);
    m_pendingRequests[requestId] = [this](QString response)
    { getActiveGamesResponse(response); };

    sendCommand(command);

    qDebug() << "getActiveGames command: " << command;
}

// ==================== Game Subscription ====================

void ApiService::subscribeOnGameAsync(qulonglong gameId)
{
    if (!m_tcpClient)
    {
        emit subscribeError("TCP client not set");
        return;
    }

    if (m_sessionId.isEmpty())
    {
        emit subscribeError("Session ID not set. Please login first.");
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 addPlayerToGame %2 %3").arg(requestId).arg(m_sessionId).arg(gameId);
    m_pendingRequests[requestId] = [this, requestId](QString response)
    { subscribeResponse(response, requestId); };

    sendCommand(command);

    qDebug() << "Subscribe command:" << command;
}

// ==================== Game Actions ====================

void ApiService::handleWordAsync(qulonglong gameId, const QString &sessionId, const QString &word)
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit wordHandleError(m_lastError);
        return;
    }

    if (sessionId.isEmpty())
    {
        m_lastError = "Session ID not set";
        emit wordHandleError(m_lastError);
        return;
    }

    if (word.isEmpty())
    {
        m_lastError = "Word cannot be empty";
        emit wordHandleError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 handleWord %2 %3 %4").arg(requestId).arg(gameId).arg(sessionId).arg(word);
    m_pendingRequests[requestId] = [this](QString response)
    { handleWordResponse(response); };

    sendCommand(command);

    qDebug() << "Handle word command:" << command;
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
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
    }

    if (!m_tcpClient->sendData(command + "\n"))
    {
        m_lastError = "Failed to send command";
    }
}

bool ApiService::isBooleanSuccess(const QString &response)
{
    return response.contains("successfully", Qt::CaseInsensitive) ||
           response == "OK" ||
           response == "Player added successfully";
}

void ApiService::loginResponse(const QString &response)
{
    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit loginError(response);
        return;
    }

    QString sessionId = response.trimmed();
    m_sessionId = sessionId;
    AppState &appState = AppState::getInstance();
    appState.setSessionId(sessionId);

    emit loginSuccess(sessionId);
}

void ApiService::getUserInfoResponse(const QString &response)
{
    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit userInfoError(response);
        return;
    }

    std::optional<UserInfo> user = ServerProtocolParser::parseUserInfo(response);
    if (!user.has_value())
    {
        emit userInfoError(response);
        return;
    }

    emit userInfoReceived(*user);
}

void ApiService::getActiveGamesResponse(const QString &response)
{
    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit activeGamesError(response);
        return;
    }

    QList<GameContext> games = ServerProtocolParser::parseMultipleGameContexts(response);

    emit activeGamesReceived(games);
}

void ApiService::subscribeResponse(const QString &response, int requestId)
{
    qDebug() << "Subscribe response:" << response;
    if (response.endsWith("successfully"))
    {
        m_pendingRequests[requestId] = [this, requestId](QString wordResponse)
        { newWordResponse(wordResponse, requestId); };
        emit subscribeSuccess();
    }
    else if (ServerProtocolParser::isError(response))
    {
        emit subscribeError(response);
    }
    else
    {
        emit subscribeError(response);
    }
}

void ApiService::newWordResponse(const QString &response, int requestId)
{
    qDebug() << "Received new word data:" << response;

    std::optional<NewWordUpdate> newWord = ServerProtocolParser::parseNewWordUpdate(response);
    if (newWord.has_value())
    {
        qDebug() << "New Word" << newWord->kanji;
        emit newWordReceived(*newWord);
    }
    else if (ServerProtocolParser::isError(response))
    {
        qWarning() << "Error from server:" << response;
        emit gameUpdateReceived(response);
    }
    else
    {
        qDebug() << "Unrecognized message:" << response;
    }

    m_pendingRequests[requestId] = [this, requestId](QString wordResponse)
    { newWordResponse(wordResponse, requestId); };
}

void ApiService::handleWordResponse(const QString &response)
{
    qDebug() << "Handle word response:" << response;

    HandleWordStatus status = ServerProtocolParser::parseHandleWordStatus(response);

    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit wordHandleError(response);
    }
    else
    {
        emit wordHandled(status);
    }
}

// ==================== Slots ====================

void ApiService::onGetResponse(QString response)
{
    if (response.isEmpty())
    {
        qDebug() << "Got empty response";
        return;
    }

    // Split responses by newline in case multiple responses arrived together
    QStringList responses = response.split('\n', Qt::SkipEmptyParts);

    for (const QString &singleResponse : responses)
    {
        QString trimmedResponse = singleResponse.trimmed();
        if (trimmedResponse.isEmpty())
            continue;

        bool ok = false;
        QString idString = trimmedResponse.split(' ')[0];
        int id = idString.toInt(&ok, 10);

        if (!ok)
        {
            qDebug() << "Got incorrect response id:" << trimmedResponse;
            continue;
        }

        auto process_it = m_pendingRequests.find(id);
        if (process_it != m_pendingRequests.end())
        {
            auto process = (*process_it);
            m_pendingRequests.erase(process_it);

            process(trimmedResponse.slice(idString.size()).trimmed());
        }
        else
        {
            qDebug() << "Ignoring response" << trimmedResponse;
        }
    }
}
