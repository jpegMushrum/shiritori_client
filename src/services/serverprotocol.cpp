#include "serverprotocol.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <variant>

std::optional<UserInfo> ServerProtocolParser::parseUserInfo(const QString &response)
{
    if (isError(response))
    {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 3)
    {
        return std::nullopt;
    }

    UserInfo info;
    bool ok1, ok2;
    info.userId = parts[0].toULongLong(&ok1);
    info.nickname = parts[1];
    info.averageWordsPerGame = parts[2].toDouble(&ok2);

    if (!ok1 || !ok2)
    {
        return std::nullopt;
    }

    return info;
}

std::optional<GameContext> ServerProtocolParser::parseGameContext(const QString &response)
{
    if (isError(response))
    {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 6)
    {
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

    if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5)
    {
        return std::nullopt;
    }

    return context;
}

QList<GameContext> ServerProtocolParser::parseMultipleGameContexts(const QString &response)
{
    QList<GameContext> contexts;

    if (response.isEmpty() || isError(response))
    {
        return contexts;
    }

    QStringList games = response.split(';');
    for (const QString &game : games)
    {
        auto context = parseGameContext(game.trimmed());
        if (context)
        {
            contexts.append(context.value());
        }
    }

    return contexts;
}

std::optional<GameHistoryEntry> ServerProtocolParser::parseGameHistoryEntry(const QString &response)
{
    if (isError(response))
    {
        return std::nullopt;
    }

    QStringList parts = response.split(' ');
    if (parts.size() < 4)
    {
        return std::nullopt;
    }

    GameHistoryEntry entry;
    bool ok1, ok2, ok3, ok4;
    entry.gameId = parts[0].toULongLong(&ok1);
    entry.userId = parts[1].toULongLong(&ok2);
    entry.wordsCount = parts[2].toInt(&ok3);
    entry.place = parts[3].toInt(&ok4);

    if (!ok1 || !ok2 || !ok3 || !ok4)
    {
        return std::nullopt;
    }

    return entry;
}

QList<GameHistoryEntry> ServerProtocolParser::parseGameHistory(const QString &response)
{
    QList<GameHistoryEntry> history;

    if (response.isEmpty() || isError(response))
    {
        return history;
    }

    QStringList entries = response.split(';');
    for (const QString &entry : entries)
    {
        auto historyEntry = parseGameHistoryEntry(entry.trimmed());
        if (historyEntry)
        {
            history.append(historyEntry.value());
        }
    }

    return history;
}

HandleWordStatus ServerProtocolParser::parseHandleWordStatus(const QString &response)
{
    if (isError(response))
    {
        return HandleWordStatus::GOT_ERROR;
    }

    if (response == "OK")
        return HandleWordStatus::OK;
    if (response == "GOT_ERROR")
        return HandleWordStatus::GOT_ERROR;
    if (response == "WRONG_ORDER")
        return HandleWordStatus::WRONG_ORDER;
    if (response == "NOT_JAPANESE_WORD")
        return HandleWordStatus::NOT_JAPANESE_WORD;
    if (response == "NO_SPEACH_PART")
        return HandleWordStatus::NO_SPEACH_PART;
    if (response == "NO_FOUND_WORD")
        return HandleWordStatus::NO_FOUND_WORD;
    if (response == "GOT_END_WORD")
        return HandleWordStatus::GOT_END_WORD;
    if (response == "GOT_DOUBLED_WORD")
        return HandleWordStatus::GOT_DOUBLED_WORD;
    if (response == "CANT_JOIN_WORDS")
        return HandleWordStatus::CANT_JOIN_WORDS;
    if (response == "GAME_NOT_FOUND")
        return HandleWordStatus::GAME_NOT_FOUND;
    if (response == "GAME_STOPPED")
        return HandleWordStatus::GAME_STOPPED;
    if (response == "NO_FOUND_PLAYER")
        return HandleWordStatus::NO_FOUND_PLAYER;

    return HandleWordStatus::UNKNOWN;
}

bool ServerProtocolParser::isError(const QString &response)
{
    return response.startsWith("Error:");
}

QString ServerProtocolParser::extractErrorMessage(const QString &response)
{
    if (!isError(response))
    {
        return "";
    }
    return response.mid(6).trimmed();
}

std::optional<PlayerJoinedGameInfo> ServerProtocolParser::parsePlayerJoinedGameInfo(const QString &response)
{
    // Format: playerJoinedGame <gameId> {"lastKana":"string","usedWords":[{word objects}]}
    if (!response.startsWith("playerJoinedGame"))
    {
        return std::nullopt;
    }

    // Split command, gameId, and JSON payload
    int firstSpace = response.indexOf(' ');
    if (firstSpace == -1)
    {
        return std::nullopt;
    }

    int secondSpace = response.indexOf(' ', firstSpace + 1);
    if (secondSpace == -1)
    {
        return std::nullopt;
    }

    // Extract gameId
    QString gameIdStr = response.mid(firstSpace + 1, secondSpace - firstSpace - 1);
    bool ok;
    qulonglong gameId = gameIdStr.toULongLong(&ok);
    if (!ok)
    {
        return std::nullopt;
    }

    // Extract JSON payload
    QString jsonStr = response.mid(secondSpace + 1).trimmed();

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (!doc.isObject())
    {
        return std::nullopt;
    }

    QJsonObject obj = doc.object();

    // Extract lastKana
    if (!obj.contains("lastKana"))
    {
        return std::nullopt;
    }

    PlayerJoinedGameInfo info;
    info.gameId = gameId;
    info.lastKana = obj["lastKana"].toString();

    // Extract usedWords array
    if (obj.contains("usedWords") && obj["usedWords"].isArray())
    {
        QJsonArray wordsArray = obj["usedWords"].toArray();
        for (const QJsonValue &wordValue : wordsArray)
        {
            if (!wordValue.isObject())
                continue;

            QJsonObject wordObj = wordValue.toObject();

            NewWordUpdate word;
            word.gameId = gameId;
            word.kanji = wordObj["kanji"].toString();

            // Extract meanings array
            if (wordObj["meanings"].isArray())
            {
                QJsonArray meaningsArray = wordObj["meanings"].toArray();
                for (const QJsonValue &value : meaningsArray)
                {
                    word.meanings.append(value.toString());
                }
            }

            // Extract partsOfSpeech array
            if (wordObj.contains("partsOfSpeach") && wordObj["partsOfSpeach"].isArray())
            {
                QJsonArray partsArray = wordObj["partsOfSpeach"].toArray();
                for (const QJsonValue &value : partsArray)
                {
                    word.partsOfSpeech.append(value.toString());
                }
            }

            // Extract readings array
            if (wordObj.contains("readings") && wordObj["readings"].isArray())
            {
                QJsonArray readingsArray = wordObj["readings"].toArray();
                for (const QJsonValue &value : readingsArray)
                {
                    word.readings.append(value.toString());
                }
            }

            info.usedWords.append(word);
        }
    }

    return info;
}

std::optional<std::variant<WordPlayedEvent, GameStoppedEvent>>
ServerProtocolParser::parseGameUpdate(const QString &response)
{
    if (!response.startsWith("gameUpdate"))
        return std::nullopt;

    int firstSpace = response.indexOf(' ');
    int secondSpace = response.indexOf(' ', firstSpace + 1);
    if (secondSpace == -1)
        return std::nullopt;

    bool ok;
    qulonglong gameId = response.mid(firstSpace + 1,
                                     secondSpace - firstSpace - 1)
                            .toULongLong(&ok);
    if (!ok)
        return std::nullopt;

    QString jsonStr = response.mid(secondSpace + 1).trimmed();

    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (!doc.isObject())
        return std::nullopt;

    QJsonObject obj = doc.object();

    QString type = obj["type"].toString();

    // ================= WORD PLAYED =================
    if (type == "wordPlayed")
    {
        WordPlayedEvent event;
        event.type = GameEventType::WordPlayed;
        event.gameId = gameId;

        QJsonObject wordObj = obj["word"].toObject();

        event.word.gameId = gameId;
        event.word.kanji = wordObj["kanji"].toString();

        // meanings
        for (auto v : wordObj["meanings"].toArray())
            event.word.meanings.append(v.toString());

        // partsOfSpeach
        for (auto v : wordObj["partsOfSpeach"].toArray())
            event.word.partsOfSpeech.append(v.toString());

        // readings
        for (auto v : wordObj["readings"].toArray())
            event.word.readings.append(v.toString());

        event.lastKana = obj["lastKana"].toString();

        return event;
    }

    // ================= GAME STOPPED =================
    if (type == "gameStopped")
    {
        GameStoppedEvent event;
        event.type = GameEventType::GameStopped;
        event.gameId = gameId;

        QJsonObject scoresObj = obj["scores"].toObject();

        bool ok;
        event.scores.userId = scoresObj["userId"].toVariant().toULongLong(&ok);
        event.scores.score = scoresObj["score"].toInt();

        if (ok)
        {
            qDebug() << "Game stopped - User:" << event.scores.userId << "Score:" << event.scores.score;
            return event;
        }
        else
        {
            qWarning() << "Failed to parse scores object";
            return std::nullopt;
        }
    }

    return std::nullopt;
}
