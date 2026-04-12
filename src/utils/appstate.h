#ifndef APPSTATE_H
#define APPSTATE_H

#include "../models/user.h"

class TcpClient;

class AppState
{
public:
    static AppState &getInstance();

    User getCurrentUser() const;
    void setCurrentUser(const User &user);

    QString getServerAddress() const;
    void setServerAddress(const QString &address);

    int getServerPort() const;
    void setServerPort(int port);

    QString getSessionId() const;
    void setSessionId(const QString &sessionId);

    QString getUsername() const;
    void setUsername(const QString &username);

    TcpClient* getTcpClient() const;
    void setTcpClient(TcpClient *tcpClient);

    bool isLoggedIn() const;
    void logout();

private:
    AppState() = default;
    User m_currentUser;
    QString m_serverAddress;
    int m_serverPort = 5000;
    QString m_sessionId;
    QString m_username;
    TcpClient *m_tcpClient = nullptr;
};

#endif // APPSTATE_H
