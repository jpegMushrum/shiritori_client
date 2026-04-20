#include "mainscreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "../services/serverprotocol.h"
#include "../services/apiservice.h"
#include "../services/notificationmanager.h"
#include "../utils/appstate.h"
#include <QGraphicsDropShadowEffect>

MainScreen::MainScreen(ApiService *apiService, NotificationManager *notificationManager, QWidget *parent)
    : BaseScreen(parent), m_apiService(apiService), m_notificationManager(notificationManager)
{
    setupUI();
}

void MainScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Main Menu", this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(30);

    auto *newGameButton = new QPushButton("New Game", this);
    newGameButton->setMinimumHeight(50);
    connect(newGameButton, &QPushButton::clicked, this, &MainScreen::onNewGameButtonClicked);
    mainLayout->addWidget(newGameButton);

    auto *searchGameButton = new QPushButton("Search for Game", this);
    searchGameButton->setMinimumHeight(50);
    connect(searchGameButton, &QPushButton::clicked, this, &MainScreen::onSearchGameButtonClicked);
    mainLayout->addWidget(searchGameButton);

    auto *statsButton = new QPushButton("View Stats", this);
    statsButton->setMinimumHeight(50);
    connect(statsButton, &QPushButton::clicked, this, &MainScreen::onStatsButtonClicked);
    mainLayout->addWidget(statsButton);

    mainLayout->addSpacing(30);

    auto *logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(40);
    connect(logoutButton, &QPushButton::clicked, this, &MainScreen::onLogoutButtonClicked);
    mainLayout->addWidget(logoutButton);

    mainLayout->addStretch();

    // Connect API service signals
    connect(m_apiService, &ApiService::gameStarted, this, &MainScreen::onGameStarted);
    connect(m_apiService, &ApiService::gameStartError, this, &MainScreen::onGameStartError);
}

void MainScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (screen == ScreenNavigator::MainScreen)
    {
        // Smth
    }
}

void MainScreen::onNewGameButtonClicked()
{
    startNewGame();
}

void MainScreen::startNewGame()
{
    AppState &appState = AppState::getInstance();
    if (!appState.isLoggedIn())
    {
        qDebug() << "Not logged in";
        navigate(ScreenNavigator::LoginScreen);
        return;
    }

    qDebug() << "Starting new game...";
    m_apiService->startNewGameAsync(appState.getSessionId());
}

void MainScreen::onGameStarted(const GameContext &game)
{
    qDebug() << "Game started with ID:" << game.gameId;

    QVariantMap gameData;
    gameData["gameId"] = game.gameId;
    navigate(ScreenNavigator::GameScreen, gameData);
}

void MainScreen::onGameStartError(const QString &error)
{
    qDebug() << "Failed to start game:" << error;
    // Could show error dialog here
}

void MainScreen::onSearchGameButtonClicked()
{
    navigate(ScreenNavigator::SearchGameScreen);
}

void MainScreen::onStatsButtonClicked()
{
    navigate(ScreenNavigator::StatsScreen);
}

void MainScreen::onLogoutButtonClicked()
{
    AppState &appState = AppState::getInstance();
    qDebug() << "Logout button: " << appState.isLoggedIn() << " " << m_apiService;
    if (!appState.isLoggedIn())
    {
        navigate(ScreenNavigator::LoginScreen);
        return;
    }

    if (m_apiService)
    {
        m_apiService->logoutAsync(appState.getSessionId());
    }
    appState.logout();

    navigate(ScreenNavigator::LoginScreen);
}
