#include "toast.h"

Toast::Toast(Type type, const QString& message): m_message(message), m_type(type)
{
}

QString Toast::getMessage() const {
    return m_message;
}

Toast::Type Toast::getType() const {
    return m_type;
}