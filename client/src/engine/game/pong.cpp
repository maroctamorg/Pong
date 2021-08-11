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
const Point Ball::b_inVel {0.001, 0};

// Ball::Ball(int v): bVel { v, 0 }, bPos { 400, 250 }, bRect { 390, 240, 20, 20 } {};

Rect& Ball::getRect() {
    return this->bRect;
}
Point& Ball::getVel() {
    return this->bVel;
}

// void Ball::setPos(double x, double y) {
//     this->bPos = { x, y };
// }
// void Ball::setVel(double x, double y) {
//     this->bVel = { x, y };
// }

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

void Paddle::move(const Point &cursorPos, bool remote) {
    
    if (cursorPos.x < 0.5 && !remote) {
        padRct.x = cursorPos.x;
    } else if (cursorPos.x > 0.5 && remote) {
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
    } else if (checkRelCollision(this->context.get(), bRect, this->rmt_goal)) {
        score[0] += 1;
        bRect = this->ball.b_inRect;
        bVel = {-this->ball.b_inVel.x, -this->ball.b_inVel.y};
    }

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
    Rect& rmt_pad_pos = this->lcl_paddle.getRect();
    // CHECK FOR HITS WITH LOCAL PADDLE
    // bool lhitX { bRect.x <= lcl_pad_pos.x + lcl_pad_pos.w && bRect.x >= lcl_pad_pos.x };
    // bool lhitY { bRect.y <= lcl_pad_pos.y + lcl_pad_pos.h && bRect.y >= lcl_pad_pos.y };
    // // CHECK FOR HITS WITH REMOTE PADDLE
    // bool rhitX = bRect.x <= rmt_pad_pos.x + rmt_pad_pos.w && bRect.x >= rmt_pad_pos.x;
    // bool rhitY = bRect.y <= rmt_pad_pos.y + rmt_pad_pos.h && bRect.y >= rmt_pad_pos.y;

    // bool hit = (lhitX && lhitY) || (rhitX && rhitY);
    // bool hit = checkRelCollision(this->context.get(), bRect, lcl_pad_pos) || checkRelCollision(this->context.get(), bRect, rmt_pad_pos);

    // auto handlePaddleCollision {
    //     [bRect, bVel](const Rect& paddle_pos) mutable {
    //         if(bRect.x < paddle_pos.x + static_cast<int>(paddle_pos.w/2)) {
    //             bRect.x = paddle_pos.x;
    //             bVel.x = bVel.x > 0 ? -bVel.x : bVel.x;
    //         } else {
    //             bRect.x = paddle_pos.x + paddle_pos.w;
    //             bVel.x = bVel.x > 0 ? bVel.x : -bVel.x;
    //         }
    //         if (paddle_pos.y <= 0.5) {
    //             bVel.y = bVel.y > 0 ? bVel.y : -bVel.y;
    //         } else if (paddle_pos.y > 0.5) {
    //             bVel.y = bVel.y > 0 ? -bVel.y : bVel.y;
    //         }
    //     }
    // };

    // if (checkRelCollision(this->context.get(), bRect, lcl_pad_pos))
    //     handlePaddleCollision(lcl_pad_pos);
    // else if (checkRelCollision(this->context.get(), bRect, rmt_pad_pos)) {
    //     handlePaddleCollision(rmt_pad_pos);
    // }

    if (checkRelCollision(this->context.get(), bRect, lcl_pad_pos)) {
        if(bRect.x < lcl_pad_pos.x + static_cast<int>(lcl_pad_pos.w/2)) {
            bRect.x = lcl_pad_pos.x;
            bVel.x = bVel.x > 0 ? -bVel.x : bVel.x;
        } else {
            bRect.x = lcl_pad_pos.x + lcl_pad_pos.w;
            bVel.x = bVel.x > 0 ? bVel.x : -bVel.x;
        }
        if (lcl_pad_pos.y <= 0.5) {
            bVel.y = bVel.y > 0 ? bVel.y : -bVel.y;
        } else if (lcl_pad_pos.y > 0.5) {
            bVel.y = bVel.y > 0 ? -bVel.y : bVel.y;
        }
    } else if (checkRelCollision(this->context.get(), bRect, rmt_pad_pos)) {
        if(bRect.x < rmt_pad_pos.x + static_cast<int>(rmt_pad_pos.w/2)) {
            bRect.x = rmt_pad_pos.x;
            bVel.x = bVel.x > 0 ? -bVel.x : bVel.x;
        } else {
            bRect.x = rmt_pad_pos.x + rmt_pad_pos.w;
            bVel.x = bVel.x > 0 ? bVel.x : -bVel.x;
        }
        if (rmt_pad_pos.y <= 0.5) {
            bVel.y = bVel.y > 0 ? bVel.y : -bVel.y;
        } else if (rmt_pad_pos.y > 0.5) {
            bVel.y = bVel.y > 0 ? -bVel.y : bVel.y;
        }
    }
}

void Game::update(Point lcl_pos, Point rmt_pos) {
    lcl_paddle.move(lcl_pos, false);
    rmt_paddle.move(rmt_pos, true);
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
    SDL_RenderFillRect(this->context->renderer, &l_goal); // !!!!!!!!!!
    SDL_SetRenderDrawColor(this->context->renderer, 250, 150, 50, 250);
    SDL_Rect r_goal = rmt_goal*window_rect;
    SDL_RenderFillRect(this->context->renderer, &r_goal); // !!!!!!!!!!
    
    ball.display(this->context->renderer, window_rect);
    lcl_paddle.display(this->context->renderer, window_rect, false);
    rmt_paddle.display(this->context->renderer, window_rect, true);

    SDL_RenderPresent(this->context->renderer);
}

bool Game::start() {
    int counter {-1};
    SDL_Event event;
    Point lcl_cursor_pos { 0.5, 0.5 };
    Point rmt_cursor_pos { 0.5, 0.5 };
    JSON sData;
    JSON lData;
    STATE state { STATE::START };
    Timer gLoop;
    bool done {false};

    while(!done) {
        gLoop.reset();
        counter++;
        if(SDL_PollEvent(&event)) {
            switch(event.type) {
                case (SDL_QUIT): {
                        done = true;
                        return true;
                        break;
                    }
                case (SDL_WINDOWEVENT): {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        this->context->updateWindowSize(event.window.data1, event.window.data2);
                        // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
                    }
                    break;
                }
            }
        }
        int x, y;
        SDL_GetMouseState(&x, &y);
        lcl_cursor_pos.x = (1.0*x) / this->context->getWidth();
        lcl_cursor_pos.y = (1.0*y) / this->context->getHeight();
        // std::cout << "Mouse State obtained: (" << cursorPos.x << ", "<< cursorPos.y << ")\n";
        state = this->connection->handleIncoming(&sData);
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
                return false;
                break;
            case (STATE::GAME_INFO) :
                std::cout << "Received game info from server: " << sData << '\n';
                rmt_cursor_pos = { sData["racket"]["x"], sData["racket"]["y"] };
                counter = 0;
                break;
            default :
                std::cout << "Unhandled server response!\n";
                counter = 0;
        }

        // UPDATE
        this->update(lcl_cursor_pos, rmt_cursor_pos);
        
        // POST LOCAL DATA TO SERVER
        lData["done"] = done;
        // lData["racket"]= this->lcl_paddle.getPos();
        lData["racket"]["x"] = this->lcl_paddle.getPos().x;
        lData["racket"]["y"] = this->lcl_paddle.getPos().y;
        // this->connection->SendGameInfo(&lData);

        // DISPLAY
        this->display();

        // STABLE FRAMES
        // double elapsed = gLoop.elapsed();
        // std::cout << "Game Loop Length:\t" << elapsed << '\n';
        // wait appropriate time - 60FPS
        sleep(1000*static_cast<int>(1/60 - gLoop.elapsed() > 0 ? 1/60 - gLoop.elapsed() : 0));
    }
    return true;
}