#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QWidget>
#include <QVariantMap>
#include "../utils/screennavigator.h"

class BaseScreen : public QWidget
{
    Q_OBJECT

public:
    explicit BaseScreen(QWidget *parent = nullptr);
    virtual ~BaseScreen() = default;

    void setNavigator(ScreenNavigator *navigator);

signals:
    void navigateToLoginScreen();
    void navigateToMainScreen();
    void navigateToSearchGameScreen();
    void navigateToGameScreen(const QVariantMap &gameData);
    void navigateToStatsScreen();
    void navigateBack();


protected slots:
    virtual void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) = 0;

protected:
    ScreenNavigator *navigator() const;
    void navigate(ScreenNavigator::ScreenType screen, const QVariantMap &data = {});
    void goBack();

private:
    ScreenNavigator *m_navigator = nullptr;
};

#endif // BASESCREEN_H
