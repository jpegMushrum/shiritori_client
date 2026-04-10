#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "basescreen.h"

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

private:
    void setupUI();

    // TODO: Add UI components
    // - Start new game button
    // - Search for existing game button
    // - View stats button
    // - Logout button
};

#endif // MAINSCREEN_H
