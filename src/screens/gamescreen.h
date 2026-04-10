#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>

class GameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);

private:
    // TODO: Add UI components
    // - Used words list
    // - Last kana display
    // - Word input field
    // - Submit button
    // - Exit button
};

#endif // GAMESCREEN_H
