#include "statsscreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

StatsScreen::StatsScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void StatsScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Your Statistics", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(30);

    auto *winsLabel = new QLabel("Wins: 0", this);
    winsLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(winsLabel);

    auto *lossesLabel = new QLabel("Losses: 0", this);
    lossesLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(lossesLabel);

    auto *winRateLabel = new QLabel("Win Rate: 0%", this);
    winRateLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(winRateLabel);

    auto *streakLabel = new QLabel("Best Streak: 0", this);
    streakLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(streakLabel);

    mainLayout->addSpacing(30);

    auto *backButton = new QPushButton("Back", this);
    backButton->setMinimumHeight(40);
    connect(backButton, &QPushButton::clicked, this, &StatsScreen::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    mainLayout->addStretch();
}

void StatsScreen::onBackButtonClicked()
{
    goBack();
}
