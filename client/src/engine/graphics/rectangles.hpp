#ifndef RECTANGLES_HPP
#define RECTANGLES_HPP

#include "graphics_engine.hpp"

inline bool isContained(SDL_Point cursor_pos, SDL_Rect rect) {
    return (cursor_pos.x > rect.x && cursor_pos.x < (rect.x + rect.w) && cursor_pos.y > rect.y && cursor_pos.y < (rect.y + rect.h));
}

void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r);
void drawCutRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r);

#endif