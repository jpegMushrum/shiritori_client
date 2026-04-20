#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QStackedWidget>
#include "utils/screennavigator.h"

class TcpClient;
class ConnectionStatusWidget;
class NotificationManager;
class ToastWidget;

class Application : public QMainWindow
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);
    ~Application() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void connectSignals();

    QStackedWidget *m_stackedWidget = nullptr;
    ScreenNavigator *m_navigator = nullptr;
    ConnectionStatusWidget *m_connectionStatusWidget = nullptr;
    NotificationManager *m_notificationManager = nullptr;
    ToastWidget *m_toastWidget = nullptr;

    // Screen pointers
    class BaseScreen *m_loginScreen = nullptr;
    class BaseScreen *m_mainScreen = nullptr;
    class BaseScreen *m_searchGameScreen = nullptr;
    class BaseScreen *m_gameScreen = nullptr;
    class BaseScreen *m_gameEndScreen = nullptr;
    class BaseScreen *m_statsScreen = nullptr;
};

#endif // APPLICATION_H
