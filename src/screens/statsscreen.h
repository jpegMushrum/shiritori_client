#ifndef STATSSCREEN_H
#define STATSSCREEN_H

#include "basescreen.h"

class StatsScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit StatsScreen(QWidget *parent = nullptr);

private slots:
    void onBackButtonClicked();

private:
    void setupUI();

    // TODO: Add UI components
    // - User info display
    // - Statistics display
    // - Back button
};

#endif // STATSSCREEN_H
