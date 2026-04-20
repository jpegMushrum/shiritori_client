#include "application.h"
#include "screens/basescreen.h"
#include "screens/loginscreen.h"
#include "screens/mainscreen.h"
#include "screens/searchgamescreen.h"
#include "screens/gamescreen.h"
#include "screens/gameendscreen.h"
#include "screens/statsscreen.h"
#include "services/connectionstatuswidget.h"
#include "services/notificationmanager.h"
#include "services/servicethread.h"
#include "utils/toastwidget.h"
#include "utils/screennavigator.h"
#include "utils/appstate.h"
#include <QFile>
#include <QCommonStyle>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QFontDatabase>

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
    // Add Fonts
    QFontDatabase::addApplicationFont(":/fonts/FunnelDisplay-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ShipporiMincho-Regular.ttf");
    setMinimumWidth(400);
    // Load application stylesheet
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(styleFile.readAll());
        qApp->setStyle(new QCommonStyle());
        qApp->setStyleSheet(style);
        styleFile.close();
    }

    // Create a central widget to hold both the stacked widget and the status widget
    auto *centralWidget = new QWidget(this);
    auto *containerLayout = new QVBoxLayout(centralWidget);
    containerLayout->setAlignment(Qt::AlignCenter);

    auto *container = new QWidget(this);
    container->setMaximumWidth(800);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    containerLayout->addWidget(container);

    auto *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_stackedWidget);

    // Create notification manager
    m_toastWidget = new ToastWidget(this);
    m_notificationManager = new NotificationManager(this);
    m_notificationManager->setToastWidget(m_toastWidget);

    // Create and start service thread (TCP and API services run in separate thread)
    m_serviceThread = new ServiceThread(this);
    m_serviceThread->start();

    auto *tcpClient = m_serviceThread->getTcpClient();
    auto *apiService = m_serviceThread->getApiService();
    m_apiService = apiService;

    // Store tcpClient in AppState for legacy code that needs server address
    AppState &appState = AppState::getInstance();
    appState.setApiTcpClient(tcpClient);

    m_navigator = new ScreenNavigator(m_stackedWidget, this);

    m_loginScreen = new LoginScreen(tcpClient, apiService, m_notificationManager, this);
    m_mainScreen = new MainScreen(apiService, m_notificationManager, this);
    m_searchGameScreen = new SearchGameScreen(apiService, m_notificationManager, this);
    m_gameScreen = new GameScreen(apiService, m_notificationManager, this);
    m_gameEndScreen = new GameEndScreen(this);
    m_statsScreen = new StatsScreen(apiService, m_notificationManager, this);

    m_loginScreen->setNavigator(m_navigator);
    m_mainScreen->setNavigator(m_navigator);
    m_searchGameScreen->setNavigator(m_navigator);
    m_gameScreen->setNavigator(m_navigator);
    m_gameEndScreen->setNavigator(m_navigator);
    m_statsScreen->setNavigator(m_navigator);

    m_stackedWidget->addWidget(m_loginScreen);      // index 0 - LoginScreen
    m_stackedWidget->addWidget(m_mainScreen);       // index 1 - MainScreen
    m_stackedWidget->addWidget(m_searchGameScreen); // index 2 - SearchGameScreen
    m_stackedWidget->addWidget(m_gameScreen);       // index 3 - GameScreen
    m_stackedWidget->addWidget(m_statsScreen);      // index 4 - StatsScreen
    m_stackedWidget->addWidget(m_gameEndScreen);    // index 5 - GameEndScreen

    m_stackedWidget->setCurrentWidget(m_loginScreen);

    // Connect GameScreen to GameEndScreen
    auto gameScreen = qobject_cast<class GameScreen *>(m_gameScreen);
    auto gameEndScreen = qobject_cast<class GameEndScreen *>(m_gameEndScreen);
    if (gameScreen && gameEndScreen)
    {
        gameScreen->setGameEndScreen(gameEndScreen);
    }

    // Create connection status widget and add it to bottom-right
    m_connectionStatusWidget = new ConnectionStatusWidget(tcpClient, apiService, this);

    m_connectionStatusWidget->setParent(this);
    m_connectionStatusWidget->raise();

    m_connectionStatusWidget->move(
        width() - m_connectionStatusWidget->width() - 20,
        height() - m_connectionStatusWidget->height() - 20);

    setCentralWidget(centralWidget);

    setWindowTitle("Shiritori Client");
    resize(1000, 640);
}

void Application::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (m_connectionStatusWidget)
    {
        m_connectionStatusWidget->move(
            width() - m_connectionStatusWidget->width() - 20,
            height() - m_connectionStatusWidget->height() - 20);
    }
}

void Application::closeEvent(QCloseEvent *event)
{
    qDebug() << "Closing app -> sending logout";

    AppState &appState = AppState::getInstance();
    m_apiService->logoutAsync(appState.getSessionId());

    event->accept();
}

void Application::connectSignals()
{
    // Navigation signals are automatically handled by the navigator
    // and screen interactions with the navigate() method
}
