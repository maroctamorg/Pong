#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include "extern.hpp"
#include "ui_element.hpp"

class TextBox final : public UI_Element {
private:
    Text text;

public:
    void render() override {
        text.displayText(context->renderer);
        return;
    }
};

#endif