#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include "../utils/toast.h"

class ToastWidget;

class NotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationManager(QObject *parent = nullptr);
    void setToastWidget(ToastWidget *toastWidget);

public slots:
    void showToast(const Toast &toast);
    void showToast(Toast::Type type, const QString &message);

private:
    ToastWidget *m_toastWidget = nullptr;
};

#endif // NOTIFICATIONMANAGER_H
