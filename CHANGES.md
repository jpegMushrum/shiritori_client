# Изменения для Shiritori Client

## 📋 Обзор добавленных функций

1. **TCP подключение к серверу** - класс `TcpClient` для надежного TCP соединения
2. **Панель статуса подключения** - виджет `ConnectionStatusWidget` в нижнем правом углу
3. **Поле ввода адреса сервера** - на экране логина с портом
4. **Кнопка Log In** - вместо "Start Game"
5. **SessionId** - хранение sessionId после успешного логина
6. **Аутентификация через UUID** - предотвращение несанкционированных запросов

---

## 🔧 Новые файлы

### `src/services/tcpclient.h` / `tcpclient.cpp`
TCP клиент для подключения к серверу по адресу и порту.

**Основные методы:**
- `connect(host, port)` - подключиться к серверу
- `disconnect()` - отключиться
- `isConnected()` - проверить соединение
- `sendData(data)` - отправить данные
- `receiveData()` - получить данные

**Сигналы:**
- `connected()` - успешное подключение
- `disconnected()` - отключение
- `connectionError(message)` - ошибка подключения
- `dataReceived(data)` - получены данные

---

### `src/services/connectionstatuswidget.h` / `connectionstatuswidget.cpp`
Виджет для показа статуса подключения в нижнем правом углу.

**Функции:**
- Показывает статус: "Server: Connected" (зеленый) или "Server: Disconnected" (красный)
- Кнопка переподключения с иконкой
- Автоматическое обновление статуса при изменении соединения

---

## 📝 Обновленные файлы

### `src/models/user.h` / `user.cpp`
**Добавлено:**
- Поле `m_sessionId`
- Методы: `getSessionId()`, `setSessionId()`

---

### `src/utils/appstate.h` / `appstate.cpp`
**Добавлено:**
- `m_serverAddress` - адрес сервера
- `m_serverPort` - порт сервера (по умолчанию 5000)
- `m_sessionId` - ID сессии после логина
- `m_username` - имя пользователя

**Новые методы:**
- `getServerAddress()` / `setServerAddress()`
- `getServerPort()` / `setServerPort()`
- `getSessionId()` / `setSessionId()`
- `getUsername()` / `setUsername()`

---

### `src/screens/loginscreen.h` / `loginscreen.cpp`
**Изменено:**
- Добавлены поля ввода: адрес сервера и порт
- Кнопка "Start Game" → "Log In"
- TCP подключение при логине
- Валидация ввода
- Вывод ошибок

---

### `src/services/apiservice.h` / `apiservice.cpp`
**Добавлено:**
- Интеграция с `TcpClient`
- Методы для работы с sessionId
- JSON сериализация запросов
- Метод `login()` теперь возвращает sessionId
- Все методы проверяют sessionId перед отправкой

---

### `src/application.h` / `application.cpp`
**Добавлено:**
- Глобальный `TcpClient` для всего приложения
- Виджет `ConnectionStatusWidget` в нижнем правом углу
- Правильная компоновка главного окна

---

### `CMakeLists.txt`
**Добавлено:**
- Qt компоненты: `Network`, `Core`
- Новые файлы: `tcpclient.h/cpp`, `connectionstatuswidget.h/cpp`
- Линковка с `Qt::Network` и `Qt::Core`

---

## 🚀 Как использовать

### При логине
1. Пользователь вводит адрес сервера (по умолчанию `localhost`)
2. Указывает порт (по умолчанию `5000`)
3. Вводит username
4. Нажимает "Log In"
5. Клиент подключается по TCP и отправляет запрос логина в JSON формате:
```json
{
  "action": "login",
  "username": "player1"
}
```
6. Сервер должен ответить:
```json
{
  "success": true,
  "sessionId": "uuid-string-here"
}
```
7. sessionId сохраняется в `AppState` и используется для всех последующих запросов

### При отправке запросов
Все запросы теперь включают `sessionId`:
```json
{
  "action": "submitWord",
  "sessionId": "uuid-string",
  "gameId": "game123",
  "word": "hello"
}
```

### Виджет статуса подключения
- **Зеленый "Server: Connected"** - соединение активно
- **Красный "Server: Disconnected"** - соединение потеряно
- **Кнопка переподключения** - попытка переподключиться

---

## ⚠️ Важные замечания

1. **Статус виджет** - располагается в нижнем правом углу главного окна, всегда видим
2. **SessionId проверка** - все методы API проверяют наличие sessionId и соединения
3. **Ошибки** - выводятся в красный label на экране логина
4. **TCP протокол** - используется JSON для передачи данных

---

## 🔒 Безопасность

- sessionId предотвращает отправку запросов от имени другого пользователя
- sessionId уникален и генерируется сервером при логине
- sessionId хранится в памяти приложения
- При logout sessionId очищается

---

## 🏗️ Архитектура

```
Application (главное окно)
├── TcpClient (глобальный, управляет TCP соединением)
├── ConnectionStatusWidget (нижний правый угол, показывает статус)
├── LoginScreen
│   ├── Input: Server Address
│   ├── Input: Server Port
│   └── Input: Username
├── MainScreen, GameScreen, etc.
└── AppState
    ├── sessionId
    ├── username
    └── serverAddress, serverPort
```

---

## 📦 Компоненты Qt

Добавлены:
- `Network` - для TCP сокетов
- `Core` - для JSON обработки

---

