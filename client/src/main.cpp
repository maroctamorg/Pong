#include "main.hpp"
#include "ui/test_menu.hpp"

typedef struct {
    double x;
    double y;
} Start_Game_Info;

int main() {
    std::shared_ptr<Graphics_Context> g_context = std::make_shared<Graphics_Context>(800, 600);
    std::cout << "Succesfully initialised Graphics_Context!\n";

    bool done {false};
    while(!done) {
        // ESTABLISH A CONNECTION TO THE SERVER
        std::shared_ptr<CustomClient> c { std::make_shared<CustomClient>() };
        c->Connect("127.0.0.1", 3000);
        while(!c->IsConnected()) {
            std::cout << "Establishing a connection to server...\n";
        }

        std::shared_ptr<Event_Handler> handler { std::make_shared<Event_Handler>() };
        std::shared_ptr<Menu> test_menu = Test_Menu::construct(g_context, handler, std::weak_ptr<CustomClient>(c));

        Event_t event;
        STATE state { STATE::ESTABLISHING };
        Start_Game_Info server_info;
        bool menu_done { false };
        int c_counter { 0 };
        while(!menu_done) {
            state = c->handleIncoming(&server_info);
            switch(state) {
                case STATE::END:
                    std::cout << "Disconnected from server!\n";
                    Test_Menu::text->updateText("Disconnected from server! Quitting...");
                    done = true;
                    break;
                case STATE::START:
                    std::cout << "Starting match with ball velocity:\tx: "<< server_info.x << "\n";
                    Test_Menu::text->updateText("Starting match...");
                    menu_done = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                    break;
                case STATE::WAITING:
                    std::cout << "Established session with server; waiting for opponent...\n";
                    Test_Menu::text->updateText("Established session with server; waiting for opponent...");
                    break;
                case STATE::ESTABLISHING:
                    if(c_counter > 1000) {
                        std::cout << "Failed to establish a session with the server! Please try again at another time. Quitting...\n";
                        Test_Menu::text->updateText("Failed to establish a session with the server! Please try again at another time. Quitting...");
                        done = true;
                        break;
                    }
                    std::cout << "Establishing session with server..." << c_counter << "\n";
                    Test_Menu::text->updateText("Establishing session with server...");
                    c_counter++;
                    break;
            }
            if(menu_done || done) break;

            test_menu->render();
            g_context->display();

            // test_menu->update();
            // SDL_PollEvent(&event);
            event = handler->pollEvent();
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
        if(done) break;

        Game game {g_context, c, {server_info.x, server_info.y}};
        done = game.start();

        c->Disconnect();
    }
    return 0;
}