#include "game.h"

Game::Game(const QString &name)
    : m_name(name)
{
}

qulonglong Game::getGameId() const
{
    return m_gameId;
}

void Game::setGameId(qulonglong gameId)
{
    m_gameId = gameId;
}

QString Game::getName() const
{
    return m_name;
}

void Game::setName(const QString &name)
{
    m_name = name;
}

int Game::getPlayerCount() const
{
    return m_playerCount;
}

void Game::setPlayerCount(int count)
{
    m_playerCount = count;
}

int Game::getUsedWordsCount() const
{
    return m_usedWordsCount;
}

void Game::setUsedWordsCount(int count)
{
    m_usedWordsCount = count;
}

QList<QString> Game::getUsedWords() const
{
    return m_usedWords;
}

void Game::addUsedWord(const QString &word)
{
    m_usedWords.append(word);
    m_usedWordsCount = m_usedWords.count();
}

QString Game::getLastKana() const
{
    return m_lastKana;
}

void Game::setLastKana(const QString &kana)
{
    m_lastKana = kana;
}

qulonglong Game::getAdminId() const
{
    return m_adminId;
}

void Game::setAdminId(qulonglong adminId)
{
    m_adminId = adminId;
}

qulonglong Game::getLastPlayerId() const
{
    return m_lastPlayerId;
}

void Game::setLastPlayerId(qulonglong lastPlayerId)
{
    m_lastPlayerId = lastPlayerId;
}
