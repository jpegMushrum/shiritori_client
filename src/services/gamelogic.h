#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QString>
#include "../models/game.h"

class GameLogic
{
public:
    GameLogic() = default;

    // TODO: Word validation
    bool isValidWord(const QString &word);
    bool isLastKanaMatching(const QString &word, const QString &lastKana);

    // TODO: Kana extraction
    QString extractLastKana(const QString &word);

    // TODO: Game state management
    bool addWordToGame(Game &game, const QString &word);

private:
    // TODO: Add kana dictionary/processing utilities
};

#endif // GAMELOGIC_H
