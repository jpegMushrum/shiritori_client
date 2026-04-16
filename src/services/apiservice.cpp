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

// ==================== Game Management ====================

void ApiService::startNewGameAsync(const QString &sessionId)
{
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit gameStartError(m_lastError);
        return;
    }

    if (sessionId.isEmpty())
    {
        m_lastError = "Session ID not set";
        emit gameStartError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 startNewGame %2").arg(requestId).arg(sessionId);
    m_pendingRequests[requestId] = [this](QString response)
    { startNewGameResponse(response); };

    sendCommand(command);

    qDebug() << "Start new game command: " << command;
}

void ApiService::getGameInfoAsync(qulonglong gameId)
{
    // DEPRECATED: Game info is now obtained atomically through addPlayerToGame
    // Use subscribeOnGameAsync instead which calls addPlayerToGame
    if (!m_tcpClient)
    {
        m_lastError = "TCP client not set";
        emit gameInfoError(m_lastError);
        return;
    }

    int requestId = m_nextRequestId++;
    QString command = QString("%1 getGameInfo %2").arg(requestId).arg(gameId);
    m_pendingRequests[requestId] = [this](QString response)
    { getGameInfoResponse(response); };

    sendCommand(command);

    qDebug() << "Get game info command: " << command;
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
    qDebug() << "Api set session id" << sessionId;

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

void ApiService::startNewGameResponse(const QString &response)
{
    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit gameStartError(response);
        return;
    }

    std::optional<GameContext> game = ServerProtocolParser::parseGameContext(response);
    if (!game.has_value())
    {
        m_lastError = "Failed to parse game context";
        emit gameStartError(m_lastError);
        return;
    }

    emit gameStarted(*game);
}

void ApiService::getGameInfoResponse(const QString &response)
{
    if (ServerProtocolParser::isError(response))
    {
        m_lastError = response;
        emit gameInfoError(response);
        return;
    }

    std::optional<GameContext> game = ServerProtocolParser::parseGameContext(response);
    if (!game.has_value())
    {
        m_lastError = "Failed to parse game context";
        emit gameInfoError(m_lastError);
        return;
    }

    emit gameInfoReceived(*game);
}

void ApiService::subscribeResponse(const QString &response, int requestId)
{
    qDebug() << "Subscribe response:" << response;

    // Check for "Player added successfully"
    if (response.endsWith("successfully"))
    {
        qDebug() << "Player added successfully, waiting for game info...";
        emit subscribeSuccess();
        // Stay subscribed for playerJoinedGame and newWord updates
        m_pendingRequests[requestId] = [this, requestId](QString nextResponse)
        { subscribeResponse(nextResponse, requestId); };
        return;
    }

    // Check for playerJoinedGame info
    if (response.startsWith("playerJoinedGame"))
    {
        qDebug() << "Received playerJoinedGame info";
        std::optional<PlayerJoinedGameInfo> info = ServerProtocolParser::parsePlayerJoinedGameInfo(response);
        if (info.has_value())
        {
            qDebug() << "Game info parsed - Last Kana:" << info->lastKana << "Used words:" << info->usedWords.size();
            emit playerJoinedGame(*info);
        }
        else
        {
            qWarning() << "Failed to parse playerJoinedGame";
        }

        // Stay subscribed for newWord updates
        m_pendingRequests[requestId] = [this, requestId](QString wordResponse)
        { subscribeResponse(wordResponse, requestId); };
        return;
    }

    // Check for newWord update
    if (response.startsWith("newWord"))
    {
        qDebug() << "Received new word update";
        std::optional<NewWordUpdate> newWord = ServerProtocolParser::parseNewWordUpdate(response);
        if (newWord.has_value())
        {
            qDebug() << "New Word" << newWord->kanji;
            emit newWordReceived(*newWord);
        }
        else
        {
            qWarning() << "Failed to parse newWord update";
            emit gameUpdateReceived(response);
        }

        // Re-add handler to continue listening for more words
        m_pendingRequests[requestId] = [this, requestId](QString nextWord)
        { subscribeResponse(nextWord, requestId); };
        return;
    }

    // Error
    if (ServerProtocolParser::isError(response))
    {
        emit subscribeError(response);
        return;
    }

    // Unknown response
    qDebug() << "Unknown subscribe response:" << response;
    emit subscribeError(response);
}

// DEPRECATED: This method is now handled by subscribeResponse
// The addPlayerToGame protocol now handles playerJoinedGame and newWord updates atomically
void ApiService::newWordResponse(const QString &response, int requestId)
{
    qDebug() << "DEPRECATED: newWordResponse called - use subscribeResponse instead";
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
