#include "main.hpp"
#include "ui/test_menu.hpp"

enum class STATE {
    END = -1,
    ESTABLISHING = 0,
    START = 1,
    GAME_INFO = 2,
};

int main() {
    std::shared_ptr<GraphicsContext> g_context = std::make_shared<GraphicsContext>(800, 600);
    std::cout << "Succesfully initialised graphicsContext!\n";

    bool done {false};
    while(!done) {
        // ESTABLISH A CONNECTION TO THE SERVER
        std::shared_ptr<CustomClient> c { std::make_shared<CustomClient>() };
        int c_counter {0};
        while(!c->Connect("127.0.0.1", 3000)) {
            if(c_counter > 100) {
                std::cout << "Failed to establish a connection to server! Quitting!\n";
                done = true;
                break;
            }
            std::cout << "Establishing connection to server...\n";
            c_counter++;
        }
        if(done) break;

        std::shared_ptr<EventHandler> handler { std::make_shared<EventHandler>() };
        std::shared_ptr<Menu> test_menu = Test_Menu::construct(g_context, handler, std::weak_ptr<CustomClient>(c));

        Event event;
        STATE state { STATE::ESTABLISHING };
        menu_done { false };
        while(!menu_done) {
            state = c->handleIncoming(nullptr);
            switch(state) {
                case STATE::END:
                    std::cout << "Disconnected from server!\n";
                    done = true;
                    break;
                case STATE::START:
                    std::cout << "Starting match...\n";
                    menu_done = true;
                    break;
                case STATE::ESTABLISHING:
                    std::cout << "Establish session with server...\n";
                    break;
            }

            test_menu->render();
            g_context->display();

            // test_menu->update();
            // SDL_PollEvent(&event);
            event = handler->PollEvent();
            if(event.type == EVENT_TYPES::UNHANDLED_SDL_EVENT) {
                switch (event.sdl_event.type) {
                    case (SDL_QUIT): {
                        done = true;
                        break;
                    }
                    case (SDL_WINDOWEVENT): {
                        if(event.sdl_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            g_context->updateWindowSize(event.sdl_event.window.data1, event.sdl_event.window.data2);
                            test_menu->updateSize();
                            // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
                        }
                        break;
                    }
                }
            }
        }

        JSON lData;
        JSON sData;
        
        while(c->handleIncoming(&sData) != STATE::GAME_INFO)
            std::cout << "Retrieving initial information from server...\n";

        Paddle lcl_Paddle;
        Paddle rmt_Paddle;
        Ball ball(sData["v"]);

        SDL_Rect lcl_goal { 0, 100, 5, 300 };
        SDL_Rect rmt_goal { 795, 100, 5, 300 };
        SDL_Point cursorPos { 0, 0 };
        SDL_Event event;

        bool game_done { false };
        while(!game_done){
            Timer gLoop;

            state = c->handleIncoming(&sData);
            switch(state) {
                case STATE::END:
                    std::cout << "Disconnected from server!\n";
                    done = true;
                    break;
                case STATE::START:
                    std::cout << "Starting match...\n";
                    // menu_done = true;
                    break;
                case STATE::ESTABLISHING:
                    std::cout << "Establish session with server...\n";
                    break;
                case STATE::GAME_INFO:
                    std::cout << "Retrieved info from server: " << sData << '\n';
                    break;
            }

            SDL_SetRenderDrawColor(renderer, 250, 250, 250, 250);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
            SDL_RenderDrawLine(renderer, 400, 0, 400, 500);
            //add moving line functionality

            SDL_SetRenderDrawColor(renderer, 50, 150, 250, 250);
            SDL_RenderFillRect(renderer, &lcl_goal);
            SDL_SetRenderDrawColor(renderer, 250, 150, 50, 250);
            SDL_RenderFillRect(renderer, &rmt_goal);
            
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
                game_done = true;
            }

            lData["done"] = game_done;
            lData["lcl_cPos"]["x"] = lcl_Paddle.getPos().x;
            lData["lcl_cPos"]["y"] = lcl_Paddle.getPos().y;

            //POST LOCAL DATA TO SERVER
            c->SendGameInfo(&lData);
            
            std::cout << "Game Loop Length:\t" << gLoop.elapsed() << '\n';
        }
    }
    return 0;
}