#include "gameupdateservice.h"
#include "tcpclient.h"
#include "../utils/appstate.h"
#include <QDebug>

GameUpdateService::GameUpdateService(QObject *parent)
    : QObject{parent}
{
}

void GameUpdateService::setTcpClient(TcpClient* tcpClient) {
    if (m_tcpClient) {
        disconnect(m_tcpClient, &TcpClient::dataReceived, this, &GameUpdateService::onDataRecieved);
    }

    m_tcpClient = tcpClient;
    if (m_tcpClient) {
        connect(m_tcpClient, &TcpClient::dataReceived, this, &GameUpdateService::onDataRecieved);
    }
    qDebug() << "GameUpdateService set TcpClient" << tcpClient;
}

void GameUpdateService::subscribeOnGameAsync(qulonglong gameId) {
    if (!m_tcpClient) {
        emit subscribeError("Tcp Error: TCP client not set");
        return;
    }

    AppState &appState = AppState::getInstance();

    QString command = QString("reg login %1").arg(appState.getUsername());

    QString command = QString("sub addPlayerToGame %1 %2").arg(m_sessionId).arg(gameId);
    qDebug() << "addPlayerToGame command:" << command;

    if (!m_tcpClient) {
        emit subscribeError("Tcp Error: TCP client not set");
        return;
    }

    if (!m_tcpClient->sendData(command + "\n")) {
        emit subscribeError("Tcp Client Error: Failed to send command");
        return;
    }
}

void GameUpdateService::subResponse(const QString& response) {
    qDebug() << "addPlayerToGame response:" << response;
    if (response.endsWith("successfully")) {
        emit subscribeSuccess();
    } else {
        emit subscribeError(response);
    }
    return;
}

void GameUpdateService::regResponse(const QString& response) {
    qDebug() << "login response:" << response;
    if (response.startsWith("Error:")) {
        emit subscribeError(response);
    }
}

void GameUpdateService::onDataRecieved(const QString& data) {
    if (data.startsWith("sub")) {
        subResponse(data.sliced(3));
        return;
    }

    if (data.startsWith("reg")) {
        regResponse(data.sliced(3));
        return;
    }

    std::optional<NewWordUpdate> newWord = ServerProtocolParser::parseNewWordUpdate(data);
    if (!newWord.has_value()) {
        emit newWordError(data);
        return;
    }

    qDebug() << "New Word" << newWord->kanji;
    emit newWordRecieved(*newWord);
}