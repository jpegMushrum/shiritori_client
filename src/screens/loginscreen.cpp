#include "loginscreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

LoginScreen::LoginScreen(QWidget *parent)
    : BaseScreen(parent)
{
    setupUI();
}

void LoginScreen::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("Shiritori Game", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(titleLabel);

    layout->addSpacing(20);

    auto *usernameLabel = new QLabel("Username:", this);
    layout->addWidget(usernameLabel);

    auto *usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your username");
    layout->addWidget(usernameInput);

    layout->addSpacing(20);

    auto *startButton = new QPushButton("Start Game", this);
    startButton->setMinimumHeight(40);
    connect(startButton, &QPushButton::clicked, this, &LoginScreen::onStartButtonClicked);
    layout->addWidget(startButton);

    layout->addStretch();
}

void LoginScreen::onStartButtonClicked()
{
    navigate(ScreenNavigator::MainScreen);
}
