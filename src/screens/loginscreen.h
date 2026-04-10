#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "basescreen.h"

class LoginScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);

private slots:
    void onStartButtonClicked();

private:
    void setupUI();

    // TODO: Add UI components
    // - Username input
    // - Start button
};

#endif // LOGINSCREEN_H
