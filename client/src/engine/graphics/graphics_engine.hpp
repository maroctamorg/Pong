#ifndef GRAPHICS_ENGINE
#define GRAPHICS_ENGINE

#include <SDL2/SDL.h>

#include "image.hpp"
#include "sound.hpp"
#include "text.hpp"

const int W_W { 800 };
const int W_H { 500 };

SDL_Window *window { nullptr };
SDL_Renderer *renderer { nullptr };

bool Init_SDL()
{

    SDL_Init(SDL_INIT_EVERYTHING);
    //TTF_Init();

    window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_W, W_H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    return (window && renderer);
}

void Quit_SDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //TTF_Quit();
    SDL_Quit();
}

#endif