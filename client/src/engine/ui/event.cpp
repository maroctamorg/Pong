#include "event.hpp"

void EventHandler::registerButtonToHandler(Button* button) {
    if(button)
        buttons.push_back(button);
    // std::cout << "Registered button to Handler! Id: " << this->buttons.back()->getId() << "\n";
}

Event EventHandler::PollEvent() {
    // remove previous button press
    b_pressed = nullptr;

    SDL_Point cursor_pos {-100, -100};
    SDL_PollEvent(&(event.sdl_event));
    if(event.sdl_event.type == SDL_MOUSEBUTTONDOWN) {
        // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
        SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
        // std::cout << "Mouse State obtained:\t" << cursor_pos.x << ",\t" << cursor_pos.y << "\n";
        for (int i{0}; i < buttons.size(); i++) {
            if (buttons.at(i)->isActive() && buttons.at(i)->Clicked(cursor_pos)) {
                // std::cout << "Pressing button!\n";
                b_pressed = buttons.at(i)->press();
            }
        }
    }

    if(!b_pressed) {
        event.button_id = -1;
        event.type = EVENT_TYPES::UNHANDLED_SDL_EVENT;
    }
    else {
        event.button_id = b_pressed->getId();
        event.type = EVENT_TYPES::BUTTON_PRESS;
    }
    // int index = getCallbackIndexById(id);
    // if(index > 0)
    //     callbacks.at(i).execute();
    return this->event;

}

// std::vector<Button*> EventHandler::getSelectedButtons() {
//     return b_pressed;
// }
std::vector<Button*> EventHandler::getSelectedButtons() {
    std::vector<Button*> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button);
    return selected;
}

// std::vector<int> EventHandler::getSelectedButtonIds() {
//     std::vector<int> ids;
//     for (const auto& button : b_pressed)
//         ids.push_back(button->id);
//     return ids;
// }
std::vector<int> EventHandler::getSelectedButtonIds() {
    std::vector<int> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button->getId());
    return selected;
}

// void EventHandler::addButtonToSelected(const Button* button) {
//     b_pressed.push_back(button);
// }

// void EventHandler::removeButtonFromSelected(const Button* button) {
//     int index {-1};
//     for (int i{0}; i < b_pressed.size(); i++) {
//         if(b_pressed.at(i) == button)
//             index = i;
//     }
//     if(index > 0)
//         b_pressed.erase(index);
// }

void EventHandler::removeButtonFromHandler(int id) {
    // std::cout << "Call to removeButtonFromHandler!\n";
    int i = this->getButtonIndexById(id);
    buttons.at(i) = nullptr;
    this->dropElementFromVector(i);
}