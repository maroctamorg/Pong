#ifndef UI_ENGINE_HPP
#define UI_ENGINE_HPP

#include "../graphics/graphics_engine.hpp"

enum GAME_STATE
{
    NOINPUT,
    UPDATE,
    TERMINATE,
    ANIMATION,
};

class UI_Element {
protected:
    const GraphicsContext* context { nullptr };

    bool hidden { false };
    int r { 0 };
    SDL_Rect rect {0, 0, 0, 0};
    SDL_Colour color {0, 0, 0, 0};
    SDL_Colour border_color {0, 0, 0, 0};

public:
    UI_Element(const GraphicsContext* graphics_context, int x, int y, int w, int h, const SDL_Colour color = SDL_Colour({0, 0, 0, 0}), const SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
        : context(graphics_context), rect{x, y, w, h}, color {color.r, color.g, color.b, color.a} {}
    UI_Element(const GraphicsContext* graphics_context, const SDL_Rect& rect, const SDL_Colour color = SDL_Colour({0, 0, 0, 0}), const SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
        : context(graphics_context), rect{rect.x, rect.y, rect.w, rect.h}, color {color.r, color.g, color.b, color.a} {}
    virtual ~UI_Element() = default;

public:
    virtual void display(SDL_Renderer* renderer) = 0;

    void setHidden(bool hidden) { this->hidden = hidden; }
    void setCurveRadius(int r) { this->r = r; }
    void setSpace(int x, int y, int w, int h) { this->rect = SDL_Rect({x, y, w, h}); }
    void setSpace(const SDL_Rect& rect) { this->rect = rect; }
    void setColor(const SDL_Colour &color) { this->color = color; }
    void setBorderColor(const SDL_Colour &color) { this->border_color = color; }

    SDL_Rect& getSpace() { return rect; };
};

void UI_Element::display(SDL_Renderer* renderer) {
    if(color.a == 0 || hidden)
        return;

    if(r < 1) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &(this->rect));

        if(border_color.a != 0) {
            SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
            SDL_RenderDrawRect(renderer, &(this->rect));
        }

        return;
    }

    drawRoundedRect(renderer, this->rect, this->color, this->border_color, r);
}

#include "text.hpp"
#include "button.hpp"
#include "input.hpp"
#include "layout.hpp"
#include "pannel.hpp"
#include "background.hpp"
#include "menu.hpp"
#include "animations.hpp"

#endif