#include "basescreen.h"

BaseScreen::BaseScreen(QWidget *parent)
    : QWidget(parent)
{
}

void BaseScreen::setNavigator(ScreenNavigator *navigator)
{
    m_navigator = navigator;
    connect(m_navigator, &ScreenNavigator::screenChanged, this, &BaseScreen::onOpen);
}

ScreenNavigator *BaseScreen::navigator() const
{
    return m_navigator;
}

void BaseScreen::navigate(ScreenNavigator::ScreenType screen, const QVariantMap &data)
{
    if (m_navigator)
    {
        m_navigator->navigateTo(screen, data);
    }
}

void BaseScreen::goBack()
{
    if (m_navigator)
    {
        m_navigator->goBack();
    }
}
