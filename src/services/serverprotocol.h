#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include <QString>
#include <QList>
#include <optional>

// Handle Word Response Status Codes
enum class HandleWordStatus
{
    OK,
    GOT_ERROR,
    WRONG_ORDER,
    NOT_JAPANESE_WORD,
    NO_SPEACH_PART,
    NO_FOUND_WORD,
    GOT_END_WORD,
    GOT_DOUBLED_WORD,
    CANT_JOIN_WORDS,
    GAME_NOT_FOUND,
    GAME_STOPPED,
    NO_FOUND_PLAYER,
    UNKNOWN
};

// User info from getUserInfo
struct UserInfo
{
    qulonglong userId;
    QString nickname;
    double averageWordsPerGame;
};

// Game context from getActiveGames, getGameInfo, startNewGame
struct GameContext
{
    qulonglong gameId;
    int wordsCount;
    int playersCount;
    qulonglong adminId;
    qulonglong lastPlayerId;
    QString lastKana;
};

// Game history entry from getGamesHistory
struct GameHistoryEntry
{
    qulonglong gameId;
    qulonglong userId;
    int wordsCount;
    int place;
};

enum class GameEventType {
    WordPlayed,
    GameStopped,
    Unknown
};

struct GameEvent {
    GameEventType type;
    qulonglong gameId;
};

// New word update from subscription or game updates
struct NewWordUpdate
{
    qulonglong gameId;
    QString kanji;
    QList<QString> meanings;
    QList<QString> partsOfSpeech;
    QList<QString> readings;
};

// Score entry from game ended
struct PlayerScore
{
    qulonglong userId;
    int score;
};

struct WordPlayedEvent : GameEvent {
    NewWordUpdate word;
    QString lastKana;
};

struct GameStoppedEvent : GameEvent {
    QList<PlayerScore> scores;
};

// Player joined game response
struct PlayerJoinedGameInfo
{
    qulonglong gameId;
    QString lastKana;
    QList<NewWordUpdate> usedWords;
};

// Protocol parsing utilities
class ServerProtocolParser
{
public:
    // Parse UserInfo from "userId nickname averageWordsPerGame"
    static std::optional<UserInfo> parseUserInfo(const QString &response);

    // Parse GameContext from "gameId wordsCount playersCount adminId lastPlayerId lastKana"
    static std::optional<GameContext> parseGameContext(const QString &response);

    // Parse multiple GameContext separated by semicolons
    static QList<GameContext> parseMultipleGameContexts(const QString &response);

    // Parse GameHistoryEntry from "gameId userId wordsCount place"
    static std::optional<GameHistoryEntry> parseGameHistoryEntry(const QString &response);

    // Parse multiple game history entries separated by semicolons
    static QList<GameHistoryEntry> parseGameHistory(const QString &response);

    // Parse HandleWordStatus from response
    static HandleWordStatus parseHandleWordStatus(const QString &response);

    // Check if response is an error
    static bool isError(const QString &response);

    // Extract error message
    static QString extractErrorMessage(const QString &response);

    // Parse updates from JSON-like format
    static std::optional<std::variant<WordPlayedEvent, GameStoppedEvent>> parseGameUpdate(const QString& response);

    // Parse PlayerJoinedGameInfo from "playerJoinedGame <gameId> {JSON}"
    static std::optional<PlayerJoinedGameInfo> parsePlayerJoinedGameInfo(const QString &response);
};

#endif // SERVERPROTOCOL_H
