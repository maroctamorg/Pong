#include "extern.hpp"
#include "button.hpp"

// typedef struct {
//     unsigned int id,
//     std::function<void(GraphicsContext*, EventHandler*, Button*)> execute
// } Callback;

enum class EVENT_TYPES {
    UNHANDLED_SDL_EVENT = 0,
    BUTTON_PRESS = 1,
};

typedef struct {
    EVENT_TYPES type { EVENT_TYPES::UNHANDLED_SDL_EVENT };
    SDL_Event sdl_event;
    int button_id { -1 };
} Event;

class EventHandler {
private:
    // Menu* menu { nullptr };

    std::vector<Button*> buttons;
    // std::vector<Callback> callbacks;
    Button* b_pressed { nullptr };

    Event event;

public:
    EventHandler() = default;
    // provide move assignment and copy?

    ~EventHandler() {
        for (auto& button : buttons) {
            button->dettachHandler();
            button = nullptr;
        }
        b_pressed = nullptr;
    }

public:
    inline Button* getButtonById(int id) {
        for(const auto& button : buttons)
            if(button->getId() == id) return button;
        return nullptr;
    }
    inline int getButtonIndexById(int id) {
        for(int i{0}; i < buttons.size(); i++)
            if(buttons.at(i)->getId() == id) return i;
        return -1;
    }

    inline void dropElementFromVector(int index) {
        std::vector<Button*> buffer;
        for (int i = buttons.size() - 1; i > -1; i--) {
            if(i != index) buffer.push_back(buttons.at(i));
            buttons.at(i) = nullptr;
            buttons.pop_back();
        }
    }

    // inline unsigned int getCallbackIndexById(int id) {
    //     for(int i{0}; i < callbacks.size(); i++)
    //         if(callbacks.at(i).id == id) return i;
    //     return -1;
    // }

    // void registerCallBack(unsigned int id, std::function<void(GraphicsContext*, EventHandler*, Button*)> callback) {
    //     if(!getButtonById(id)) return;
    //     if(const index = getCallbackIndexById(); index > 0)
    //         callbacks.at(i) = { id, callback }; return;
    //     callbacks.push_back({ id, callback });
    // }
    void registerButtonToHandler(Button* button);
    Event PollEvent();
    std::vector<Button*> getSelectedButtons();
    std::vector<int> getSelectedButtonIds();
    // void addButtonToSelected(const Button* button);
    // void removeButtonFromSelected(const Button* button);
    void removeButtonFromHandler(int id);
};