#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QWidget>

class MainScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MainScreen(QWidget *parent = nullptr);

private:
    // TODO: Add UI components
    // - Start new game button
    // - Search for existing game button
    // - Log in / Register button
    // - View stats button
};

#endif // MAINSCREEN_H
