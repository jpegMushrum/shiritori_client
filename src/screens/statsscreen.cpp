#include "statsscreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "../services/serverprotocol.h"
#include "../services/apiservice.h"
#include "../utils/appstate.h"

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

    m_usernameLabel = new QLabel("Username: Loading...", this);
    m_usernameLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(m_usernameLabel);

    m_averageWordsLabel = new QLabel("Average Words Per Game: Loading...", this);
    m_averageWordsLabel->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(m_averageWordsLabel);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red; font-size: 14px;");
    m_errorLabel->setWordWrap(true);
    mainLayout->addWidget(m_errorLabel);

    mainLayout->addSpacing(30);

    auto *backButton = new QPushButton("Back", this);
    backButton->setMinimumHeight(40);
    connect(backButton, &QPushButton::clicked, this, &StatsScreen::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    mainLayout->addStretch();

    // Setup API service
    m_apiService = new ApiService(this);
    connect(m_apiService, &ApiService::userInfoReceived, this, &StatsScreen::onUserInfoReceived);
    connect(m_apiService, &ApiService::userInfoError, this, &StatsScreen::onUserInfoError);
}

void StatsScreen::onOpen(ScreenNavigator::ScreenType screen) {
    if (screen == ScreenNavigator::StatsScreen) {
        loadUserStats();
    }
}

void StatsScreen::loadUserStats()
{
    AppState &appState = AppState::getInstance();
    if (!appState.isLoggedIn()) {
        qDebug() << "Stats Screen Not logged in";
        m_errorLabel->setText("Not logged in");
        return;
    }

    m_errorLabel->clear();
    // m_apiService->getUserInfoAsync(appState.getSessionId());
}

void StatsScreen::displayUserStats(const UserInfo &info)
{
    m_usernameLabel->setText(QString("Username: %1").arg(info.nickname));
    m_averageWordsLabel->setText(QString("Average Words Per Game: %1").arg(info.averageWordsPerGame, 0, 'f', 1));
    m_errorLabel->clear();
}

void StatsScreen::onBackButtonClicked()
{
    goBack();
}

void StatsScreen::onUserInfoReceived(const UserInfo &info)
{
    qDebug() << "User info received:" << info.nickname;
    displayUserStats(info);
}

void StatsScreen::onUserInfoError(const QString &error)
{
    qDebug() << "User info error:" << error;
    m_errorLabel->setText("Error: " + error);
}
