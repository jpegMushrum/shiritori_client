#include "notificationmanager.h"
#include "../utils/toastwidget.h"

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
{
}

void NotificationManager::setToastWidget(ToastWidget *toastWidget)
{
    m_toastWidget = toastWidget;
}

void NotificationManager::showToast(const Toast &toast)
{
    if (m_toastWidget)
    {
        m_toastWidget->showToast(toast);
    }
}

void NotificationManager::showToast(Toast::Type type, const QString &message)
{
    Toast toast(type, message);
    showToast(toast);
}