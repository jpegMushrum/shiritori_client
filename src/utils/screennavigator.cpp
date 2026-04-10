#include "screennavigator.h"

ScreenNavigator::ScreenNavigator(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent), m_stackedWidget(stackedWidget), m_currentScreen(LoginScreen), m_previousScreen(LoginScreen)
{
}

void ScreenNavigator::navigateTo(ScreenType screen, const QVariantMap &data)
{
    if (!m_stackedWidget)
        return;

    if (m_currentScreen != screen)
    {
        m_previousScreen = m_currentScreen;
        m_currentScreen = screen;
        m_stackedWidget->setCurrentIndex(static_cast<int>(screen));
        emit screenChanged(screen);
    }

    emit navigationRequested(screen, data);
}

void ScreenNavigator::goBack()
{
    navigateTo(m_previousScreen);
}

ScreenNavigator::ScreenType ScreenNavigator::currentScreen() const
{
    return m_currentScreen;
}

void ScreenNavigator::setPreviousScreen(ScreenType screen)
{
    m_previousScreen = screen;
}
