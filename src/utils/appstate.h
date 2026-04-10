#ifndef APPSTATE_H
#define APPSTATE_H

#include "../models/user.h"

class AppState
{
public:
    static AppState &getInstance();

    User getCurrentUser() const;
    void setCurrentUser(const User &user);

    bool isLoggedIn() const;
    void logout();

private:
    AppState() = default;
    User m_currentUser;
};

#endif // APPSTATE_H
