#include "application.h"
#include "screens/loginscreen.h"
#include "screens/mainscreen.h"
#include "screens/searchgamescreen.h"
#include "screens/gamescreen.h"
#include "screens/statsscreen.h"
#include "utils/constants.h"

Application::Application(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connectSignals();
}

void Application::setupUI()
{
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    m_loginScreen = new LoginScreen(this);
    m_mainScreen = new MainScreen(this);
    m_searchGameScreen = new SearchGameScreen(this);
    m_gameScreen = new GameScreen(this);
    m_statsScreen = new StatsScreen(this);

    m_stackedWidget->addWidget(m_loginScreen);
    m_stackedWidget->addWidget(m_mainScreen);
    m_stackedWidget->addWidget(m_searchGameScreen);
    m_stackedWidget->addWidget(m_gameScreen);
    m_stackedWidget->addWidget(m_statsScreen);

    m_stackedWidget->setCurrentWidget(m_loginScreen);

    setWindowTitle("Shiritori Client");
    resize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
}

void Application::connectSignals()
{
    // TODO: Connect screen navigation signals
}
