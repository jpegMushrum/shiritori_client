#ifndef APISERVICE_H
#define APISERVICE_H

#include <QString>
#include <QObject>
#include <QList>
#include <QHash>
#include <memory>
#include <functional>
#include "../models/user.h"
#include "../models/game.h"
#include "serverprotocol.h"

class TcpClient;
class QTcpSocket;

class ApiService : public QObject
{
    Q_OBJECT

public:
    explicit ApiService(QObject *parent = nullptr);
    ~ApiService() override;

    // Set the main TCP client for API commands
    void setTcpClient(TcpClient *tcpClient);

    // ==================== Authentication ====================

    /// Login user. Returns session ID.
    /// On success: returns session ID and emits loginSuccess signal
    /// On error: returns empty string and emits loginError signal
    void loginAsync(const QString &username);

    /// Logout user
    void logoutAsync(const QString &sessionId);

    QString getLastError() const;
    void clearLastError();

signals:
    // Authentication signals
    void loginSuccess(const QString &sessionId);
    void loginError(const QString &error);

    void logoutSuccess();
    void logoutError(const QString &error);

    // User signals
    void userInfoReceived(const UserInfo &info);
    void userInfoError(const QString &error);

    // Game signals
    void activeGamesReceived(const QList<GameContext> &games);
    void activeGamesError(const QString &error);

    void gameInfoReceived(const GameContext &info);
    void gameInfoError(const QString &error);

    void gameStarted(const GameContext &game);
    void gameStartError(const QString &error);

    void gameStopped();
    void gameStopError(const QString &error);

    void playerAddedToGame();
    void playerAddError(const QString &error);

    void wordHandled(HandleWordStatus status);
    void wordHandleError(const QString &error);

    // Game subscription signals
    void gameSocketConnected();
    void gameSocketDisconnected();
    void newWordReceived(const NewWordUpdate &update);
    void gameUpdateReceived(const QString &update);

    // History signals
    void gameHistoryReceived(const QList<GameHistoryEntry> &history);
    void gameHistoryError(const QString &error);

private slots:
    void onGetResponse(QString response);

private:
    // Send command to main TCP client and get response
    void sendCommand(const QString &command);

    QHash<int, std::function<void(QString)>> m_pendingRequests;
    int m_nextRequestId = 1;

    void loginResponse(QString response);

    // Parse boolean responses like "Logged out successfully"
    bool isBooleanSuccess(const QString &response);

    TcpClient *m_tcpClient = nullptr;
    std::unique_ptr<QTcpSocket> m_gameSocket;
    QString m_lastError;
};

#endif // APISERVICE_H
