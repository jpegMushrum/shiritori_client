#ifndef GAMEUPDATESERVICE_H
#define GAMEUPDATESERVICE_H

#include <QObject>

#include "apiservice.h"
#include "serverprotocol.h"

class GameUpdateService : public QObject
{
    Q_OBJECT
public:
    explicit GameUpdateService(QObject *parent = nullptr);

    void setTcpClient(TcpClient* tcpClient);
    void subscribeOnGameAsync(qulonglong gameId);

signals:
    void newWordRecieved(const NewWordUpdate& update);
    void newWordError(const QString& error);

    void subscribeError(const QString& error);
    void subscribeSuccess();
private slots:

    void onLoginSuccess();
    void onRegisterSuccess();

    void onDataRecieved(const QString& data);

private:
    TcpClient *m_tcpClient = nullptr;
    QString m_sessionId = "";

    void subResponse(const QString& response);
    void regResponse(const QString& response);

    bool checkSubscribeResponse(const QString& response);

};

#endif // GAMEUPDATESERVICE_H
