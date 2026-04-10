#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "basescreen.h"

class QLineEdit;
class QLabel;
class TcpClient;

class LoginScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen() override;

private slots:
    void onLoginButtonClicked();
    void onLoginSuccess();
    void onLoginError(const QString &errorMessage);

private:
    void setupUI();
    void setupTcpClient();
    bool validateInput();

    QLineEdit *m_serverAddressInput = nullptr;
    QLineEdit *m_serverPortInput = nullptr;
    QLineEdit *m_usernameInput = nullptr;
    QLabel *m_errorLabel = nullptr;

    TcpClient *m_tcpClient = nullptr;
};

#endif // LOGINSCREEN_H
