#include "application.h"
#include "screens/basescreen.h"
#include "screens/loginscreen.h"
#include "screens/mainscreen.h"
#include "screens/searchgamescreen.h"
#include "screens/gamescreen.h"
#include "screens/statsscreen.h"
#include "services/tcpclient.h"
#include "services/connectionstatuswidget.h"
#include "utils/constants.h"
#include "utils/screennavigator.h"
#include <QWidget>
#include <QVBoxLayout>

Application::Application(QWidget *parent)
    : QMainWindow(parent), m_tcpClient(std::make_unique<TcpClient>(this))
{
    setupUI();
    connectSignals();
}

Application::~Application()
{
}

void Application::setupUI()
{
    // Create a central widget to hold both the stacked widget and the status widget
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);

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

    // Create connection status widget and add it to bottom-right
    m_connectionStatusWidget = new ConnectionStatusWidget(this);
    m_connectionStatusWidget->setTcpClient(m_tcpClient.get());

    // Create a container for the status widget in bottom-right
    auto *statusContainer = new QWidget(this);
    auto *statusLayout = new QVBoxLayout(statusContainer);
    statusLayout->setContentsMargins(10, 10, 10, 10);
    statusLayout->addStretch();
    statusLayout->addWidget(m_connectionStatusWidget, 0, Qt::AlignBottom | Qt::AlignRight);

    mainLayout->addWidget(statusContainer);

    setCentralWidget(centralWidget);

    setWindowTitle("Shiritori Client");
    resize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
}

void Application::connectSignals()
{
    // Navigation signals are automatically handled by the navigator
    // and screen interactions with the navigate() method
}
