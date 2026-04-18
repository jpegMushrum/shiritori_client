#ifndef GAMEENDSCREEN_H
#define GAMEENDSCREEN_H

#include "basescreen.h"
#include "../services/serverprotocol.h"
#include <QList>

class GameEndScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit GameEndScreen(QWidget *parent = nullptr);
    void displayGameResults(const GameStoppedEvent &event);

private slots:
    void onMainMenuButtonClicked();
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap& data) override;

private:
    void setupUI();

    class QTableWidget *m_scoresTable = nullptr;
    GameStoppedEvent m_lastEvent;
};

#endif // GAMEENDSCREEN_H
