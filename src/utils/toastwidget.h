#ifndef TOASTWIDGET_H
#define TOASTWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include "toast.h"

class ToastWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToastWidget(QWidget *parent = nullptr);

public slots:
    void showToast(const Toast &toast);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void setupUI();
    void startAutoHide();

    class QLabel *m_messageLabel = nullptr;
    QPropertyAnimation *m_fadeInAnimation = nullptr;
    QPropertyAnimation *m_fadeOutAnimation = nullptr;
    Toast::Type m_currentType = Toast::NOTIFICATION;
};

#endif // TOASTWIDGET_H
