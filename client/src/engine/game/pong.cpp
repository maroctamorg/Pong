#include "pong.hpp"

Ball::Ball(int v): bVel { v, 0 }, bPos { 400, 250 }, bRect { 390, 240, 20, 20 } {};
        
SDL_Point Ball::getPos() {
    return this->bPos;
}

SDL_Point Ball::getVel(){
    return this->bVel;
}

void Ball::move(){
    bPos.x += bVel.x;
    bPos.y += bVel.y;

    bRect.x = bPos.x - 10;
    bRect.y = bPos.y - 10;
}

void Ball::checkCollision(SDL_Point padPos, bool lcl) {
    SDL_Rect lcl_goal { 0, 100, 5, 300 };
    SDL_Rect rmt_goal { 795, 100, 5, 300 };

    bool goalX { bPos.x <= 800 && bPos.x >= 795 };
    bool goalY { bPos.y <= 400 && bPos.y >= 100 };

    bool goalLX { bPos.x <= 5 && bPos.x >= 0 };
    bool goalLY { bPos.y <= 400 && bPos.y >= 100 };

    if ((goalX && goalY) || (goalLX && goalLY)) {
        bPos.x = 400;
        bPos.y = 250;

        //bVel.x = 5;
        //bVel.y = 0;
    }
    
    if (bPos.x >= 800) {
        bPos.x = 800;
        bVel.x = -5;
    } else if (bPos.x <= 0) {
        bPos.x = 0;
        bVel.x = 5;
    }

    if (bPos.y >= 500) {
        bPos.y = 500;
        bVel.y = -5;
    } else if (bPos.y <= 0) {
        bPos.y = 0;
        bVel.y = 5;
    }

    bool hitX { bPos.x <= padPos.x + 5 && bPos.x >= padPos.x };
    bool hitY { bPos.y <= padPos.y + 50 && bPos.y >= padPos.y };

    if (hitX && hitY) {
        if (padPos.y <= 400) {
            bVel.y = 5;
        } else if (padPos.y >= 400) {
            bVel.y = -5;
        }

        if (lcl) {
            bVel.x = 5;
        } else {
            bVel.x = -5;
        }
    }

    bRect.x = bPos.x - 10;
    bRect.y = bPos.y - 10;

}

void Ball::display(SDL_Renderer *renderer) {
    bRect.x = bPos.x - 10;
    bRect.y = bPos.y - 10;

    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 250);
    SDL_RenderFillRect(renderer, &bRect);
}

SDL_Point Paddle::getPos() {
    return { padRct.x, padRct.y };
}

void Paddle::move(const SDL_Point &cursorPos, bool remote) {
    
    if (cursorPos.x < 400 || remote) {
        padRct.x = cursorPos.x;
    }

    padRct.y = cursorPos.y;
}

void Paddle::display(SDL_Renderer *renderer, bool remote) {
    if (!remote) {
        SDL_SetRenderDrawColor(renderer, padClr.r, padClr.g, padClr.b, padClr.a);
    } else {
        SDL_SetRenderDrawColor(renderer, 250, 0, 0, 250);
    }
    SDL_RenderFillRect(renderer, &padRct);
}