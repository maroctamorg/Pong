#include "test_menu.hpp"

const std::unique_ptr<Menu> Test_Menu(std::shared_ptr<GraphicsContext> context) {
    Layout* layout = new Layout(context, {Container(0.25, 0.25, 0.5, 0.5)});
    // layout.placeUI_Element(std::make_unique<Pannel>(context, nullptr));
    layout->placeUI_Element(new Pannel(context, nullptr, SDL_Color({24, 43, 100, 255})), 0);
    return std::make_unique<Menu>(context, layout, SDL_Color({255, 255, 255, 255}));
}