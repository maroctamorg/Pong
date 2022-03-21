#ifndef SPLASH_HPP
#define SPLASH_HPP

#include "ui.hpp"

class Splash_Background final : public UI_Element {
public:
    Splash_Background(const std::shared_ptr<Graphics_Context> context)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()}), "resources/splash_background.jpg") {}
    ~Splash_Background() = default;

    void render() override {
        UI_Element::render();
    }
};

class Splash_Overlay final : public UI_Element {
public:
    std::shared_ptr<Layout> main_layout;
    std::shared_ptr<Text_Box> title_txt;
    std::shared_ptr<Menu> main;

public:
    void render() override {
        main->render();
    }

public:
    Splash_Overlay(const std::shared_ptr<Graphics_Context> context)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()})),
            main_layout { new Layout(context, {Container(0.25, 0.4, 0.5, 0.1)}) },
            title_txt { std::make_shared<Text_Box>(context, "Press any key to start...", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER, UI::palette[0]) }
        {
            main_layout->placeUI_Element(title_txt, 0);
            main = std::make_shared<Menu>(context, main_layout, SDL_Color{255, 255, 255, 0}); // fix transparency
        }
};

#endif