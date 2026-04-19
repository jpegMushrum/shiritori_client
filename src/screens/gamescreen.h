#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "basescreen.h"
#include "../services/apiservice.h"
#include "../services/notificationmanager.h"
#include <QMap>
#include <QKeyEvent>

class GameScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit GameScreen(ApiService *apiService, NotificationManager *notificationManager,
                        QWidget *parent = nullptr);

    void setGameEndScreen(class GameEndScreen *gameEndScreen);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void onSubmitButtonClicked();
    void onExitButtonClicked();
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) override;
    void onNewWordReceived(const NewWordUpdate &update);
    void onSubscribeError(const QString &error);
    void onSubscribeSuccess();
    void onPlayerJoinedGame(const PlayerJoinedGameInfo &info);
    void onGameStopped(const GameStoppedEvent &event);

    void onWordHandled(HandleWordStatus status);
    void onWordHandleError(const QString &error);
    void onWordListItemClicked(int row);

private:
    void setupUI();
    void connectSignals();
    void showToast(Toast::Type type, const QString &message);

    ApiService *m_apiService = nullptr;
    class GameEndScreen *m_gameEndScreen = nullptr;
    class NotificationManager *m_notificationManager = nullptr;
    qulonglong m_gameId = -1;
    QString m_sessionId;

    // UI components
    class QLineEdit *m_wordInput = nullptr;
    class QListWidget *m_wordsList = nullptr;
    class QLabel *m_lastKanaLabel = nullptr;

    // Word data storage for popup info
    struct WordData
    {
        QStringList readings;
        QString translation;
        QStringList partOfSpeech;
    };
    QMap<QString, WordData> m_wordDataMap;

    // TODO: Add UI components
    // - Used words list
    // - Last kana display
    // - Word input field
    // - Submit button
    // - Exit button
};

#endif // GAMESCREEN_H
