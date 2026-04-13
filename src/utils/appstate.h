#ifndef APPSTATE_H
#define APPSTATE_H

#include "../models/user.h"
#include "../services/apiservice.h"

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

    ApiService* getApiService() const;
    void setApiService(ApiService *apiService);

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
    ApiService *m_apiService = nullptr;
};

#endif // APPSTATE_H
