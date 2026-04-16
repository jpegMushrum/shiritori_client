#ifndef SEARCHGAMESCREEN_H
#define SEARCHGAMESCREEN_H

#include <QLineEdit>
#include <QTableWidget>

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
    void onTableCellClicked(int row, int column);
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) override;

private:
    void setupUI();
    void loadGames();
    void displayGames(const QList<GameContext> &games);

    QLineEdit *m_searchInput = nullptr;
    QTableWidget *m_gamesTable = nullptr;
    ApiService *m_apiService = nullptr;
    qulonglong m_selectedGameId = -1;
};

#endif // SEARCHGAMESCREEN_H
