#include "game.h"

Game::Game(const QString &name)
    : m_name(name)
{
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
