#ifndef FSELECT_H
#define FSELECT_H

#include "global.h"
#include "text.h"
#include "button.h"

class Menu
{
private:

    const SDL_Colour bckgr_clr;
    const SDL_Colour mn_clr;
    /*
    static const SDL_Color btt_clr;
    static const char* btt_ft;
    static const SDL_Color btt_ftclr;
    static const SDL_Color font_color;
    */

    SDL_Texture *bckgr_texture;

    SDL_Rect menurect;
    
    std::vector<Text> txt;

    std::vector <Button> btt;

    Button *bpressed { nullptr };
    Button *bselected { nullptr };
    
    int tn {0};
    int an {0};
    int kn {0};
    int tot {0};
    
    bool button_functionality(int id);

    int generateTextures(SDL_Renderer *renderer);

public:

    bool effen { false };
    //gRatio gridRatio { UNDEFINED };

    void updateDimensions();

    //Menu();
    Menu(SDL_Rect menuRect, SDL_Texture *bckgr_texture, SDL_Colour bckgr_clr, SDL_Colour mn_clr, SDL_Renderer *renderer);

    void addBtt(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer);
    void addTxt(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer);
    void assignTxt(int index, const std::string text, SDL_Renderer *renderer);

    void display(SDL_Renderer *renderer, SDL_Rect *src_rect, SDL_Rect *dest_rect);
    
    GAME_STATE get_uinput();
    
    int update();

    void destroyTextures();
    //~Menu();
};

#endif
