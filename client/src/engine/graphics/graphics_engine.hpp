#ifndef GRAPHICS_ENGINE
#define GRAPHICS_ENGINE

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#include <string>

#include "rectangles.hpp"
#include "image.hpp"
#include "sound.hpp"
#include "text.hpp"

class GraphicsContext final {
private:
    int W_W { 800 };
    int W_H { 500 };

public:
    SDL_Window *window { nullptr };
    SDL_Renderer *renderer { nullptr };
    // SDL_Window* getWindow() { return window; }
    // SDL_Renderer* getRenderer() { return renderer; }

    void Init_SDL()
    {
        //Initialize SDL
        if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
            throw(SDL_GetError());
        }
        else
        {
            // printf("Succesfully initialised SDL!\n");
            //Set texture filtering to linear
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                printf( "Warning: Linear texture filtering not enabled!" );
            }

            //Create window
            this->window = SDL_CreateWindow("PONG++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->W_W, this->W_H, 0 | SDL_WINDOW_RESIZABLE);
            if( window == NULL )
            {
                printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
                throw(SDL_GetError());
            }
            else
            {
                //Create renderer for window
                this->renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
                if( renderer == NULL )
                {
                    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                    throw(SDL_GetError());
                }
                else
                {
                    // printf("Succesfully initialised window and renderer!\n");
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                    //Initialize renderer color
                    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                        throw(IMG_GetError());
                    }
                }
            }

            SDL_RenderClear(this->renderer);
            SDL_RenderPresent(this->renderer);
        }
        //Initialize TTF and MIXER
        TTF_Init();
        Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

        // Initializing AUDIO subsystem
        SDL_InitSubSystem(SDL_INIT_AUDIO);
        Mix_AllocateChannels(1);
        Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

        // return (window && renderer);
    }

    void Quit_SDL()
    {
        //Destroy renderer and window
        SDL_DestroyRenderer( renderer );
        SDL_DestroyWindow( window );
        window = nullptr;
        renderer = nullptr;

        //Quit SDL subsystems
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

public:
    GraphicsContext() {
        Init_SDL();
    }
    GraphicsContext(int width, int height)
        : W_W {width}, W_H {height} {
        Init_SDL();
    }

    ~GraphicsContext() {
        Quit_SDL();
    }

public:
    int getWidth() { return W_W; }
    int getHeight() { return W_H; }

    void display() {
        SDL_RenderPresent(this->renderer);
    }

    void updateWindowSize(int w, int h) {
        W_W = w;
        W_H = h;

        // SDL_SetWindowSize(window, W_W, W_H);
    }
};

#endif