#ifndef PONG_HPP
#define PONG_HPP

#include "../../../dependencies/nlohmann/json.hpp"
#include "../../services/client.hpp"
#include "../../utilities/timer.hpp"
#include "../graphics/graphics_engine.hpp"
#include "../ui/ui_engine.hpp"

using JSON = nlohmann::json;

enum DIR {
    UP,
    DOWN,
};

class Point {
private:
    SDL_Point point;

public:
    double x, y;
    SDL_Point getSDLPoint();
    SDL_Point updatePos(std::shared_ptr<GraphicsContext> context);
    friend SDL_Point operator*(const Point& rect, const SDL_Rect& context); 

public:
    Point(std::shared_ptr<GraphicsContext> context, double x, double y);
    Point(double x, double y);
};

class Rect {
private:
    SDL_Rect rect;

public:
    double x, y, w, h;
    SDL_Rect getSDLRect();
    SDL_Rect updatePos(std::shared_ptr<GraphicsContext> context);
    friend SDL_Rect operator*(const Rect& rect, const SDL_Rect& context); 

public:
    Rect(std::shared_ptr<GraphicsContext> context, double x, double y, double w, double h);
    Rect(double x, double y, double w, double h);
};

class Ball {
public:
    static const Rect b_inRect;
    static const Point b_inVel;

private:
    Point bVel { 0.0001, 0 };
    Rect bRect { 0.4875, 0.4875, 0.025, 0.025 };
    
public:
    Ball() = default;
    // Ball(int v);
    
    Rect& getRect();
    Point& getVel();

    // void setPos(double x, double y);
    // void setVel(double x, double y);

    void move();
    void display(SDL_Renderer *renderer, const SDL_Rect &window_rect);
};

class Paddle {
    private:
        const SDL_Color padClr { 0, 0, 255, 255 };
        Rect padRct { 0.2, 0.1, 0.01, 0.05 };  // !!!!!!!!!!
        //maybe implement active, whereby strong ball knocks paddle into inactivity

    public:
        bool lcl { true };
        DIR dir { UP };

        Paddle() = default;

        Point getPos();
        Rect& getRect();

        void move(const Point &cursorPos, bool remote);
        void display(SDL_Renderer *renderer, const SDL_Rect &window_rect, bool remote);

};

class Game {
private:
    std::shared_ptr<GraphicsContext> context;
    std::shared_ptr<CustomClient> connection;

    bool done { false };
    int score[2] {0, 0};

    SDL_Rect background;
    SDL_Colour bckgr_clr { 120, 250, 50, 255 };
    Ball ball;
    Paddle lcl_paddle;
    Paddle rmt_paddle;
    Rect lcl_goal { 0, 0.2, 0.01, 0.6 };  // !!!!!!!!!!
    Rect rmt_goal { 0.99, 0.2, 0.01, 0.5 };  // !!!!!!!!!!

public:
    Game(std::shared_ptr<GraphicsContext> context, std::shared_ptr<CustomClient> connection)
        :   context(context), connection(connection), background({0, 0, context->getWidth(), context->getHeight()}) {}

public:
    void checkCollision();
    void update(Point lcl_pos, Point rmt_pos);
    void display();
    bool start();
};

#endif