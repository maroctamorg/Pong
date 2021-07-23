#ifndef PONG_HPP
#define PONG_HPP

#include "services/client.hpp"
#include "../graphics/graphics_engine.hpp"
#include "../ui/ui_engine.hpp"

enum DIR {
    UP,
    DOWN,
};

class Ball {
    private:
        SDL_Point bPos { 400, 250 };
        SDL_Point bVel { 5, 0 };
        SDL_Rect bRect { 390, 240, 20, 20 };
    
    public:
        Ball(int v);
        
        SDL_Point getPos();
        SDL_Point getVel();

        void move();
        void checkCollision(SDL_Point padPos, bool lcl);

        void display(SDL_Renderer *renderer);
};

class Paddle {
    private:
        const SDL_Color padClr { 0, 0, 250, 250 };
        SDL_Rect padRct { 100, 50, 5, 50 };
        //maybe implement active, whereby strong ball knocks paddle into inactivity

    public:
        bool lcl { true };
        DIR dir { UP };

        Paddle() = default;

        SDL_Point getPos();

        void move(const SDL_Point &cursorPos, bool remote);
        void display(SDL_Renderer *renderer, bool remote);

};

#endif