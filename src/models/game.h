#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QList>

class Game
{
public:
    Game() = default;
    explicit Game(const QString &name);

    QString getName() const;
    void setName(const QString &name);

    int getPlayerCount() const;
    void setPlayerCount(int count);

    int getUsedWordsCount() const;
    void setUsedWordsCount(int count);

    QList<QString> getUsedWords() const;
    void addUsedWord(const QString &word);

    QString getLastKana() const;
    void setLastKana(const QString &kana);

private:
    QString m_name;
    int m_playerCount = 0;
    int m_usedWordsCount = 0;
    QList<QString> m_usedWords;
    QString m_lastKana;
};

#endif // GAME_H
