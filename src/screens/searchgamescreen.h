#ifndef SEARCHGAMESCREEN_H
#define SEARCHGAMESCREEN_H

#include <QLineEdit>
#include <QListWidget>

#include "basescreen.h"
#include "../services/serverprotocol.h"
#include "../services/apiservice.h"

class SearchGameScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit SearchGameScreen(QWidget *parent = nullptr);

private slots:
    void onJoinGameButtonClicked();
    void onBackButtonClicked();
    void onRefreshButtonClicked();
    void onActiveGamesReceived(const QList<GameContext> &games);
    void onActiveGamesError(const QString &error);

private:
    void setupUI();
    void loadGames();
    void displayGames(const QList<GameContext> &games);

    QLineEdit *m_searchInput = nullptr;
    QListWidget *m_gamesList = nullptr;
    ApiService *m_apiService = nullptr;
    QList<GameContext> m_availableGames;
};

#endif // SEARCHGAMESCREEN_H
