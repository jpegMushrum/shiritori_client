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
