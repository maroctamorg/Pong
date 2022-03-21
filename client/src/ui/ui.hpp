#ifndef UI_HPP
#define UI_HPP

#include "Tsukuyomi.hpp"


namespace UI {
    SDL_Color palette[3] {SDL_Color{68, 72, 87, 255}, SDL_Color{122, 163, 152, 255}, SDL_Color{207, 198, 169, 255}};
    const std::string font {"resources/fonts/CourierNew.ttf"};
    int W_W {800};
    int W_H {600};

    std::shared_ptr<UI_Handler>         handler   { nullptr };
    std::shared_ptr<Graphics_Context>   g_context {nullptr};
    std::shared_ptr<Animation_Handler>  a_handler {nullptr};
    std::shared_ptr<Event_Handler>      e_handler {nullptr};
};

#include "splash.hpp"
#include "mainmenu.hpp"
#include "connect_screen.hpp"

#include "../services/client.hpp"

typedef struct {
    double x;
    double y;
} Start_Game_Info;

namespace UI {
    bool init() {
        try {
            //define UI_Handler
            handler = std::make_unique<UI_Handler>(W_W, W_H, "Pong++");

            std::cout << "Defined UI_Handler...\n";

            //get default context and handlers
            g_context = handler->getGraphicsContext();
            a_handler = handler->getAnimationHandler();
            e_handler = handler->getEventHandler();

            std::cout << "Obtained default context and handlers...\n";
        } catch (...) {
            return false;
        }
        return true;
    }

    void reset() {
        handler->reset();
        a_handler = handler->getAnimationHandler();
        e_handler = handler->getEventHandler();
    }

    bool splash() {
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

    bool mainmenu(MODE& mode, std::string& username, std::string& IP, short& PORT) {
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
};

#endif