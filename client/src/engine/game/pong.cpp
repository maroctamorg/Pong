#include "pong.hpp"

SDL_Point Point::getSDLPoint() { return point; }
SDL_Point Point::updatePos(std::shared_ptr<GraphicsContext> context) {
    this->point = { static_cast<int>(x*context->getWidth()), static_cast<int>(y*context->getHeight())};
    return this->point;
}

Point::Point(std::shared_ptr<GraphicsContext> context, double x, double y)
    :   x{x}, y{y}, point{static_cast<int>(x*context->getWidth()), static_cast<int>(y*context->getHeight())} {}
Point::Point(double x, double y)
    :   x{x}, y{y}, point{0, 0} {}

SDL_Point operator*(const Point& point, const SDL_Rect& context) {
    return { static_cast<int>(point.x*context.w + context.x), static_cast<int>(point.y*context.h + context.y) };
}

SDL_Rect Rect::getSDLRect() { return rect; }
SDL_Rect Rect::updatePos(std::shared_ptr<GraphicsContext> context) {
    this->rect = {static_cast<int>(x*context->getWidth()), static_cast<int>(y*context->getHeight()), static_cast<int>(w*context->getWidth()), static_cast<int>(h*context->getHeight())};
    return this->rect;
}

Rect::Rect(std::shared_ptr<GraphicsContext> context, double x, double y, double w, double h)
    :   x{x}, y{y}, w{w}, h{h}, rect{static_cast<int>(x*context->getWidth()), static_cast<int>(y*context->getHeight()), static_cast<int>(w*context->getWidth()), static_cast<int>(h*context->getHeight())} {}
Rect::Rect(double x, double y, double w, double h)
    :   x{x}, y{y}, w{w}, h{h}, rect{0, 0, 0, 0} {}

SDL_Rect operator*(const Rect& rect, const SDL_Rect& context) {
    return { static_cast<int>(rect.x*context.w + context.x), static_cast<int>(rect.y*context.h + context.y), static_cast<int>(rect.w*context.w), static_cast<int>(rect.h*context.h) };
}

const Rect Ball::b_inRect {0.4875, 0.4875, 0.025, 0.025};

// Ball::Ball(int v): bVel { v, 0 }, bPos { 400, 250 }, bRect { 390, 240, 20, 20 } {};

Rect& Ball::getRect() {
    return this->bRect;
}
Point& Ball::getVel() {
    return this->bVel;
}

void Ball::setPos(double x, double y) {
    this->bRect.x = x;
    this->bRect.y = y;
}
void Ball::setVel(double x, double y) {
    this->bVel.x = x;
    this->bVel.y = y;
}

void Ball::move(){
    bRect.x += bVel.x;
    bRect.y += bVel.y;
}

void Ball::display(SDL_Renderer *renderer, const SDL_Rect &window_rect) {
    // SDL_Rect target = {
    //     bRect.x * window_rect.w,
    //     bRect.y * window_rect.h,
    //     bRect.w * window_rect.w,
    //     bRect.h * window_rect.h
    // };
    SDL_Rect target = bRect * window_rect;

    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 250);
    SDL_RenderFillRect(renderer, &target);
}

Point Paddle::getPos() {
    return { padRct.x, padRct.y };
}

Rect& Paddle::getRect() {
    return this->padRct;
}

void Paddle::move(const Pos &cursorPos, bool remote) {
    
    if (cursorPos.x <= 0.5 && !remote) {
        padRct.x = cursorPos.x;
    } else if (cursorPos.x >= 0.5 && remote) {
        padRct.x = cursorPos.x;
    }

    padRct.y = cursorPos.y;
}

void Paddle::display(SDL_Renderer *renderer, const SDL_Rect& window, bool remote) {
    if (!remote) {
        SDL_SetRenderDrawColor(renderer, padClr.r, padClr.g, padClr.b, padClr.a);
    } else {
        SDL_SetRenderDrawColor(renderer, 250, 0, 0, 250);
    }
    SDL_Rect padRect = this->padRct * window;
    SDL_RenderFillRect(renderer, &padRect);
}

bool checkRelCollision(GraphicsContext* context, Rect r_rect1, Rect r_rect2) {
    SDL_Rect test { 0, 0, context->getWidth(), context->getHeight() };
    context = nullptr;
    SDL_Rect rect1 = r_rect1*test;
    SDL_Rect rect2 = r_rect2*test;
    return SDL_HasIntersection(&rect1, &rect2);
}

void Game::checkCollision() {
    // CHECK FOR GOALS
    Rect& bRect = this->ball.getRect();
    Point& bVel = this->ball.getVel();
    if(checkRelCollision(this->context.get(), bRect, this->lcl_goal)) {
        score[1] += 1;
        bRect = this->ball.b_inRect;
        bVel = this->ball.b_inVel;
    }
    // else if (checkRelCollision(this->context.get(), bRect, this->rmt_goal)) {
    //     score[0] += 1;
    //     bRect = this->ball.b_inRect;
    //     bVel = {-this->ball.b_inVel.x, -this->ball.b_inVel.y};
    // }

    // CHECK FOR BOUNCE ON EDGES
    if (bRect.x >= 1) {
        bRect.x = 1;
        bVel.x = -bVel.x;
    } else if (bRect.x <= 0) {
        bRect.x = 0;
        bVel.x = -bVel.x;
    }

    if (bRect.y >= 1) {
        bRect.y = 1;
        bVel.y = -bVel.y;
    } else if (bRect.y <= 0) {
        bRect.y = 0;
        bVel.y = -bVel.y;
    }

    Rect& lcl_pad_pos = this->lcl_paddle.getRect();
    Rect& rmt_pad_pos = this->rmt_paddle.getRect();

    // CHECK FOR LOCAL ---AND REMOTE--- PADDLE COLLISIONS
    if (checkRelCollision(this->context.get(), bRect, lcl_pad_pos)) {
        if(bRect.x < lcl_pad_pos.x + static_cast<int>(lcl_pad_pos.w/2)) {
            bRect.x = lcl_pad_pos.x - bRect.w;
            bVel.x = bVel.x >= 0 ? -bVel.x : bVel.x;
        } else {
            bRect.x = lcl_pad_pos.x + lcl_pad_pos.w;
            bVel.x = bVel.x >= 0 ? bVel.x : -bVel.x;
        }
        if (lcl_pad_pos.y <= 0.5) {
            bVel.y = bVel.y >= 0 ? (bVel.y == 0 ? this->ball.b_inVel.x : bVel.y) : -bVel.y;
        } else if (lcl_pad_pos.y > 0.5) {
            bVel.y = bVel.y >= 0 ? (bVel.y == 0 ? -this->ball.b_inVel.x : -bVel.y) : bVel.y;
        }
    }
    // else if (checkRelCollision(this->context.get(), bRect, rmt_pad_pos)) {
    //     if(bRect.x < rmt_pad_pos.x + static_cast<int>(rmt_pad_pos.w/2)) {
    //         bRect.x = rmt_pad_pos.x - bRect.w;
    //         bVel.x = bVel.x >= 0 ? -bVel.x : bVel.x;
    //     } else {
    //         bRect.x = rmt_pad_pos.x + rmt_pad_pos.w;
    //         bVel.x = bVel.x >= 0 ? bVel.x : -bVel.x;
    //     }
    //     if (rmt_pad_pos.y <= 0.5) {
    //         bVel.y = bVel.y >= 0 ? (bVel.y == 0 ? this->ball.b_inVel.x : bVel.y) : -bVel.y;
    //     } else if (rmt_pad_pos.y > 0.5) {
    //         bVel.y = bVel.y >= 0 ? (bVel.y == 0 ? -this->ball.b_inVel.x : -bVel.y) : bVel.y;
    //     }
    // }
}

void Game::update(Pos lcl_pos, Pos rmt_pos, Pos ballPos, Pos ballVel) {
    lcl_paddle.move(lcl_pos, false);
    rmt_paddle.move(rmt_pos, true);
    ball.setPos(ballPos.x, ballPos.y);
    ball.setVel(ballVel.x, ballVel.y);
    ball.move();

    this->checkCollision();
}

void Game::display() {
    SDL_Rect window_rect {0, 0, this->context->getWidth(), this->context->getHeight()};
    
    SDL_SetRenderDrawColor(this->context->renderer, 250, 250, 250, 250);
    SDL_RenderClear(this->context->renderer);

    SDL_SetRenderDrawColor(this->context->renderer, 0, 0, 0, 250);
    SDL_RenderDrawLine(this->context->renderer, window_rect.w*0.5, 0, window_rect.w*0.5, window_rect.h);
    //add moving line functionality

    SDL_SetRenderDrawColor(this->context->renderer, 50, 150, 250, 250);
    SDL_Rect l_goal = lcl_goal*window_rect;
    SDL_RenderFillRect(this->context->renderer, &l_goal);
    SDL_SetRenderDrawColor(this->context->renderer, 250, 150, 50, 250);
    SDL_Rect r_goal = rmt_goal*window_rect;
    SDL_RenderFillRect(this->context->renderer, &r_goal);
    
    ball.display(this->context->renderer, window_rect);
    lcl_paddle.display(this->context->renderer, window_rect, false);
    rmt_paddle.display(this->context->renderer, window_rect, true);

    SDL_RenderPresent(this->context->renderer);
}

std::string custom_struct_utils::toString(Game_Info info) {
    std::ostringstream oss;
    oss << "\ndone: " << (info.done ? "true" : "false") << ",\tscore: " << info.score << "\nracket:\tx: " << info.racketPos.x << ", y: " << info.racketPos.y << '\n';
    return oss.str();
}

bool Game::start() {
    Pos lcl_cursor_pos { 0.5, 0.5 };
    Pos rmt_cursor_pos { 0.5, 0.5 };
    Pos ballPos { this->ball.getRect().x, this->ball.getRect().y };
    Pos ballVel { this->ball.getVel().x, this->ball.getVel().y };
    bool done {false};

    std::thread server_update_thr = std::thread([this, &done, &rmt_cursor_pos, &ballPos, &ballVel, connection { this->connection }]() mutable {
        STATE state { STATE::START };
        Game_Info sData;
        int counter {-1};
        Timer sLoop;
        while(!done) {
            counter++;
            state = connection->handleIncoming(&sData);
            switch(state) {
            case (STATE::QUIET) :
                if(false && counter > 10000) {
                    std::cout << "Communication with the server has failed, leaving match...\n";
                    done = true;
                    return false;
                }
                break;
            case (STATE::END) :
                done = true;
                break;
            case (STATE::GAME_INFO) :
                // std::cout << "Received game info from server: " << custom_struct_utils::toString(sData) << '\n';
                rmt_cursor_pos = sData.racketPos;
                ballPos = sData.ballPos;
                ballVel = sData.ballVel;
                this->checkCollision();
                counter = -1;
                break;
            default :
                std::cout << "Unhandled server response!\n";
                counter = -1;
            }

            // consistent 240 Hz communication
            // std::cout << "server update thread elapsed time: " << sLoop.elapsed() << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/240 - sLoop.elapsed()))));
            sLoop.reset();
        }
    });

    std::thread display_thr = std::thread([this, &done, &lcl_cursor_pos, &rmt_cursor_pos, &ballPos, &ballVel]() {
        Timer gLoop;
        while(!done) {
            // UPDATE
            this->update(lcl_cursor_pos, rmt_cursor_pos, ballPos, ballVel);
            ballPos = { this->ball.getRect().x, this->ball.getRect().y };
            ballVel = { this->ball.getVel().x, this->ball.getVel().y };
            // DISPLAY
            this->display();

            // wait appropriate time - 120FPS
            // std::cout << "display thread elapsed time: " << gLoop.elapsed() << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/120 - gLoop.elapsed()))));
            // reset timer
            gLoop.reset();
        }
    });

    // GET USER INPUT
    int x, y;
    SDL_Event event;
    Game_Info lData;
    while(!done) {
        if(!SDL_PollEvent(&event))
            continue;
        switch(event.type) {
            case (SDL_QUIT): {
                    done = true;
                    break;
                }
            case (SDL_WINDOWEVENT): {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    context->updateWindowSize(event.window.data1, event.window.data2);
                    // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
                }
                break;
            }
            case (SDL_MOUSEMOTION): {
                SDL_GetMouseState(&x, &y);
                lcl_cursor_pos.x = (1.0*x) / context->getWidth();
                lcl_cursor_pos.y = (1.0*y) / context->getHeight();
                // std::cout << "Mouse State obtained: (" << cursorPos.x << ", "<< cursorPos.y << ")\n";

                // POST LOCAL DATA TO SERVER
                lData.done = done;
                lData.racketPos = lcl_cursor_pos;
                lData.score = this->score[0];
                lData.ballPos = ballPos;
                lData.ballVel = ballVel;
                connection->SendGameInfo<Game_Info>(&lData);
                break;
            }
        }
    }

    if (server_update_thr.joinable()) server_update_thr.join();
    if (display_thr.joinable()) display_thr.join();

    return true;
}