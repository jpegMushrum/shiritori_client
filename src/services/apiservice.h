#ifndef APISERVICE_H
#define APISERVICE_H

#include <QString>
#include <QList>
#include "../models/user.h"
#include "../models/game.h"

class TcpClient;

class ApiService
{
public:
    explicit ApiService(TcpClient *tcpClient = nullptr);

    void setTcpClient(TcpClient *tcpClient);
    void setSessionId(const QString &sessionId);

    bool login(const QString &username, QString &sessionId);

    bool createGame(const QString &gameName, Game &game);
    bool getAvailableGames(QList<Game> &games);
    bool joinGame(const QString &gameId, const User &user);
    bool getGameInfo(const QString &gameId, Game &game);

    bool submitWord(const QString &gameId, const User &user, const QString &word);
    bool exitGame(const QString &gameId, const User &user);

    bool getUserStats(const User &user);

private:
    bool ensureConnected() const;
    QString sendRequest(const QString &request);

    TcpClient *m_tcpClient = nullptr;
    QString m_sessionId;
};

#endif // APISERVICE_H
