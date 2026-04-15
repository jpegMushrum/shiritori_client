#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "basescreen.h"
#include "../services/apiservice.h"

class GameScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);

private slots:
    void onSubmitButtonClicked();
    void onExitButtonClicked();
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) override;
    void onNewWordReceived(const NewWordUpdate &update);
    void onSubscribeError(const QString &error);
    void onSubscribeSuccess();

private:
    void setupUI();
    void connectSignals();

    ApiService *m_apiService = nullptr;
    qulonglong m_gameId = -1;

    // TODO: Add UI components
    // - Used words list
    // - Last kana display
    // - Word input field
    // - Submit button
    // - Exit button
};

#endif // GAMESCREEN_H
