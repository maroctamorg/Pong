#include "main.hpp"

int main() {

    // init connection object

    if(!UI::init()) {
        std::cout << "Failed to initialize UI... exiting!\n";
        return 1;
    }; // initialize UI

    bool done { false };
    try {
        done = UI::splash();
        UI::e_handler->deRegisterSDLEventCallback(SDL_KEYDOWN);
        
        while(!done) {            
            MODE mode {MODE::NONE};
            std::string username {"user0000"};
            std::string IP {"127.0.0.1"};
            short PORT {3600};
            
            UI::reset();
            done = UI::mainmenu(mode, username, IP, PORT);
            if(done) break;
            printf("###################obtained:##################\n\tmode: %d\n\tusername: %s\n\tIP: %s\n\tPORT: %d\n\n", mode, username.c_str(), IP.c_str(), PORT);
            fflush(stdout);
            
            std::string message {"Establishing a connection to server...\n"};
            UI::reset();
            std::thread connection_screen {
                [&done, &message](){
                    done = UI::connect(message); // how do I tell the UI to update the text? (just write it into update, might need mutex to ensure thread safety)
                    // can simply not call default main and instead render, pollEvent and update directly
                }
            };
            // what is above and below should go into connect (not necessarily into the UI_Handler)

            // ESTABLISH A CONNECTION TO THE SERVER : DO THIS BEFORE STARTING OR UPDATE MESSAGE TO ESTABLISHING SESSION
            std::shared_ptr<CustomClient> c { std::make_shared<CustomClient>() };
            c->Connect(IP, PORT);
            short count {0};
            while(!c->IsConnected() && count < 5) {
                std::cout << message << std::flush;
                std::this_thread::sleep_for(2000ms);
                count++;
            } // attempts to establish server connection every 2s, 5 times
            if(!c->IsConnected()) {
                message = "Unable to establish a connection to server...";
                std::cout << message << std::flush;
                if(connection_screen.joinable()) connection_screen.join();
            } else {
                // establish session


                // join connection screen
                if(connection_screen.joinable()) connection_screen.join();
            }

            UI::reset();
            done = UI::match(c); //synchronize start + hold match (pause overlay can be later implemented)

            UI::reset();
            done = UI::result(c); //display results of match, <allow replay with same user>, reset connection, send back to mainmenu or quit
        }
    } catch(...) {
        std::cerr << "Runtime exception thrown in main! Exiting...\n";
        return 1;
    }
    // quit()
    return 0;
}