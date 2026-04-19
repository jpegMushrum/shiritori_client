#include "toastwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QPropertyAnimation>

ToastWidget::ToastWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    setupUI();

    // Create fade animations
    m_fadeInAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeInAnimation->setDuration(300);
    m_fadeInAnimation->setStartValue(0.0);
    m_fadeInAnimation->setEndValue(1.0);

    m_fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeOutAnimation->setDuration(300);
    m_fadeOutAnimation->setStartValue(1.0);
    m_fadeOutAnimation->setEndValue(0.0);

    connect(m_fadeOutAnimation, &QPropertyAnimation::finished, this, [this]()
            { hide(); });

    setWindowOpacity(0.0);
    hide();
}

void ToastWidget::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 15, 20, 15);

    m_messageLabel = new QLabel(this);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setStyleSheet("color: white; font-size: 14px;");
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setMaximumWidth(400);

    layout->addWidget(m_messageLabel);

    setLayout(layout);
}

void ToastWidget::showToast(const Toast &toast)
{
    m_currentType = toast.getType();
    m_messageLabel->setText(toast.getMessage());

    qDebug() << "Show toast:" << toast.getType() << toast.getMessage();

    // Set background color based on type
    QString bgColor;
    switch (toast.getType())
    {
    case Toast::NOTIFICATION:
        bgColor = "rgba(0, 100, 200, 200)";
        break;
    case Toast::WARNING:
        bgColor = "rgba(200, 150, 0, 200)";
        break;
    case Toast::ERROR:
        bgColor = "rgba(200, 0, 0, 200)";
        break;
    }

    setStyleSheet(QString("background-color: %1; border-radius: 8px;").arg(bgColor));

    // Show with fade-in animation (positioning will be done in showEvent)
    m_fadeOutAnimation->stop();
    setWindowOpacity(0.0);
    show();
    raise();
    m_fadeInAnimation->start();

    startAutoHide();
}

void ToastWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QWidget::paintEvent(event);
}

void ToastWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    adjustSize();

    if (parentWidget())
    {
        QWidget *parent = parentWidget();

        int x = parent->width() / 2 - width() / 2;
        int y = parent->height() - height() - 30;

        QPoint globalPos = parent->mapToGlobal(QPoint(x, y));
        move(globalPos);
    }
}

void ToastWidget::startAutoHide()
{
    // Auto-hide after 3 seconds
    QTimer::singleShot(3000, this, [this]()
                       { m_fadeOutAnimation->start(); });
}
