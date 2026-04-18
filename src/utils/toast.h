#ifndef TOAST_H
#define TOAST_H

#include <QString>

class Toast
{
public:
    enum Type {
        NOTIFICATION,
        WARNING,
        ERROR
    };

    Toast(Type type, const QString& message);


    QString getMessage() const;
    Type getType() const;

private:
    QString m_message;
    Type m_type;
};

#endif // TOAST_H
