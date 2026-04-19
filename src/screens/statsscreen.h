#ifndef STATSSCREEN_H
#define STATSSCREEN_H

#include "basescreen.h"

class QLabel;
class ApiService;
class NotificationManager;
struct UserInfo;

class StatsScreen : public BaseScreen
{
    Q_OBJECT

public:
    explicit StatsScreen(ApiService *apiService, NotificationManager *notificationManager,
                         QWidget *parent = nullptr);

private slots:
    void onBackButtonClicked();
    void onUserInfoReceived(const UserInfo &info);
    void onUserInfoError(const QString &error);
    void onOpen(ScreenNavigator::ScreenType screen, const QVariantMap &data = {}) override;

private:
    void setupUI();
    void loadUserStats();
    void displayUserStats(const UserInfo &info);

    QLabel *m_usernameLabel = nullptr;
    QLabel *m_averageWordsLabel = nullptr;
    QLabel *m_errorLabel = nullptr;

    ApiService *m_apiService = nullptr;
    class NotificationManager *m_notificationManager = nullptr;
};

#endif // STATSSCREEN_H
