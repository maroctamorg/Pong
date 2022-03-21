#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using namespace std::chrono;

#include "services/client.hpp"
#include "ui/ui.hpp"
#include "game/pong.hpp"

bool connect_to_server(std::shared_ptr<CustomClient> c, Start_Game_Info& server_info, MODE mode, const std::string& username, const std::string& IP, short PORT) {
    std::string message {"Establishing a connection to server...\n"};
    bool update { false };

    //construct Main and Overlay objects
    std::shared_ptr<Splash_Background> background = std::make_shared<Splash_Background>(UI::g_context);
    std::shared_ptr<Connect_Overlay> overlay = std::make_shared<Connect_Overlay>(UI::g_context, message, update);

    //setup Main and Overlay
    std::cout << "Setting splash background and overlay...\n";
    UI::handler->setMain(background);
    UI::handler->setOverlay(overlay);

    //setup event-handling and callbacks
    std::cout << "Setting up event-handling and callbacks...\n";
    
    // * make three dots connecting animation * //

    // uint hideshow_a_id = a_handler->add(std::make_unique<Overlay_HideShow_Animation>(overlay, UI::handler->getLayout(), 1));
    // UI::e_handler->registerKeyCallback([a_handler, hideshow_a_id](SDL_Keycode keycode){
    //     if(keycode == SDLK_TAB) a_handler->start(hideshow_a_id);
    // });

    //probably need to re-refactor...
    UI::e_handler->registerKeyCallback(SDLK_ESCAPE, []() {
        return DEFEvent({EVENT_TYPES::NEXT, -1, NULL});
    });

    bool done { false };
    bool next { false };
    std::thread connection_thr {
        [&done, &next, &message, &update, &server_info, c, IP, PORT]() mutable {
            // ESTABLISH A CONNECTION TO THE SERVER : LATER UPDATE MESSAGE TO ESTABLISHING SESSION
            c->Connect(IP, PORT);
            short count {0};
            while(!c->IsConnected() && count < 5) {
                std::cout << message << std::flush;
                std::this_thread::sleep_for(2000ms);
                count++;
            } // attempts to establish server connection every 2s, 5 times
            if(!c->IsConnected()) {
                message = "Unable to establish a connection to server...";
                update = true;
                std::cout << message << std::flush;
                done = true;
                return;
            } else {
                // establish session
                message = "Establishing a session with the server...";
                update = true; // write update function: if true : update txt + set update to false + print message to console

                STATE state { STATE::ESTABLISHING };

                bool done_connecting { false };
                short c_counter {0};
                while(!next && !done) {
                    state = c->handleIncoming(&server_info);
                    switch(state) {
                        case STATE::END:
                            message = "Disconnected from server! Quitting...";
                            update = true;
                            // Test_Menu::text->updateText("Disconnected from server! Quitting...");
                            done = true;
                            break;
                        case STATE::START:
                            message = "Starting match";
                            update = true;
                            // Test_Menu::text->updateText("Starting match...");
                            next = true;
                            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                            break;
                        case STATE::WAITING:
                            message = "Established session with server; waiting for opponent...";
                            update = true;
                            // Test_Menu::text->updateText("Established session with server; waiting for opponent...");
                            break;
                        case STATE::ESTABLISHING:
                            if(c_counter > 1000) {
                                message = "Failed to establish a session with the server! Please try again at another time. Quitting...";
                                update = true;
                                // Test_Menu::text->updateText("Failed to establish a session with the server! Please try again at another time. Quitting...");
                                done = true;
                                break;
                            }
                            message = "Establishing session with server..." + c_counter;
                            update = true;
                            // Test_Menu::text->updateText("Establishing session with server...");
                            c_counter++;
                            break;
                    }
                }
            }
        }
    };

    while(!done) {
        UI::handler->main_iter(done, next);
    }
    
    // join connection screen
    if(connection_thr.joinable()) connection_thr.join();
    return !next;
}

bool match(std::shared_ptr<CustomClient> c, Start_Game_Info server_info) {
    Game game {UI::g_context, c, {server_info.x, server_info.y}};
    return game.start();
}

#endif