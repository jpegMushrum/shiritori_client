#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QList>

class Game
{
public:
    Game() = default;
    explicit Game(const QString &name);

    // Game identification
    qulonglong getGameId() const;
    void setGameId(qulonglong gameId);

    QString getName() const;
    void setName(const QString &name);

    // Game state
    int getPlayerCount() const;
    void setPlayerCount(int count);

    int getUsedWordsCount() const;
    void setUsedWordsCount(int count);

    QList<QString> getUsedWords() const;
    void addUsedWord(const QString &word);

    QString getLastKana() const;
    void setLastKana(const QString &kana);

    // Admin and player tracking
    qulonglong getAdminId() const;
    void setAdminId(qulonglong adminId);

    qulonglong getLastPlayerId() const;
    void setLastPlayerId(qulonglong lastPlayerId);

private:
    qulonglong m_gameId = 0;
    QString m_name;
    int m_playerCount = 0;
    int m_usedWordsCount = 0;
    QList<QString> m_usedWords;
    QString m_lastKana;
    qulonglong m_adminId = 0;
    qulonglong m_lastPlayerId = 0;
};

#endif // GAME_H
