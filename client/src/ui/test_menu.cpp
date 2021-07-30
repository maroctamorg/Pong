#include "test_menu.hpp"

const std::unique_ptr<Menu> Test_Menu(std::shared_ptr<GraphicsContext> context) {
    Layout* main_layout = new Layout(context, {Container(0.1, 0.1, 0.8, 0.8)});
    Layout* pannel_layout = new Layout(context, {Container(0.25, 0.05, 0.5, 0.05), Container(0.25, 0.15, 0.5, 0.10), Container(0.25, 0.30, 0.5, 0.40)});
    // Layout* pannel_layout = new Layout(context, {Container(0.25, 0.05, 0.5, 0.5), Container(0.25, 0.25, 0.5, 0.5)});
    std::string medium = "This is some medium-length text that is meant to test things out!";
    std::string large = "What follows is a long piece of text, meant to test out the TextBox class' adaptability functionality. Though this does not say much, it contains many words and occupies much space. The sophistic philosophers of Ancient Greece would most certainly be proud. I think this should be enough for the purposes of this text. If this is not the case, I will make sure to edit this so that the functionality is thoroughly tested.";
    // std::string font = "/Users/magneto/Programming/C++/Game_Development/Pong++/client/resources/fonts/CourierNew.ttf";
    std::string font = "resources/fonts/CourierNew.ttf";
    // std::string font = "CarrierNew.ttf";
    SDL_Color font_color { 255, 255, 255, 255 };
    pannel_layout->placeUI_Element(new TextBox(context, "Short Text", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    pannel_layout->placeUI_Element(new TextBox(context, medium, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 1);
    pannel_layout->placeUI_Element(new TextBox(context, large, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 2);
    main_layout->placeUI_Element(new Pannel(context, pannel_layout, SDL_Color({24, 43, 100, 255})), 0);
    // main_layout->placeUI_Element(new Pannel(context, nullptr, SDL_Color({24, 43, 100, 255})), 0);
    return std::make_unique<Menu>(context, main_layout, SDL_Color({255, 255, 255, 255}));
}