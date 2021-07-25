#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include "ui_engine.hpp"

class TextBox final : public UI_Element {
private:
    Text text;

public:
    void display (SDL_Renderer* renderer) override {
        text.displayText(renderer);
        return;
    }
};

#endif