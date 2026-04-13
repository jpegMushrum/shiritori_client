#include "gamescreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>

GameScreen::GameScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void GameScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Shiritori Game", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(15);

    auto *lastKanaLabel = new QLabel("Last Kana: あ", this);
    lastKanaLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(lastKanaLabel);

    mainLayout->addSpacing(15);

    auto *wordsLabel = new QLabel("Used Words:", this);
    mainLayout->addWidget(wordsLabel);

    auto *wordsList = new QListWidget(this);
    mainLayout->addWidget(wordsList);

    mainLayout->addSpacing(15);

    auto *inputLabel = new QLabel("Your Word:", this);
    mainLayout->addWidget(inputLabel);

    auto *wordInput = new QLineEdit(this);
    wordInput->setPlaceholderText("Enter a word...");
    mainLayout->addWidget(wordInput);

    auto *submitButton = new QPushButton("Submit", this);
    submitButton->setMinimumHeight(40);
    connect(submitButton, &QPushButton::clicked, this, &GameScreen::onSubmitButtonClicked);
    mainLayout->addWidget(submitButton);

    mainLayout->addSpacing(10);

    auto *exitButton = new QPushButton("Exit Game", this);
    exitButton->setMinimumHeight(40);
    connect(exitButton, &QPushButton::clicked, this, &GameScreen::onExitButtonClicked);
    mainLayout->addWidget(exitButton);
}

void GameScreen::onSubmitButtonClicked()
{
    // TODO: Submit word to server
}

void GameScreen::onOpen(ScreenNavigator::ScreenType screen)
{
    // TODO: Submit word to server
}


void GameScreen::onExitButtonClicked()
{
    navigate(ScreenNavigator::MainScreen);
}
