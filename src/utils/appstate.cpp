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

bool AppState::isLoggedIn() const
{
    return !m_currentUser.getUsername().isEmpty();
}

void AppState::logout()
{
    m_currentUser = User();
}
