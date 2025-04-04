#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL3/SDL.h>
namespace CONSTANTS {
constexpr bool ENABLE_LOG{ false };

constexpr SDL_Color COLOR_RED   = { 255, 0, 0, 255 };
constexpr SDL_Color COLOR_GREEN = { 0, 255, 0, 255 };
constexpr SDL_Color COLOR_BLUE  = { 0, 0, 255, 255 };
constexpr SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
constexpr SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };

constexpr char COLOR_RED_STR[]   = "RED";
constexpr char COLOR_GREEN_STR[] = "GREEN";
constexpr char COLOR_BLUE_STR[]  = "BLUE";
constexpr char COLOR_WHITE_STR[] = "WHITE";
constexpr char COLOR_BLACK_STR[] = "BLACK";

constexpr SDL_FRect FRECT_ONE = { 1, 1, 1, 1 };

namespace HELPERS {
constexpr char* COLOR_TO_NAME(const SDL_Color& c) {
    int flag{ 0 };
    if (c.r == 255) {
        flag += 1;
    }
    if (c.g == 255) {
        flag += 2;
    }
    if (c.b == 255) {
        flag += 4;
    }
    constexpr int RED{ 1 };
    constexpr int GREEN{ 2 };
    constexpr int BLUE{ 4 };
    constexpr int WHITE{ 7 };
    constexpr int BLACK{ 0 };
    switch (flag) {
    case RED: return const_cast<char*>(COLOR_RED_STR);
    case GREEN: return const_cast<char*>(COLOR_GREEN_STR);
    case BLUE: return const_cast<char*>(COLOR_BLUE_STR);
    case WHITE: return const_cast<char*>(COLOR_WHITE_STR);
    case BLACK: return const_cast<char*>(COLOR_BLACK_STR);
    default:
        if (ENABLE_LOG) {
            SDL_Log("ERROR: COLOR_TO_NAME <%d>\n", flag);
        }
        return const_cast<char*>("CC");
    }
}
} // namespace HELPERS
} // namespace CONSTANTS
#endif