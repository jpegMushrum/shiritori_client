#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User() = default;
    explicit User(const QString &username);

    // Username and session
    QString getUsername() const;
    void setUsername(const QString &username);

    QString getSessionId() const;
    void setSessionId(const QString &sessionId);

    // User ID from server
    qulonglong getUserId() const;
    void setUserId(qulonglong userId);

    // Stats
    int getWins() const;
    void setWins(int wins);

    int getLosses() const;
    void setLosses(int losses);

    double getAverageWordsPerGame() const;
    void setAverageWordsPerGame(double average);

private:
    QString m_username;
    QString m_sessionId;
    qulonglong m_userId = 0;
    int m_wins = 0;
    int m_losses = 0;
    double m_averageWordsPerGame = 0.0;
};

#endif // USER_H
