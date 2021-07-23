#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
/*#include "text.h"
#include "Engine.h"
#include "button.h"
#include "menu.h"*/
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cassert>

namespace g {
    extern const int W_W;
    extern const int W_H;

    extern const char* saveFile;
    extern const std::string font;
    extern const SDL_Colour scoreCounterTxtColour;
    extern const SDL_Rect scoreCounterRect;
    extern const SDL_Colour scoreCounterRectColour;

}

typedef struct
{
    bool active, pressed;
} state;

enum GAME_STATE
{
    NOINPUT,
    UPDATE,
    TERMINATE,
    ANIMATION,
};

inline bool isContained(SDL_Point cursor_pos, SDL_Rect rect) {
    return (cursor_pos.x > rect.x && cursor_pos.x < (rect.x + rect.w) && cursor_pos.y > rect.y && cursor_pos.y < (rect.y + rect.h));
}

#endif