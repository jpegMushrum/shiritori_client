#include "gameendscreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <algorithm>

GameEndScreen::GameEndScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void GameEndScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Game Over", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    // Create scores table
    m_scoresTable = new QTableWidget(this);
    m_scoresTable->setColumnCount(2);
    m_scoresTable->setHorizontalHeaderLabels({"User ID", "Score"});
    m_scoresTable->horizontalHeader()->setStretchLastSection(true);
    m_scoresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_scoresTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_scoresTable->setColumnWidth(0, 150);
    m_scoresTable->setColumnWidth(1, 150);
    m_scoresTable->setSortingEnabled(true);
    mainLayout->addWidget(m_scoresTable);

    mainLayout->addSpacing(15);

    auto *buttonLayout = new QHBoxLayout();
    auto *mainMenuButton = new QPushButton("Main Menu", this);
    mainMenuButton->setMinimumHeight(40);
    connect(mainMenuButton, &QPushButton::clicked, this, &GameEndScreen::onMainMenuButtonClicked);
    buttonLayout->addWidget(mainMenuButton);

    mainLayout->addLayout(buttonLayout);
}

void GameEndScreen::displayGameResults(const GameStoppedEvent &event)
{
    m_lastEvent = event;

    m_scoresTable->setRowCount(0);

    // Sort scores by score descending
    auto sortedScores = event.scores;
    std::sort(sortedScores.begin(), sortedScores.end(),
              [](const PlayerScore &a, const PlayerScore &b)
              {
                  return a.score > b.score;
              });

    int row = 0;
    for (const auto &score : sortedScores)
    {
        m_scoresTable->insertRow(row);

        auto *userIdItem = new QTableWidgetItem(QString::number(score.userId));
        auto *scoreItem = new QTableWidgetItem(QString::number(score.score));

        userIdItem->setFlags(userIdItem->flags() & ~Qt::ItemIsEditable);
        scoreItem->setFlags(scoreItem->flags() & ~Qt::ItemIsEditable);

        // Center align
        userIdItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);

        // Set user role for numeric sorting
        scoreItem->setData(Qt::UserRole, score.score);

        m_scoresTable->setItem(row, 0, userIdItem);
        m_scoresTable->setItem(row, 1, scoreItem);

        row++;
    }
}

void GameEndScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap& data = {}) {
    if (screen == ScreenNavigator::GameEndScreen) {
        // Something
    }
}

void GameEndScreen::onMainMenuButtonClicked()
{
    navigate(ScreenNavigator::MainScreen);
}
