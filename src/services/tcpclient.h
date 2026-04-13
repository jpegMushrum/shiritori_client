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

    void connectToHost(const QString &host, int port);
    void disconnect();
    bool isConnected() const;

    bool sendData(const QString &data);
    QString receiveData();

    QString getHost() const;
    int getPort() const;

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
    QString m_host;
    int m_port = 0;
};

#endif // TCPCLIENT_H
