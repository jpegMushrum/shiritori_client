#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QString>
#include <QObject>
#include <memory>

class QTcpSocket;

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient() override;

    bool connectToHost(const QString &host, int port);
    void disconnect();
    bool isConnected() const;

    bool sendData(const QString &data);
    QString receiveData();

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &errorMessage);
    void dataReceived(const QString &data);

private slots:
    void onConnected();
    void onDisconnected();
    void onError();
    void onReadyRead();

private:
    std::unique_ptr<QTcpSocket> m_socket;
};

#endif // TCPCLIENT_H
