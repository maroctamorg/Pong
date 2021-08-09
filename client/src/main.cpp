#include "main.hpp"
#include "ui/test_menu.hpp"

int main() {
    std::shared_ptr<GraphicsContext> g_context = std::make_shared<GraphicsContext>(800, 600);
    std::cout << "Succesfully initialised graphicsContext!\n";

    bool done {false};
    while(!done) {
        // ESTABLISH A CONNECTION TO THE SERVER
        std::shared_ptr<CustomClient> c { std::make_shared<CustomClient>() };
        if(!c->Connect("127.0.0.1", 3000)) {
            std::cout << "Failed to establish a connection to server! Quitting!\n";
            done = true;
            break;
        }
        if(done) break;

        // int c_counter {0};
        // while(!c->Connect("127.0.0.1", 3000) && !done) {
        //     if(c_counter > 100) {
        //         std::cout << "Failed to establish a connection to server! Quitting!\n";
        //         done = true;
        //         break;
        //     }
        //     std::cout << "Establishing connection to server..." << c_counter << "\n";
        //     c_counter++;
        // }
        // if(done) break;

        std::shared_ptr<EventHandler> handler { std::make_shared<EventHandler>() };
        std::shared_ptr<Menu> test_menu = Test_Menu::construct(g_context, handler, std::weak_ptr<CustomClient>(c));

        Event event;
        STATE state { STATE::ESTABLISHING };
        bool menu_done { false };
        int c_counter { 0 };
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
                case STATE::WAITING:
                    std::cout << "Established session with server; waiting for opponent...\n";
                    break;
                case STATE::ESTABLISHING:
                    if(c_counter > 1000) {
                        std::cout << "Failed to establish a session with the server! Please try again at another time. Quitting!\n";
                        done = true;
                        break;
                    }
                    std::cout << "Establishing session with server..." << c_counter << "\n";
                    c_counter++;
                    break;
            }
            if(menu_done || done) break;

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
        
        if(done) break;

        Game game {g_context, c};
        done = game.start();

        c->Disconnect();
    }
    return 0;
}