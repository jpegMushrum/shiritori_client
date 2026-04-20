#include "searchgamescreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include "../services/serverprotocol.h"
#include "../services/apiservice.h"
#include "../services/notificationmanager.h"
#include "../utils/appstate.h"

SearchGameScreen::SearchGameScreen(ApiService *apiService, NotificationManager *notificationManager, QWidget *parent)
    : BaseScreen(parent), m_apiService(apiService), m_notificationManager(notificationManager)
{
    setupUI();
}

void SearchGameScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Search for Game", this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(15);

    // Create table with sortable columns
    auto *container = new QWidget(this);
    container->setObjectName("tableContainer");
    mainLayout->addWidget(container);

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_gamesTable = new QTableWidget(container);
    m_gamesTable->setColumnCount(4);
    m_gamesTable->setHorizontalHeaderLabels({"Game ID", "Players", "Words", "Last Kana"});
    m_gamesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_gamesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_gamesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_gamesTable->setSortingEnabled(true);
    m_gamesTable->verticalHeader()->setVisible(false);
    m_gamesTable->setShowGrid(false);
    connect(m_gamesTable, &QTableWidget::cellClicked, this, &SearchGameScreen::onTableCellClicked);
    layout->addWidget(m_gamesTable);

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

    buttonLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(100);

    // Connect API service signals
    connect(m_apiService, &ApiService::activeGamesReceived, this, &SearchGameScreen::onActiveGamesReceived);
    connect(m_apiService, &ApiService::activeGamesError, this, &SearchGameScreen::onActiveGamesError);
}

void SearchGameScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (screen == ScreenNavigator::SearchGameScreen)
    {
        m_gamesTable->sortByColumn(0, Qt::AscendingOrder);
        m_apiService->getActiveGamesAsync();
    }
}

void SearchGameScreen::loadGames()
{
    // This method is called when the screen opens to request active games
}

void SearchGameScreen::displayGames(const QList<GameContext> &games)
{
    m_gamesTable->setRowCount(0);

    if (games.isEmpty())
    {
        m_gamesTable->insertRow(0);
        auto *item = new QTableWidgetItem("No games available");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setTextAlignment(Qt::AlignCenter);
        m_gamesTable->setItem(0, 0, item);
        return;
    }

    int row = 0;
    for (const auto &game : games)
    {
        m_gamesTable->insertRow(row);

        auto *idItem = new QTableWidgetItem(QString::number(game.gameId));
        auto *playersItem = new QTableWidgetItem(QString::number(game.playersCount));
        auto *wordsItem = new QTableWidgetItem(QString::number(game.wordsCount));
        auto *kanaItem = new QTableWidgetItem(game.lastKana);

        idItem->setData(Qt::UserRole, static_cast<qulonglong>(game.gameId));

        playersItem->setData(Qt::UserRole, game.playersCount);
        wordsItem->setData(Qt::UserRole, game.wordsCount);

        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        playersItem->setFlags(playersItem->flags() & ~Qt::ItemIsEditable);
        wordsItem->setFlags(wordsItem->flags() & ~Qt::ItemIsEditable);
        kanaItem->setFlags(kanaItem->flags() & ~Qt::ItemIsEditable);

        idItem->setTextAlignment(Qt::AlignCenter);
        playersItem->setTextAlignment(Qt::AlignCenter);
        wordsItem->setTextAlignment(Qt::AlignCenter);
        kanaItem->setTextAlignment(Qt::AlignCenter);

        m_gamesTable->setItem(row, 0, idItem);
        m_gamesTable->setItem(row, 1, playersItem);
        m_gamesTable->setItem(row, 2, wordsItem);
        m_gamesTable->setItem(row, 3, kanaItem);

        row++;
    }
}

void SearchGameScreen::onTableCellClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0 || row >= m_gamesTable->rowCount())
        return;

    auto *idItem = m_gamesTable->item(row, 0);
    if (idItem)
    {
        bool ok;
        m_selectedGameId = idItem->data(Qt::UserRole).toULongLong(&ok);
        if (!ok)
            m_selectedGameId = -1;
        qDebug() << "Selected game ID:" << m_selectedGameId;
    }
}

void SearchGameScreen::onJoinGameButtonClicked()
{
    if (m_selectedGameId == static_cast<qulonglong>(-1))
    {
        qDebug() << "No game selected";
        return;
    }

    QVariantMap gameData;
    gameData["gameId"] = m_selectedGameId;
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
}
