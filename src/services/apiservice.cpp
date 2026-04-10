#include "apiservice.h"
#include "tcpclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ApiService::ApiService(TcpClient *tcpClient)
    : m_tcpClient(tcpClient)
{
}

void ApiService::setTcpClient(TcpClient *tcpClient)
{
    m_tcpClient = tcpClient;
}

void ApiService::setSessionId(const QString &sessionId)
{
    m_sessionId = sessionId;
}

bool ApiService::ensureConnected() const
{
    return m_tcpClient && m_tcpClient->isConnected();
}

QString ApiService::sendRequest(const QString &request)
{
    if (!ensureConnected())
    {
        qDebug() << "Not connected to server";
        return QString();
    }

    if (!m_tcpClient->sendData(request))
    {
        qDebug() << "Failed to send data";
        return QString();
    }

    return m_tcpClient->receiveData();
}

bool ApiService::login(const QString &username, QString &sessionId)
{
    if (!ensureConnected())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::createGame(const QString &gameName, Game &game)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::getAvailableGames(QList<Game> &games)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::joinGame(const QString &gameId, const User &user)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::getGameInfo(const QString &gameId, Game &game)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::submitWord(const QString &gameId, const User &user, const QString &word)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::exitGame(const QString &gameId, const User &user)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO

    return false;
}

bool ApiService::getUserStats(const User &user)
{
    if (!ensureConnected() || m_sessionId.isEmpty())
    {
        return false;
    }

    // TODO
    return false;
}
