#include "main.hpp"

int main() {

    Timer init;

    if (!Init_SDL()) {
        std::cout << "Failure to initalise SDL window or renderer.\n";
        return 0;
    }
    std::cout << "Succesfully initialised SDL window and renderer.\n";

    bool done { false };

    //const WebSocket::Connection = WS_Server::Establish_Connection();

    Paddle lcl_Paddle;
    Paddle rmt_Paddle;
    Ball ball(sData["v"]);

    SDL_Rect lcl_goal { 0, 100, 5, 300 };
    SDL_Rect rmt_goal { 795, 100, 5, 300 };
    SDL_Point cursorPos { 0, 0 };
    SDL_Event event;

    std::cout << "Initialisation time:\t" << init.elapsed() << '\n';

    while(!done){
        Timer gLoop;

        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 250);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
        SDL_RenderDrawLine(renderer, 400, 0, 400, 500);
        //add moving line functionality

        SDL_SetRenderDrawColor(renderer, 50, 150, 250, 250);
        SDL_RenderFillRect(renderer, &lcl_goal);
        SDL_SetRenderDrawColor(renderer, 250, 150, 50, 250);
        SDL_RenderFillRect(renderer, &rmt_goal);

        std::cout << "Time in gameLoop until HTTP requests:\t" << gLoop.elapsed() << '\n';

        Timer get;

        //GET SERVER DATA INTO SDATA
        
        JSON lData;
        lData["rmt_cPos"]["x"] = sData["rmt_cPos"]["x"];
        lData["rmt_cPos"]["y"] = sData["rmt_cPos"]["y"];

        SDL_Point rmt_cursorPos { sData["rmt_cPos"]["x"], sData["rmt_cPos"]["y"] };

        SDL_GetMouseState(&cursorPos.x, &cursorPos.y);
        std::cout << "Mouse State obtained: (" << cursorPos.x << ", "<< cursorPos.y << ")\n";

        lcl_Paddle.move(cursorPos, false);
        rmt_Paddle.move(rmt_cursorPos, true);
        ball.move();
        ball.checkCollision(lcl_Paddle.getPos(), true);
        ball.checkCollision(rmt_Paddle.getPos(), false);

        lcl_Paddle.display(renderer, false);
        rmt_Paddle.display(renderer, true);
        ball.display(renderer);

        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
            done = true;
        }

        lData["done"] = done;
        lData["lcl_cPos"]["x"] = lcl_Paddle.getPos().x;
        lData["lcl_cPos"]["y"] = lcl_Paddle.getPos().y;

        //POST LOCAL DATA TO SERVER
        
        std::cout << "Game Loop Length:\t" << gLoop.elapsed() << '\n';

    }

    Quit_SDL();

    return 0;
}