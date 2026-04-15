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

    m_lastKanaLabel = new QLabel("Last Kana: あ", this);
    m_lastKanaLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(m_lastKanaLabel);

    mainLayout->addSpacing(15);

    auto *wordsLabel = new QLabel("Used Words:", this);
    mainLayout->addWidget(wordsLabel);

    m_wordsList = new QListWidget(this);
    mainLayout->addWidget(m_wordsList);

    mainLayout->addSpacing(15);

    auto *inputLabel = new QLabel("Your Word:", this);
    mainLayout->addWidget(inputLabel);

    m_wordInput = new QLineEdit(this);
    m_wordInput->setPlaceholderText("Enter a word...");
    mainLayout->addWidget(m_wordInput);

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
        connect(m_apiService, &ApiService::wordHandled, this, &GameScreen::onWordHandled);
        connect(m_apiService, &ApiService::wordHandleError, this, &GameScreen::onWordHandleError);
    }
}

void GameScreen::onSubmitButtonClicked()
{
    if (!m_wordInput || m_wordInput->text().isEmpty())
    {
        qWarning() << "Word input is empty";
        return;
    }

    if (m_gameId == -1 || m_sessionId.isEmpty())
    {
        qWarning() << "Game ID or Session ID not set";
        return;
    }

    QString word = m_wordInput->text().trimmed();
    m_wordInput->clear();

    if (m_apiService)
    {
        m_apiService->handleWordAsync(m_gameId, m_sessionId, word);
    }
}

void GameScreen::onNewWordReceived(const NewWordUpdate &update)
{
    qDebug() << "GameScreen: New word received -" << update.kanji << "Meaning:" << update.meaning;

    // Add word to used words list
    if (m_wordsList && !update.kanji.isEmpty())
    {
        QString displayText = update.kanji;
        if (!update.meaning.isEmpty())
        {
            displayText += QString(" (%1)").arg(update.meaning);
        }
        m_wordsList->addItem(displayText);
    }

    // Update last kana - get the last character of the word
    if (m_lastKanaLabel && !update.kanji.isEmpty())
    {
        QString lastChar = update.readings[0].right(1);
        m_lastKanaLabel->setText(QString("Last Kana: %1").arg(lastChar));
    }
}

void GameScreen::onSubscribeSuccess()
{
    qDebug() << "Successfully subscribed to game updates";

    // Clear previous game state when subscribed
    if (m_wordsList)
    {
        m_wordsList->clear();
    }

    if (m_lastKanaLabel)
    {
        m_lastKanaLabel->setText("Last Kana: (waiting for first word)");
    }

    // Enable input field
    if (m_wordInput)
    {
        m_wordInput->setEnabled(true);
        m_wordInput->setFocus();
    }
}

void GameScreen::onSubscribeError(const QString &error)
{
    qWarning() << "Failed to subscribe to game:" << error;

    if (m_lastKanaLabel)
    {
        m_lastKanaLabel->setText(QString("Error: %1").arg(error));
    }

    // Enable input field again so user can retry or exit
    if (m_wordInput)
    {
        m_wordInput->setEnabled(true);
    }
}

void GameScreen::onWordHandled(HandleWordStatus status)
{
    qDebug() << "Word submitted. Status:" << static_cast<int>(status);

    switch (status)
    {
    case HandleWordStatus::OK:
        qDebug() << "Word accepted!";
        // The new word will come via subscription
        break;
    case HandleWordStatus::WRONG_ORDER:
        qWarning() << "Word doesn't start with correct hiragana";
        break;
    case HandleWordStatus::NOT_JAPANESE_WORD:
        qWarning() << "Word not found in dictionary";
        break;
    case HandleWordStatus::NO_SPEACH_PART:
        qWarning() << "Word missing required speech part";
        break;
    case HandleWordStatus::GOT_END_WORD:
        qWarning() << "Word ends with ん - game over!";
        break;
    case HandleWordStatus::GOT_DOUBLED_WORD:
        qWarning() << "Word was already said in this game";
        break;
    case HandleWordStatus::GAME_NOT_FOUND:
        qWarning() << "Game not found";
        break;
    case HandleWordStatus::GAME_STOPPED:
        qWarning() << "Game has been stopped";
        break;
    case HandleWordStatus::NO_FOUND_PLAYER:
        qWarning() << "Player not in this game";
        break;
    default:
        qWarning() << "Unknown word status:" << static_cast<int>(status);
    }
}

void GameScreen::onWordHandleError(const QString &error)
{
    qWarning() << "Error submitting word:" << error;
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
        AppState &appState = AppState::getInstance();
        m_sessionId = appState.getSessionId();
        m_gameId = data["gameId"].toULongLong();

        // Clear UI for new game
        if (m_wordsList)
        {
            m_wordsList->clear();
        }

        if (m_lastKanaLabel)
        {
            m_lastKanaLabel->setText("Last Kana: (connecting...)");
        }

        if (m_wordInput)
        {
            m_wordInput->clear();
            m_wordInput->setEnabled(false); // Disable until subscription succeeds
        }

        // Subscribe to game updates
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
