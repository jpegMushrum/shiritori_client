#include "mainscreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

MainScreen::MainScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void MainScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Main Menu", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(30);

    auto *newGameButton = new QPushButton("New Game", this);
    newGameButton->setMinimumHeight(50);
    newGameButton->setStyleSheet("font-size: 14px;");
    connect(newGameButton, &QPushButton::clicked, this, &MainScreen::onNewGameButtonClicked);
    mainLayout->addWidget(newGameButton);

    auto *searchGameButton = new QPushButton("Search for Game", this);
    searchGameButton->setMinimumHeight(50);
    searchGameButton->setStyleSheet("font-size: 14px;");
    connect(searchGameButton, &QPushButton::clicked, this, &MainScreen::onSearchGameButtonClicked);
    mainLayout->addWidget(searchGameButton);

    auto *statsButton = new QPushButton("View Stats", this);
    statsButton->setMinimumHeight(50);
    statsButton->setStyleSheet("font-size: 14px;");
    connect(statsButton, &QPushButton::clicked, this, &MainScreen::onStatsButtonClicked);
    mainLayout->addWidget(statsButton);

    mainLayout->addSpacing(30);

    auto *logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(40);
    connect(logoutButton, &QPushButton::clicked, this, &MainScreen::onLogoutButtonClicked);
    mainLayout->addWidget(logoutButton);

    mainLayout->addStretch();
}

void MainScreen::onNewGameButtonClicked()
{
    QVariantMap gameData;
    gameData["gameId"] = "";
    navigate(ScreenNavigator::GameScreen, gameData);
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
    navigate(ScreenNavigator::LoginScreen);
}
