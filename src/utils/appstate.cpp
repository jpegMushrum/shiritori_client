#include "appstate.h"

AppState &AppState::getInstance()
{
    static AppState instance;
    return instance;
}

User AppState::getCurrentUser() const
{
    return m_currentUser;
}

void AppState::setCurrentUser(const User &user)
{
    m_currentUser = user;
}

QString AppState::getServerAddress() const
{
    return m_serverAddress;
}

void AppState::setServerAddress(const QString &address)
{
    m_serverAddress = address;
}

int AppState::getServerPort() const
{
    return m_serverPort;
}

void AppState::setServerPort(int port)
{
    m_serverPort = port;
}

QString AppState::getSessionId() const
{
    return m_sessionId;
}

void AppState::setSessionId(const QString &sessionId)
{
    m_sessionId = sessionId;
}

QString AppState::getUsername() const
{
    return m_username;
}

void AppState::setUsername(const QString &username)
{
    m_username = username;
}

bool AppState::isLoggedIn() const
{
    return !m_currentUser.getUsername().isEmpty() && !m_sessionId.isEmpty();
}

void AppState::logout()
{
    m_currentUser = User();
    m_sessionId.clear();
    m_username.clear();
}
