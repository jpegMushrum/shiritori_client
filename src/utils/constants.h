#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants
{
    // TODO: API endpoints
    inline constexpr const char *API_BASE_URL = "http://localhost:8000";

    // TODO: Game settings
    inline constexpr int MIN_WORD_LENGTH = 2;
    inline constexpr int MAX_PLAYERS_PER_GAME = 4;

    // TODO: UI constants
    inline constexpr int WINDOW_WIDTH = 800;
    inline constexpr int WINDOW_HEIGHT = 600;
}

#endif // CONSTANTS_H
