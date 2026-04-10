#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QStackedWidget>

class Application : public QMainWindow
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);

private:
    void setupUI();
    void connectSignals();

    QStackedWidget *m_stackedWidget = nullptr;

    // TODO: Screen pointers
    class LoginScreen *m_loginScreen = nullptr;
    class MainScreen *m_mainScreen = nullptr;
    class SearchGameScreen *m_searchGameScreen = nullptr;
    class GameScreen *m_gameScreen = nullptr;
    class StatsScreen *m_statsScreen = nullptr;
};

#endif // APPLICATION_H
