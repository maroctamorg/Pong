#ifndef CONNECT_SCREEN_HPP
#define CONNECT_SCREEN_HPP

#include "ui.hpp"

class Connect_Overlay final : public UI_Element {
private:
    bool& update_f;
    std::string& message;
    
public:
    std::shared_ptr<Layout> main_layout;
    std::shared_ptr<Text_Box> title_txt;
    std::shared_ptr<Menu> main;

public:
    void render() override {
        main->render();
    }

    void update() override {
        if(!update_f) return;
        std::cout << message << std::flush;
        title_txt->updateText(message);
        main_layout->placeUI_Element(title_txt, 0);
        update_f = false;
    }

public:
    Connect_Overlay(const std::shared_ptr<Graphics_Context> context, std::string& message, bool& update)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()})),
            main_layout { new Layout(context, {Container(0.25, 0.4, 0.5, 0.1)}) },
            message     { message },
            update_f    { update }
        {
            title_txt = std::make_shared<Text_Box>(context, message, UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER, UI::palette[0]);
            main_layout->placeUI_Element(title_txt, 0);
            main = std::make_shared<Menu>(context, main_layout, SDL_Color{255, 255, 255, 0}); // fix transparency
        }
};

#endif