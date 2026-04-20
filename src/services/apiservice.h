#ifndef APISERVICE_H
#define APISERVICE_H

#include <QString>
#include <QObject>
#include <QList>
#include <QHash>
#include <functional>
#include "tcpclient.h"
#include "serverprotocol.h"

class TcpClient;
class QTcpSocket;

class ApiService : public QObject
{
    Q_OBJECT

public:
    explicit ApiService(TcpClient* tcpClient, QObject *parent = nullptr);
    ~ApiService() override;

    // ==================== Authentication ====================

    void loginAsync(const QString &username);

    void logoutAsync(const QString &sessionId);

    void getUserInfoAsync(const QString &sessionId);

    void getActiveGamesAsync();

    // Game management
    void startNewGameAsync(const QString &sessionId);

    void getGameInfoAsync(qulonglong gameId);

    // Game subscription
    void subscribeOnGameAsync(qulonglong gameId);

    // Game actions
    void handleWordAsync(qulonglong gameId, const QString &sessionId, const QString &word);

    QString getLastError() const;
    void clearLastError();

signals:
    // Authentication signals
    void loginSuccess(const QString &sessionId);
    void loginError(const QString &error);

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

    void playerJoinedGame(const PlayerJoinedGameInfo &info);

    void wordHandled(HandleWordStatus status);
    void wordHandleError(const QString &error);

    // Game subscription signals
    void gameSocketConnected();
    void gameSocketDisconnected();
    void newWordReceived(const NewWordUpdate &update);
    void gameUpdateReceived(const QString &update);
    void gameStopped(const GameStoppedEvent &event);

    // Subscription signals
    void subscribeSuccess();
    void subscribeError(const QString &error);

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

    void loginResponse(const QString &response);
    void getUserInfoResponse(const QString &response);
    void getActiveGamesResponse(const QString &response);
    void startNewGameResponse(const QString &response);
    void getGameInfoResponse(const QString &response);
    void subscribeResponse(const QString &response, int requestId);
    void newWordResponse(const QString &response, int requestId);
    void handleWordResponse(const QString &response);

    // Parse boolean responses like "Logged out successfully"
    bool isBooleanSuccess(const QString &response);

    TcpClient *m_tcpClient = nullptr;
    QString m_lastError;
    QString m_sessionId; // Store session ID for subscriptions
};

#endif // APISERVICE_H
