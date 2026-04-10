#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User() = default;
    explicit User(const QString &username);

    QString getUsername() const;
    void setUsername(const QString &username);

    int getWins() const;
    void setWins(int wins);

    int getLosses() const;
    void setLosses(int losses);

private:
    QString m_username;
    int m_wins = 0;
    int m_losses = 0;
};

#endif // USER_H
