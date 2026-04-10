#ifndef CONNECTIONSTATUSWIDGET_H
#define CONNECTIONSTATUSWIDGET_H

#include <QWidget>
#include <memory>

class QLabel;
class QPushButton;
class TcpClient;

class ConnectionStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionStatusWidget(QWidget *parent = nullptr);
    ~ConnectionStatusWidget() override;

    void setTcpClient(TcpClient *tcpClient);

private slots:
    void onConnected();
    void onDisconnected();
    void onReconnectClicked();
    void onConnectionError(const QString &errorMessage);

private:
    QIcon recolorIcon(const QString &path, const QColor &color);
    void setupUI();
    void updateStatus();

    QLabel *m_statusLabel = nullptr;
    QPushButton *m_reconnectButton = nullptr;
    TcpClient *m_tcpClient = nullptr;

};

#endif // CONNECTIONSTATUSWIDGET_H
