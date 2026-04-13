#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "basescreen.h"

class ApiService;
struct GameContext;

class MainScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit MainScreen(QWidget *parent = nullptr);

private slots:
    void onNewGameButtonClicked();
    void onSearchGameButtonClicked();
    void onStatsButtonClicked();
    void onLogoutButtonClicked();
    void onGameStarted(const GameContext &game);
    void onGameStartError(const QString &error);
    void onOpen(ScreenNavigator::ScreenType screen) override;

private:
    void setupUI();
    void startNewGame();

    ApiService *m_apiService = nullptr;
};

#endif // MAINSCREEN_H
