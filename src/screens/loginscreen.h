#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "basescreen.h"
#include <QLineEdit>
#include <QLabel>

#include "../services/apiservice.h"
#include "../services/notificationmanager.h"

class LoginScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit LoginScreen(TcpClient *tcpClient, ApiService *apiService,
                         NotificationManager *notificationManager, QWidget *parent = nullptr);
    ~LoginScreen() override;

private slots:
    void onLoginButtonClicked();
    void onApiLoginSuccess(const QString &sessionId);
    void onApiLoginError(const QString &errorMessage);
    void onTcpConnectFailed(const QString &errorMessage);
    void onTcpConnectSuccess();
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) override;

private:
    void setupUI();
    bool validateInput();

    bool m_loginRequested = false;

    QLineEdit *m_serverAddressInput = nullptr;
    QLineEdit *m_serverPortInput = nullptr;
    QLineEdit *m_usernameInput = nullptr;
    QLabel *m_errorLabel = nullptr;

    TcpClient *m_tcpClient = nullptr;
    ApiService *m_apiService = nullptr;
    class NotificationManager *m_notificationManager = nullptr;
};

#endif // LOGINSCREEN_H
