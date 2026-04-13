#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "basescreen.h"

class GameScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);

private slots:
    void onSubmitButtonClicked();
    void onExitButtonClicked();
    void onOpen(ScreenNavigator::ScreenType screen) override;

private:
    void setupUI();

    // TODO: Add UI components
    // - Used words list
    // - Last kana display
    // - Word input field
    // - Submit button
    // - Exit button
};

#endif // GAMESCREEN_H
