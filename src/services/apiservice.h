#ifndef APISERVICE_H
#define APISERVICE_H

#include <QString>
#include <QList>
#include "../models/user.h"
#include "../models/game.h"

class ApiService
{
public:
    ApiService() = default;

    // TODO: User management
    bool login(const QString &username, User &user);
    bool registerUser(const QString &username, User &user);

    // TODO: Game management
    bool createGame(const QString &gameName, Game &game);
    bool getAvailableGames(QList<Game> &games);
    bool joinGame(const QString &gameId, const User &user);
    bool getGameInfo(const QString &gameId, Game &game);

    // TODO: Game actions
    bool submitWord(const QString &gameId, const User &user, const QString &word);
    bool exitGame(const QString &gameId, const User &user);

    // TODO: Stats
    bool getUserStats(const User &user);

private:
    // TODO: Add connection management
};

#endif // APISERVICE_H
