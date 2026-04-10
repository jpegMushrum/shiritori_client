#ifndef SEARCHGAMESCREEN_H
#define SEARCHGAMESCREEN_H

#include "basescreen.h"

class SearchGameScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit SearchGameScreen(QWidget *parent = nullptr);

private slots:
    void onJoinGameButtonClicked();
    void onBackButtonClicked();
    void onRefreshButtonClicked();

private:
    void setupUI();

    // TODO: Add UI components
    // - Search field / filter by name
    // - Sort buttons (by players, by used words count)
    // - Games list
    // - Refresh button
    // - Join game button
    // - Back button
};

#endif // SEARCHGAMESCREEN_H
