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
            short PORT {3000};
            
            UI::reset();
            done = UI::mainmenu(mode, username, IP, PORT);
            if(done) break;
            printf("#####################################\n\tmode: %d\n\tusername: %s\n\tIP: %s\n\tPORT: %d\n\n", mode, username.c_str(), IP.c_str(), PORT);
            fflush(stdout);
            
            Start_Game_Info server_info;

            UI::reset();
            std::shared_ptr<CustomClient> c { std::make_shared<CustomClient>() };
            done = connect_to_server(c, server_info, mode, username, IP, PORT);
            if(done) break;

            UI::reset();
            done = match(c, server_info); //synchronize start + hold match (pause overlay can be later implemented)

            // UI::reset();
            // done = UI::result(c); //display results of match, <allow replay with same user>, reset connection, send back to mainmenu or quit
        }
    } catch(...) {
        std::cerr << "Runtime exception thrown in main! Exiting...\n";
        return 1;
    }
    // quit()
    return 0;
}