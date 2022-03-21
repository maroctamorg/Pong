#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <stdlib.h>
#include <time.h>

#include "ui.hpp"

enum class MODE {
    NONE,
    P2P,
    SERVER
};

class MainMenu_Overlay final : public UI_Element {
public:
    //UI

    std::shared_ptr<Menu> main;
    std::shared_ptr<Layout> main_l;

    std::shared_ptr<Pannel> form;
    std::shared_ptr<Layout> form_l;
    
    std::shared_ptr<Input_Field> username_f;
    // std::shared_ptr<Layout> username_fl;
    std::shared_ptr<Input_Field> connectionIP_f;
    // std::shared_ptr<Layout> connectionIP_fl;
    std::shared_ptr<Input_Field> connectionPORT_f;
    // std::shared_ptr<Layout> connectionPORT_fl;

    std::shared_ptr<Button> P2P_b;
    std::shared_ptr<Layout> P2P_bl;
    std::shared_ptr<Button> server_b;
    std::shared_ptr<Layout> server_bl;

public:
    MainMenu_Overlay(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Event_Handler> e_handler, MODE& mode, std::string& username, std::string& IP, short& PORT) :
        UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()})),
        main_l { std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.2, 0.2, 0.6, 0.6)})) },
        form_l { std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.2, 0.12, 0.2, 0.1), Container(0.50, 0.12, 0.3, 0.1), Container(0.2, 0.35, 0.2, 0.1), Container(0.5, 0.35, 0.3, 0.1), Container(0.2, 0.55, 0.2, 0.1), Container(0.5, 0.55, 0.3, 0.1), Container(0.2, 0.75, 0.2, 0.1), Container(0.5, 0.75, 0.15, 0.1), Container(0.7, 0.75, 0.15, 0.1)})) },
        username_f { std::make_shared<Input_Field>(context, e_handler->overlay_input_handler, 0, UI::font, 12, SDL_Color({0, 0, 0, 255})) },
        connectionIP_f { std::make_shared<Input_Field>(context, e_handler->overlay_input_handler, 0, UI::font, 12, SDL_Color({0, 0, 0, 255})) },
        connectionPORT_f { std::make_shared<Input_Field>(context, e_handler->overlay_input_handler, 0, UI::font, 12, SDL_Color({0, 0, 0, 255})) },
        P2P_bl { std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.1, 0.05, 0.8, 0.9)})) },
        server_bl { std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.1, 0.05, 0.8, 0.9)})) }
    {
        
        P2P_b = std::make_shared<Button>(context, e_handler->overlay_input_handler, P2P_bl, 0, true, false, SDL_Color({172, 43, 12, 125}));
        server_b = std::make_shared<Button>(context, e_handler->overlay_input_handler, server_bl, 1, true, false, SDL_Color({172, 43, 12, 125}));

        P2P_bl->placeUI_Element(std::make_shared<Text_Box>(context, "P2P", UI::font, 10, UI::palette[2], ALIGN_X::LEFT, ALIGN_Y::CENTER), 0);
        P2P_b->registerCallBack([button{std::weak_ptr<Button>(P2P_b)}]() mutable {
            if(auto b = button.lock()) {
                b->setColor(SDL_Colour({12, 172, 43, 125}));
            } else {
                std::cout << "unable to lock button in P2P_b callback\n";
            }
        });
        e_handler->registerButtonEventCallback(P2P_b->getId(), [this, &mode, &username, &IP, &PORT]() mutable {
            printf("button event callback - P2P\n");
            srand(time(NULL));
            mode = MODE::P2P;
            if(std::string name = username_f->getText(); !name.empty()) username = name;
            else username = "user" + (rand() % 100000 + 1);
            if(std::string ip = connectionIP_f->getText(); !ip.empty()) IP = ip;
            if(std::string port = connectionPORT_f->getText(); !port.empty()) {
                try {
                    PORT = std::stoi(port);
                } catch (...) {
                    PORT = 3600;
                    std::cout << "Invalid PORT...\n";
                }
            }
            return DEFEvent({EVENT_TYPES::NEXT, -1, NULL});
        });

        server_bl->placeUI_Element(std::make_shared<Text_Box>(context, "server", UI::font, 10, UI::palette[2], ALIGN_X::LEFT, ALIGN_Y::CENTER), 0);
        server_b->registerCallBack([button{std::weak_ptr<Button>(server_b)}]() mutable {
            if(auto b = button.lock()) {
                b->setColor(SDL_Colour({12, 172, 43, 125}));
            } else {
                std::cout << "unable to lock button in server_b callback\n";
            }
        });
        e_handler->registerButtonEventCallback(server_b->getId(), [this, mode, username, IP, PORT]() mutable {
            printf("button event callback - server\n");
            srand(time(NULL));
            mode = MODE::SERVER;
            if(std::string name = username_f->getText(); !name.empty()) username = name;
            else username = "user" + (rand() % 100000 + 1);
            if(std::string ip = connectionIP_f->getText(); !ip.empty()) IP = ip;
            if(std::string port = connectionPORT_f->getText(); !port.empty()) PORT = std::stoi(port);
            return DEFEvent({EVENT_TYPES::NEXT, -1, NULL});
        });

        form_l->placeUI_Element(std::make_shared<Text_Box>(context, "username:", UI::font, 12, UI::palette[2]), 0);
        form_l->placeUI_Element(username_f, 1);
        form_l->placeUI_Element(std::make_shared<Text_Box>(context, "IP:", UI::font, 12, UI::palette[2]), 2);
        form_l->placeUI_Element(connectionIP_f, 3);
        form_l->placeUI_Element(std::make_shared<Text_Box>(context, "PORT:", UI::font, 12, UI::palette[2]), 4);
        form_l->placeUI_Element(connectionPORT_f, 5);
        form_l->placeUI_Element(std::make_shared<Text_Box>(context, "MODE:", UI::font, 12, UI::palette[2]), 6);
        form_l->placeUI_Element(P2P_b, 7);
        form_l->placeUI_Element(server_b, 8);

        form = std::make_shared<Pannel>(context, form_l, SDL_Color{255, 255, 255, 15}); // fix transparency

        main_l->placeUI_Element(form, 0);
        main = std::make_shared<Menu>(context, main_l, "resources/splash_background.jpg"); // fix transparency
    }

    void render() {
        main->render();
    }
};

#endif