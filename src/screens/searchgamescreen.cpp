#include "searchgamescreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include "../services/serverprotocol.h"
#include "../services/apiservice.h"
#include "../utils/appstate.h"

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

    m_gamesList = new QListWidget(this);
    mainLayout->addWidget(m_gamesList);

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

    // Setup API service
    AppState& appState = AppState::getInstance();
    m_apiService = appState.getApiService();
    connect(m_apiService, &ApiService::activeGamesReceived, this, &SearchGameScreen::onActiveGamesReceived);
    connect(m_apiService, &ApiService::activeGamesError, this, &SearchGameScreen::onActiveGamesError);

    // Load games on creation
}

void SearchGameScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (screen == ScreenNavigator::SearchGameScreen) {
        m_apiService->getActiveGamesAsync();
    }
}

void SearchGameScreen::loadGames()
{
    AppState &appState = AppState::getInstance();
    if (!appState.isLoggedIn()) {
        qDebug() << "Not logged in";
        return;
    }

    m_gamesList->clear();
    m_gamesList->addItem("Loading games...");
}

void SearchGameScreen::displayGames(const QList<GameContext> &games)
{
    m_gamesList->clear();

    if (games.isEmpty()) {
        m_gamesList->addItem("No games available");
        return;
    }

    m_availableGames = games;

    for (const auto &game : games) {
        QString gameInfo = QString("Game %1 - Players: %2, Words: %3, Last: %4")
            .arg(game.gameId)
            .arg(game.playersCount)
            .arg(game.wordsCount)
            .arg(game.lastKana);

        auto *item = new QListWidgetItem(gameInfo, m_gamesList);
        item->setData(Qt::UserRole, static_cast<qulonglong>(game.gameId));
        m_gamesList->addItem(item);
    }
}

void SearchGameScreen::onJoinGameButtonClicked()
{
    auto *selectedItem = m_gamesList->currentItem();
    if (!selectedItem) {
        qDebug() << "No game selected";
        return;
    }

    qulonglong gameId = selectedItem->data(Qt::UserRole).toULongLong();

    QVariantMap gameData;
    gameData["gameId"] = gameId;
    navigate(ScreenNavigator::GameScreen, gameData);
}

void SearchGameScreen::onBackButtonClicked()
{
    goBack();
}

void SearchGameScreen::onRefreshButtonClicked()
{
    loadGames();
}

void SearchGameScreen::onActiveGamesReceived(const QList<GameContext> &games)
{
    qDebug() << "Received" << games.size() << "games";
    displayGames(games);
}

void SearchGameScreen::onActiveGamesError(const QString &error)
{
    qDebug() << "Games error:" << error;
    m_gamesList->clear();
    m_gamesList->addItem("Error: " + error);
}
