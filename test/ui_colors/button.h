#ifndef BUTTON_H
#define BUTTON_H

#include "global.h"
#include "text.h"

class Button
{
    public:

        Text b_txt {};
        
        SDL_Rect b_rect { 200, 200, 200, 200 };
        
        SDL_Color b_color { 250, 250, 250, 250 };

        SDL_Texture *b_texture { nullptr };

        state b_state { false };
        
        int b_id { -1 };

        bool initialized {false};

        int generateBttTexture(char *imgPath, SDL_Renderer *renderer);

    // public:


        Button() = default;
        Button(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer);

        void assign(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer);

        void activate();
        Button* press();

        void deactivate();

        bool isActive();
        bool isPressed();

        bool Clicked(const SDL_Point &cursor_pos);

        void display(SDL_Renderer *renderer);

        void destroyTexture();

        ~Button();
};

#endif
