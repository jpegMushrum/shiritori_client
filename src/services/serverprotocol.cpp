#include "serverprotocol.h"

std::optional<UserInfo> ServerProtocolParser::parseUserInfo(const QString &response)
{
    if (isError(response)) {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 3) {
        return std::nullopt;
    }

    UserInfo info;
    bool ok1, ok2;
    info.userId = parts[0].toULongLong(&ok1);
    info.nickname = parts[1];
    info.averageWordsPerGame = parts[2].toDouble(&ok2);

    if (!ok1 || !ok2) {
        return std::nullopt;
    }

    return info;
}

std::optional<GameContext> ServerProtocolParser::parseGameContext(const QString &response)
{
    if (isError(response)) {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 6) {
        return std::nullopt;
    }

    GameContext context;
    bool ok1, ok2, ok3, ok4, ok5;
    context.gameId = parts[0].toULongLong(&ok1);
    context.wordsCount = parts[1].toInt(&ok2);
    context.playersCount = parts[2].toInt(&ok3);
    context.adminId = parts[3].toULongLong(&ok4);
    context.lastPlayerId = parts[4].toULongLong(&ok5);
    context.lastKana = parts[5];

    if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5) {
        return std::nullopt;
    }

    return context;
}

QList<GameContext> ServerProtocolParser::parseMultipleGameContexts(const QString &response)
{
    QList<GameContext> contexts;

    if (response.isEmpty() || isError(response)) {
        return contexts;
    }

    QStringList games = response.split(';');
    for (const QString &game : games) {
        auto context = parseGameContext(game.trimmed());
        if (context) {
            contexts.append(context.value());
        }
    }

    return contexts;
}

std::optional<GameHistoryEntry> ServerProtocolParser::parseGameHistoryEntry(const QString &response)
{
    if (isError(response)) {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 4) {
        return std::nullopt;
    }

    GameHistoryEntry entry;
    bool ok1, ok2, ok3, ok4;
    entry.gameId = parts[0].toULongLong(&ok1);
    entry.userId = parts[1].toULongLong(&ok2);
    entry.wordsCount = parts[2].toInt(&ok3);
    entry.place = parts[3].toInt(&ok4);

    if (!ok1 || !ok2 || !ok3 || !ok4) {
        return std::nullopt;
    }

    return entry;
}

QList<GameHistoryEntry> ServerProtocolParser::parseGameHistory(const QString &response)
{
    QList<GameHistoryEntry> history;

    if (response.isEmpty() || isError(response)) {
        return history;
    }

    QStringList entries = response.split(';');
    for (const QString &entry : entries) {
        auto historyEntry = parseGameHistoryEntry(entry.trimmed());
        if (historyEntry) {
            history.append(historyEntry.value());
        }
    }

    return history;
}

HandleWordStatus ServerProtocolParser::parseHandleWordStatus(const QString &response)
{
    if (isError(response)) {
        return HandleWordStatus::GOT_ERROR;
    }

    if (response == "OK") return HandleWordStatus::OK;
    if (response == "GOT_ERROR") return HandleWordStatus::GOT_ERROR;
    if (response == "WRONG_ORDER") return HandleWordStatus::WRONG_ORDER;
    if (response == "NOT_JAPANESE_WORD") return HandleWordStatus::NOT_JAPANESE_WORD;
    if (response == "NO_SPEACH_PART") return HandleWordStatus::NO_SPEACH_PART;
    if (response == "NO_FOUND_WORD") return HandleWordStatus::NO_FOUND_WORD;
    if (response == "GOT_END_WORD") return HandleWordStatus::GOT_END_WORD;
    if (response == "GOT_DOUBLED_WORD") return HandleWordStatus::GOT_DOUBLED_WORD;
    if (response == "CANT_JOIN_WORDS") return HandleWordStatus::CANT_JOIN_WORDS;
    if (response == "GAME_NOT_FOUND") return HandleWordStatus::GAME_NOT_FOUND;
    if (response == "GAME_STOPPED") return HandleWordStatus::GAME_STOPPED;
    if (response == "NO_FOUND_PLAYER") return HandleWordStatus::NO_FOUND_PLAYER;

    return HandleWordStatus::UNKNOWN;
}

bool ServerProtocolParser::isError(const QString &response)
{
    return response.startsWith("Error:");
}

QString ServerProtocolParser::extractErrorMessage(const QString &response)
{
    if (!isError(response)) {
        return "";
    }
    return response.mid(6).trimmed();
}

std::optional<NewWordUpdate> ServerProtocolParser::parseNewWordUpdate(const QString &response)
{
    // Format: NewWord <gameId> {"kanji":string,"meaning":string,"partsOfSpeach":[]string,"readings":[]string}
    if (!response.startsWith("NewWord")) {
        return std::nullopt;
    }

    // This is a simplified parser. In production, you'd use a JSON library.
    // For now, we'll extract basic info
    QStringList parts = response.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        return std::nullopt;
    }

    NewWordUpdate update;
    bool ok;
    update.gameId = parts[1].toULongLong(&ok);
    if (!ok) {
        return std::nullopt;
    }

    // TODO: Parse JSON data from parts[2] onwards
    // For now, return basic structure
    return update;
}
