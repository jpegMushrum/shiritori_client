#include "gameendscreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

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
    titleLabel->setObjectName("largeTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    mainLayout->addSpacing(40);

    // EndGame Image
    auto *container = new QWidget(this);
    container->setObjectName("imageContainer");
    container->setMaximumHeight(300);
    container->setMaximumWidth(300);

    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    m_imageLabel = new QLabel(container);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(true);

    QPixmap pixmap(":/endgame_pic.jpg");
    m_imageLabel->setPixmap(pixmap);

    layout->addWidget(m_imageLabel);

    mainLayout->addWidget(container, 0, Qt::AlignCenter);

    mainLayout->addSpacing(40);

    // Score display
    m_scoreLabel = new QLabel(this);
    m_scoreLabel->setObjectName("scoreLabel");
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_scoreLabel);

    mainLayout->addSpacing(30);

    // Main menu button
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    auto *mainMenuButton = new QPushButton("Main Menu", this);
    mainMenuButton->setMinimumHeight(40);
    mainMenuButton->setMinimumWidth(120);
    connect(mainMenuButton, &QPushButton::clicked, this, &GameEndScreen::onMainMenuButtonClicked);
    buttonLayout->addWidget(mainMenuButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void GameEndScreen::displayGameResults(const GameStoppedEvent &event)
{
    m_lastEvent = event;

    const PlayerScore &score = event.scores;
    m_scoreLabel->setText(QString("Your Score: %1").arg(score.score));
    qDebug() << "Game ended. Your score:" << score.score;
}

void GameEndScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (screen == ScreenNavigator::GameEndScreen)
    {
        // Handle screen open if needed
    }
}

void GameEndScreen::onMainMenuButtonClicked()
{
    navigate(ScreenNavigator::MainScreen);
}
