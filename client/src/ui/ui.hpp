#ifndef UI_HPP
#define UI_HPP

#include "Tsukuyomi.hpp"


namespace UI {
    SDL_Color palette[3] {SDL_Color{68, 72, 87, 255}, SDL_Color{122, 163, 152, 255}, SDL_Color{207, 198, 169, 255}};
    const std::string font {"resources/fonts/CourierNew.ttf"};
    int W_W {800};
    int W_H {600};
};

#include "splash.hpp"
#include "mainmenu.hpp"
#include "../main.hpp"
// #include "pause.hpp"

#endif