#ifndef SCREENNAVIGATOR_H
#define SCREENNAVIGATOR_H

#include <QObject>
#include <QStackedWidget>

class ScreenNavigator : public QObject
{
    Q_OBJECT

public:
    enum ScreenType
    {
        LoginScreen = 0,
        MainScreen = 1,
        SearchGameScreen = 2,
        GameScreen = 3,
        StatsScreen = 4
    };
    Q_ENUM(ScreenType)

    explicit ScreenNavigator(QStackedWidget *stackedWidget, QObject *parent = nullptr);

    void navigateTo(ScreenType screen, const QVariantMap &data = {});
    void goBack();
    ScreenType currentScreen() const;

    void setPreviousScreen(ScreenType screen);

signals:
    void screenChanged(ScreenType newScreen);
    void navigationRequested(ScreenType screen, const QVariantMap &data);

private:
    void updateHistory(ScreenType screen);

    QStackedWidget *m_stackedWidget = nullptr;
    ScreenType m_currentScreen = LoginScreen;
    ScreenType m_previousScreen = LoginScreen;
};

#endif // SCREENNAVIGATOR_H
