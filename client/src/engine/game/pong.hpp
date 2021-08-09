#ifndef PONG_HPP
#define PONG_HPP

#include "../../services/client.hpp"
#include "../graphics/graphics_engine.hpp"
#include "../ui/ui_engine.hpp"

enum DIR {
    UP,
    DOWN,
};

class Ball {
    private:
        SDL_Point bPos { 0.5, 0.5 };
        SDL_Point bVel { 0.05, 0 };
        SDL_Rect bRect { 0.475, 0.475, 0.025, 0.025 };
    
    public:
        Ball() = default;
        // Ball(int v);
        
        SDL_Point getPos();
        SDL_Point getVel();

        void move();
        void display(SDL_Renderer *renderer, const SDL_Rect &window_rect);
};

class Paddle {
    private:
        const SDL_Color padClr { 0, 0, 255, 255 };
        SDL_Rect padRct { 0.2, 0.1, 0.01, 0.05 };
        //maybe implement active, whereby strong ball knocks paddle into inactivity

    public:
        bool lcl { true };
        DIR dir { UP };

        Paddle() = default;

        SDL_Point getPos();

        void move(const SDL_Point &cursorPos, bool remote);
        void display(SDL_Renderer *renderer, const SDL_Rect &window_rect, bool remote);

};

class Game {
private:
    std::shared_ptr<GraphicsContext> context;
    std::shared_ptr<CustomClient client> connection;

    bool done { false };
    int score[2] {0, 0};

    SDL_Rect background;
    SDL_Colour bckgr_clr { 120, 250, 50, 255 };
    Ball ball;
    Paddle lcl_paddle;
    Paddle rmt_paddle;
    SDL_Rect lcl_goal { 0, 0.2, 0.01, 0.6 };
    SDL_Rect rmt_goal { 1, 0.2, 0.01, 0.5 };

public:
    Game(std::shared_ptr<GraphicsContext> context, std::shared_ptr<CustomClient client> connection)
        :   context(context) connection(connection), background({0, 0, context->getWidth(), context->getHeight()}) {}

public:
    void checkCollision();
    void update();
    void start();
}

#endif