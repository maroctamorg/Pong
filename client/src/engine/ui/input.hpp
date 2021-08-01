#ifndef INPUT_HPP
#define INPUT_HPP

#include "extern.hpp"

class Input : public UI_Element {
private:
    std::unique_ptr<Layout> layout;

public:
    Input(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, TextBox* text, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    Input(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, TextBox* text, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Texture* a_texture = nullptr);
    ~Input();

public:
    std::string getInput();

};

#endif