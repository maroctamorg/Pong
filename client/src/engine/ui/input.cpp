#include "input.hpp"

Input::Input(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, SDL_Color color, SDL_Rect rect)
    :   UI_Element(context, rect, color), handler(handler), layout(layout) {
        // std::cout << "Input state on initialization:\nactive: " << state.active << "\tselect: " << state.select << '\n';
        if(this->layout)
            this->layout->updatePosition(this->rect);
        if(this->button) {
            this->button->select(true);
            this->button->registerCallBack([](GraphicsContext* context, EventHandler* handler, Button* button) {
                button->up
            })
        }
        if(this->handler) {
            this->handler->registerBu
        }
            
    }
Input::Input(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, SDL_Rect rect, SDL_Texture* a_texture)
    :   UI_Element(context, rect, a_texture), handler(handler), layout(layout) {
        if(this->layout)
            this->layout->updatePosition(this->rect);
        if(this->handler)
            // this->handler->registerButtonToHandler(this);
    }