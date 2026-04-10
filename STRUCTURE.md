# Project Structure

## Директория структура Shiritori Client

```
shiritori-client/
├── src/
│   ├── application.h/cpp          # Главное окно приложения (QStackedWidget)
│   │
│   ├── screens/                   # Все экраны приложения
│   │   ├── loginscreen.h/cpp      # 1. Login screen (username, start button)
│   │   ├── mainscreen.h/cpp       # 2. Main screen (start game, search, stats, profile)
│   │   ├── searchgamescreen.h/cpp # 3. Search game screen (list, filter, sort, join)
│   │   ├── gamescreen.h/cpp       # 4. Game screen (used words, last kana, new word input)
│   │   └── statsscreen.h/cpp      # 5. Stats screen (user info, achievements)
│   │
│   ├── models/                    # Модели данных
│   │   ├── user.h/cpp            # User (username, wins, losses)
│   │   └── game.h/cpp            # Game (name, players, used words, last kana)
│   │
│   ├── services/                  # Бизнес логика и API
│   │   ├── apiservice.h/cpp      # Работа с API (login, create game, join, submit word)
│   │   └── gamelogic.h/cpp       # Логика игры (валидация слов, кана)
│   │
│   └── utils/                     # Вспомогательные файлы
│       ├── constants.h           # Константы (API URL, размеры окна, настройки)
│       └── appstate.h/cpp        # Глобальное состояние приложения (текущий пользователь)
│
├── resources/
│   └── resources.qrc             # Qt ресурсы (иконки, стили, изображения)
│
├── main.cpp                       # Точка входа в приложение
├── CMakeLists.txt                 # Конфигурация CMake (обновлен)
└── README.md                      # Описание проекта
```

## Основные компоненты

### Экраны (Screens)
- **LoginScreen**: Вход - поле для имени пользователя и кнопка старта
- **MainScreen**: Главный экран - кнопки для начала игры, поиска, статистики
- **SearchGameScreen**: Поиск игр - список, сортировка, фильтр, присоединение
- **GameScreen**: Игровой экран - список слов, последняя кана, ввод нового слова
- **StatsScreen**: Статистика - информация о пользователе

### Модели (Models)
- **User**: Пользователь с именем, количеством побед/поражений
- **Game**: Игра с названием, количеством игроков, списком слов

### Сервисы (Services)
- **ApiService**: Работа с бэенд API (регистрация, логин, управление играми)
- **GameLogic**: Валидация слов и кана, логика игры

### Утилиты (Utils)
- **Constants**: Глобальные константы
- **AppState**: Singleton для хранения текущего состояния приложения

## Навигация между экранами

Приложение использует `QStackedWidget` для навигации между экранами:

```
LoginScreen
    ↓ (после входа)
MainScreen
    ├→ SearchGameScreen (поиск игры)
    │     ↓ (присоединение)
    │     GameScreen
    │
    └→ StatsScreen (статистика)
```

## Следующие шаги

1. **Реализовать UI для каждого экрана** - добавить кнопки, поля ввода, списки
2. **Подключить API** - реализовать HTTP запросы в ApiService
3. **Добавить сигналы/слоты** - навигация между экранами и обновление состояния
4. **Реализовать игровую логику** - валидация слов, проверка каны
5. **Добавить стили/оформление** - CSS-стили через QSS файлы в resources/

## TODO в коде

Все места, где нужно реализовать логику, отмечены комментарием `// TODO:`
