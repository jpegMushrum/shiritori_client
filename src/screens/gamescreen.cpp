#include "gamescreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>

#include "../utils/appstate.h"

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

    // Setting up api
    AppState &appState = AppState::getInstance();
    m_apiService = appState.getApiService();

    connectSignals();
}

void GameScreen::connectSignals()
{
    if (m_apiService)
    {
        connect(m_apiService, &ApiService::newWordReceived, this, &GameScreen::onNewWordReceived);
        connect(m_apiService, &ApiService::subscribeError, this, &GameScreen::onSubscribeError);
        connect(m_apiService, &ApiService::subscribeSuccess, this, &GameScreen::onSubscribeSuccess);
    }
}

void GameScreen::onSubmitButtonClicked()
{
    // TODO: Submit word to server
}

void GameScreen::onNewWordReceived(const NewWordUpdate &update)
{
    qDebug() << "GameScreen: New word received -" << update.kanji;
    // TODO: Update UI with new word information
}

void GameScreen::onSubscribeSuccess()
{
    qDebug() << "Successfully subscribed to game updates";
    // TODO: Update UI to show game is active
}

void GameScreen::onSubscribeError(const QString &error)
{
    qWarning() << "Failed to subscribe to game:" << error;
    // TODO: Show error message to user
}

void GameScreen::onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (data.empty())
    {
        return;
    }

    qDebug() << "GameScreen Opened" << data;
    if (screen == ScreenNavigator::GameScreen)
    {
        m_gameId = data["gameId"].toULongLong();
        if (m_apiService)
        {
            m_apiService->subscribeOnGameAsync(m_gameId);
        }
    }
}

void GameScreen::onExitButtonClicked()
{
    navigate(ScreenNavigator::MainScreen);
}
