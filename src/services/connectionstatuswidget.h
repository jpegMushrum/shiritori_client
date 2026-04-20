#ifndef CONNECTIONSTATUSWIDGET_H
#define CONNECTIONSTATUSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "apiservice.h"

class ConnectionStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionStatusWidget(TcpClient *tcpClient, ApiService *apiService, QWidget *parent = nullptr);
    ~ConnectionStatusWidget() override;

private slots:
    void onConnected();
    void onDisconnected();
    void onReconnectClicked();
    void onConnectionError(const QString &errorMessage);

private:
    void setupUI();
    void updateStatus();

    bool m_loginRequested = false;
    QLabel *m_statusLabel = nullptr;
    QPushButton *m_reconnectButton = nullptr;
    TcpClient *m_tcpClient = nullptr;
    ApiService *m_apiService = nullptr;
};

#endif // CONNECTIONSTATUSWIDGET_H
