#include "main.hpp"

bool splash(UI_Handler* handler, std::shared_ptr<Graphics_Context> g_context, std::shared_ptr<Event_Handler> e_handler) {
    //construct Main and Overlay objects
    std::shared_ptr<Splash_Background> sp_background = std::make_shared<Splash_Background>(g_context);
    std::shared_ptr<Splash_Overlay> sp_overlay = std::make_shared<Splash_Overlay>(g_context);

    //setup Main and Overlay
    std::cout << "Setting splash background and overlay...\n";
    handler->setMain(sp_background);
    handler->setOverlay(sp_overlay);

    //setup event-handling and callbacks
    std::cout << "Setting up event-handling and callbacks...\n";
    
    // * make fading splash screen animation * //

    // uint hideshow_a_id = a_handler->add(std::make_unique<Overlay_HideShow_Animation>(overlay, handler->getLayout(), 1));
    // e_handler->registerKeyCallback([a_handler, hideshow_a_id](SDL_Keycode keycode){
    //     if(keycode == SDLK_TAB) a_handler->start(hideshow_a_id);
    // });

    //probably need to re-refactor...
    e_handler->registerSDLEventCallback(SDL_KEYDOWN, []() {
        return DEFEvent({EVENT_TYPES::NEXT, -1, NULL});
    });

    return handler->main();
}

bool mainmenu(UI_Handler* handler, std::shared_ptr<Graphics_Context> g_context, std::shared_ptr<Event_Handler> e_handler, MODE& mode, std::string& username, std::string& IP, short& PORT) {
    //construct Main and Overlay objects
    std::shared_ptr<Splash_Background> background = std::make_shared<Splash_Background>(g_context);
    std::shared_ptr<MainMenu_Overlay> mainmenu_overlay = std::make_shared<MainMenu_Overlay>(g_context, e_handler, mode, username, IP, PORT);

    //setup Main and Overlay
    std::cout << "Setting main background and overlay...\n";
    handler->setMain(background);
    handler->setOverlay(mainmenu_overlay);

    //setup event-handling and callbacks
    std::cout << "Setting up event-handling and callbacks...\n";

    return handler->main();
}

int main() {
//INITIALIZE UI_HANDLER

    //define UI_Handler
    auto handler { std::make_unique<UI_Handler>(UI::W_W, UI::W_H, "Pong++") };

    std::cout << "Defined UI_Handler...\n";

    //get default context and handlers
    std::shared_ptr<Graphics_Context> g_context = handler->getGraphicsContext();
    std::shared_ptr<Animation_Handler> a_handler = handler->getAnimationHandler();
    std::shared_ptr<Event_Handler> e_handler = handler->getEventHandler();

    std::cout << "Obtained default context and handlers...\n";  

    bool done { false };
    try {
        while(!done) {
            done = splash(handler.get(), g_context, e_handler);
            if(done) break;
            e_handler->deRegisterSDLEventCallback(SDL_KEYDOWN);
            
            handler->reset();
            a_handler = handler->getAnimationHandler();
            e_handler = handler->getEventHandler();

            MODE mode {MODE::NONE};
            std::string username {"user0000"};
            std::string IP {"127.0.0.1"};
            short PORT {3600};

            done = mainmenu(handler.get(), g_context, e_handler, mode, username, IP, PORT);
            if(done) break;
            printf("###################obtained:##################\n\tmode: %d\n\tusername: %s\n\tIP: %s\n\tPORT: %d\n\n", mode, username.c_str(), IP.c_str(), PORT);
            fflush(stdout);
            
            // initMatch();
            // done = match(connection);
            // initResult();
            // done = result();
        }
    } catch(...) {
        std::cerr << "Runtime exception thrown in main! Exiting...\n";
    }
    // quit()
    return 0;
}