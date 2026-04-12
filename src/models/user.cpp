#include "user.h"

User::User(const QString &username)
    : m_username(username)
{
}

QString User::getUsername() const
{
    return m_username;
}

void User::setUsername(const QString &username)
{
    m_username = username;
}

QString User::getSessionId() const
{
    return m_sessionId;
}

void User::setSessionId(const QString &sessionId)
{
    m_sessionId = sessionId;
}

int User::getWins() const
{
    return m_wins;
}

void User::setWins(int wins)
{
    m_wins = wins;
}

int User::getLosses() const
{
    return m_losses;
}

void User::setLosses(int losses)
{
    m_losses = losses;
}

qulonglong User::getUserId() const
{
    return m_userId;
}

void User::setUserId(qulonglong userId)
{
    m_userId = userId;
}

double User::getAverageWordsPerGame() const
{
    return m_averageWordsPerGame;
}

void User::setAverageWordsPerGame(double average)
{
    m_averageWordsPerGame = average;
}
