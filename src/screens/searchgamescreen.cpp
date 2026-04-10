#include "searchgamescreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>

SearchGameScreen::SearchGameScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void SearchGameScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Search for Game", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(15);

    auto *searchLayout = new QHBoxLayout();
    auto *searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Search by game name...");
    searchLayout->addWidget(searchInput);

    auto *refreshButton = new QPushButton("Refresh", this);
    refreshButton->setMaximumWidth(100);
    connect(refreshButton, &QPushButton::clicked, this, &SearchGameScreen::onRefreshButtonClicked);
    searchLayout->addWidget(refreshButton);

    mainLayout->addLayout(searchLayout);

    mainLayout->addSpacing(15);

    auto *gamesList = new QListWidget(this);
    mainLayout->addWidget(gamesList);

    mainLayout->addSpacing(15);

    auto *buttonLayout = new QHBoxLayout();
    auto *joinButton = new QPushButton("Join Game", this);
    joinButton->setMinimumHeight(40);
    connect(joinButton, &QPushButton::clicked, this, &SearchGameScreen::onJoinGameButtonClicked);
    buttonLayout->addWidget(joinButton);

    auto *backButton = new QPushButton("Back", this);
    backButton->setMinimumHeight(40);
    connect(backButton, &QPushButton::clicked, this, &SearchGameScreen::onBackButtonClicked);
    buttonLayout->addWidget(backButton);

    mainLayout->addLayout(buttonLayout);
}

void SearchGameScreen::onJoinGameButtonClicked()
{
    QVariantMap gameData;
    gameData["gameId"] = "selected_game_id";
    navigate(ScreenNavigator::GameScreen, gameData);
}

void SearchGameScreen::onBackButtonClicked()
{
    goBack();
}

void SearchGameScreen::onRefreshButtonClicked()
{
    // TODO: Refresh games list from server
}
