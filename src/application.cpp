#include "application.h"
#include "screens/basescreen.h"
#include "screens/loginscreen.h"
#include "screens/mainscreen.h"
#include "screens/searchgamescreen.h"
#include "screens/gamescreen.h"
#include "screens/statsscreen.h"
#include "utils/constants.h"
#include "utils/screennavigator.h"

Application::Application(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connectSignals();
}

Application::~Application()
{
}

void Application::setupUI()
{
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    m_navigator = new ScreenNavigator(m_stackedWidget, this);

    m_loginScreen = new LoginScreen(this);
    m_mainScreen = new MainScreen(this);
    m_searchGameScreen = new SearchGameScreen(this);
    m_gameScreen = new GameScreen(this);
    m_statsScreen = new StatsScreen(this);

    m_loginScreen->setNavigator(m_navigator);
    m_mainScreen->setNavigator(m_navigator);
    m_searchGameScreen->setNavigator(m_navigator);
    m_gameScreen->setNavigator(m_navigator);
    m_statsScreen->setNavigator(m_navigator);

    m_stackedWidget->addWidget(m_loginScreen);      // index 0 - LoginScreen
    m_stackedWidget->addWidget(m_mainScreen);       // index 1 - MainScreen
    m_stackedWidget->addWidget(m_searchGameScreen); // index 2 - SearchGameScreen
    m_stackedWidget->addWidget(m_gameScreen);       // index 3 - GameScreen
    m_stackedWidget->addWidget(m_statsScreen);      // index 4 - StatsScreen

    m_stackedWidget->setCurrentWidget(m_loginScreen);

    setWindowTitle("Shiritori Client");
    resize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
}

void Application::connectSignals()
{
    // Navigation signals are automatically handled by the navigator
    // and screen interactions with the navigate() method
}
