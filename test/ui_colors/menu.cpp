#include "menu.h"

//const SDL_Colour menu::bckgr_clr   { 230, 230, 230, 250 };
//const SDL_Colour menu::mn_clr      { 193, 193, 193, 250 };

//const SDL_Color menu::btt_clr { 95, 95, 95, 250 };
//const SDL_Color menu::btt_ftclr { 250, 250, 250, 250 };
//const SDL_Color menu::font_color { 0, 0, 0, 250 };

/*void menu::updateDimensions()
{
    int x, y, w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GetWindowPosition(window, &x, &y);
    menurect = { x, y, w, h };

    //update buttons and text textures: figure out everything as ratios (mostly ptsize's)
    
}*/

/*menu::menu()
    :   menurect { constants::W_W/20, constants::W_H/20, 9*constants::W_W/10, 9*constants::W_H/10 }, a1_remaining {0}, a2_remaining {0}, btt {}, txt {}, bselected { nullptr, nullptr },
        btt_ft {"resources/Krungthep.ttf"}, effen { false }, tn {0}, an {0}, kn {0}, tot {0}
{
    std::cout << "menu() constructor called.\n";

    const int title_h {menurect.h/5};
    const int title_w {menurect.w/5};

    const int button_w {menurect.w/4};
    const int button_h {menurect.h/15};

    btt[0].assign(nullptr, "", btt_ft, 25, btt_ftclr, {menurect.x+7*menurect.w/12, menurect.y+13*menurect.h/15, button_w, button_h}, {menurect.x+7*menurect.w/12, menurect.y+13*menurect.h/15, button_w, button_h}, btt_clr, true, 0);
    btt[1].assign(nullptr, " REGULAR: 50*35", btt_ft, 25, btt_ftclr, {menurect.w/6+menurect.x, menurect.y+3*menurect.h/10, button_w, button_h}, {menurect.w/6+menurect.x, menurect.y+3*menurect.h/10, button_w, button_h}, btt_clr, true, 1);
    
    btt[2].assign(nullptr, "  WIDE: 60*30", btt_ft, 25, btt_ftclr, {menurect.x + 7 * menurect.w / 12, menurect.y + 3 * menurect.h / 10, button_w, button_h}, {menurect.x + 7 * menurect.w / 12, menurect.y + 3 * menurect.h / 10, button_w, button_h}, btt_clr, true, 2);
    btt[3].assign(nullptr, "  TALL: 35*40", btt_ft, 25, btt_ftclr, {menurect.x + menurect.w / 6, menurect.y + 3 * menurect.h / 5, button_w, button_h}, {menurect.x + menurect.w / 6, menurect.y + 3 * menurect.h / 5, button_w, button_h}, btt_clr, true, 3);
    btt[4].assign(nullptr, " DOUBLE: 100*70", btt_ft, 25, btt_ftclr, {menurect.x + 7 * menurect.w / 12, menurect.y + 3 * menurect.h / 5, button_w, button_h}, {menurect.x + 7 * menurect.w / 12, menurect.y + 3 * menurect.h / 5, button_w, button_h}, btt_clr, true, 4);
    btt[5].assign(nullptr, " .", btt_ft, 25, btt_ftclr, {menurect.x + menurect.w / 6, menurect.y + 13 * menurect.h / 15, 50, 50}, {menurect.x + menurect.w / 6, menurect.y + 13 * menurect.h / 15, button_h, button_h}, btt_clr, true, 5);
    */
/*
    std::cout << "btt assigned.\n";

    txt[0].assign("SELECT FIELD", btt_ft, 50, font_color, { 2*menurect.w/5+menurect.x - 75, menurect.y + title_h/4 + 25, menurect.w/5, menurect.h/5 });
    txt[1].assign("  EFFECTS ENABLED",btt_ft, 25, font_color, { menurect.x+menurect.w/6+50, menurect.y+13*menurect.h/15, 50, 50});

    std::cout << "txt assigned.\n";

    int control = generateTextures();
    std::cout << "generateTextures returned flag: " << control << '\n';
}*/

int Menu::generateTextures(SDL_Renderer *renderer)
{
    int control {0};
    if(renderer == nullptr) {
        control = 1;
        std::cout << "Null renderer passed to menu!\n";
    }
    return control;
    //implement when loading textures onto menu outside of buttons or texts
}

Menu::Menu(SDL_Rect menuRect, SDL_Texture *bckgr_texture, SDL_Colour bckgr_clr, SDL_Colour mn_clr, SDL_Renderer *renderer)
    : menurect(menuRect), bckgr_texture {bckgr_texture}, bckgr_clr {bckgr_clr}, mn_clr {mn_clr}, btt(2), txt(2) {

        int control = generateTextures(renderer);
        //std::cout << "menu::menu(...) : generateTextures returned flag: " << control << '\n';

    }

void Menu::addBtt(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer) {
    for (int i = 0; i < btt.size(); i++) {
        if(!btt.at(i).initialized) {
            btt.at(i).assign(imgPath, text, font, ptsize, font_color, target_rect, rect, color, is_active, id, renderer);
            return;
        }
    }

    Button newButton {imgPath, text, font, ptsize, font_color, target_rect, rect, color, is_active, id, renderer};
    btt.push_back(newButton);

}

void Menu::addTxt(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer) {

    for (int i = 0; i < txt.size(); i++) {
        if(!txt.at(i).isInitialized()) {
            txt.at(i).assign(text, font, ptsize, color, target_rect, rect_colour, display_rect, renderer);
            return;
        }
    }

    Text newText {text, font, ptsize, color, target_rect, rect_colour, display_rect, renderer};
    txt.push_back(newText);

}

void Menu::assignTxt(int index, const std::string text, SDL_Renderer *renderer) {
    txt.at(index).assignTxt(text, renderer);

}

//should implement this in the actual game loop: check for button selections and implement functionality
/*
bool menu::button_functionality(int id)
{
    std::cout << "Call to button functionality function.\n";

    std::cout << "Button id : " << id << '\n';
    switch(id)
    {
        case 0:
            if (bselected.at(0))
            {
                btt[0].b_color = bckgr_clr;
                
                return true;
            }
            break;
                  
        case 1:
            if (bselected.at(0))
            {
                bselected[0]->b_color = btt_clr;
            }

            bselected[0] = &btt[1];
            bselected[0]->b_color = bckgr_clr;

            //gridRatio = REGULAR;

            std::cout << "Field size: 50*35\n";
            break;
            
    }

    return false;
    
}
*/

void Menu::display(SDL_Renderer *renderer, SDL_Rect *src_rect, SDL_Rect *dest_rect)
{

    SDL_SetRenderDrawColor(renderer, bckgr_clr.r, bckgr_clr.g, bckgr_clr.b, bckgr_clr.a);
    SDL_RenderClear(renderer);
    // if (bckgr_texture)
    //     SDL_RenderCopy(renderer, bckgr_texture, src_rect, dest_rect);
    // else
    
    SDL_SetRenderDrawColor(renderer, mn_clr.r, mn_clr.g, mn_clr.b, mn_clr.a);
    SDL_RenderFillRect(renderer, &menurect);
    
    for (int i = 0; i < txt.size(); i++)
    {
        if(txt.at(i).isInitialized()) {
            txt[i].displayText(renderer);
        }
    }

    for (int i = 0; i < btt.size(); i++)
    {
        if(btt.at(i).initialized) {
            btt[i].display(renderer);
        }
    }


    //SDL_RenderPresent(renderer); //shouldn't call render present inside function

}

GAME_STATE Menu::get_uinput()
{
    GAME_STATE state { NOINPUT };

    SDL_Event event;
    SDL_Point cursor_pos { 0, 0 };

        SDL_PollEvent(&event);
        //std::cout << "Polling Events...\n";
        switch(event.type)
        {
            case SDL_MOUSEBUTTONDOWN:

                //std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";

                SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));

                //std::cout << "Mouse State obtained.\n";

                    for (int i {0}; i < btt.size(); i++)
                    {
                        //std::cout << "Evaluating btt at " << i <<'\n';
                        if (btt[i].isActive() && btt[i].Clicked(cursor_pos))
                        {
                            state = UPDATE;

                            //std::cout << "Call to press function.\n";

                            bpressed = btt[i].press();

                            //std::cout << "bpressed: " << bpressed << '\n';
                            //element.press must return a pointer to button element
                        }
                    }
                break;
            //case SDL_WINDOWEVENT_RESIZED:
                //updateDimensions();
                break;

            case SDL_QUIT:  
                state = TERMINATE;
                break;
        }
    
    return state;
}

int Menu::update()
{
// gets user input
// passes into class variables
// updates display

// get button id
// call button_functionality function from main menu - switch...

    bool done { false };

    //std::cout << "Call to update function.\n";
    if (bpressed)
    {
        return bpressed->b_id;
        bpressed = nullptr;
    }

    return done;    
    
}

void Menu::destroyTextures() {
    if(bckgr_texture != NULL && bckgr_texture != nullptr) {
        SDL_DestroyTexture(bckgr_texture);
        bckgr_texture = nullptr;
    }

    for(int i = 0; i < btt.size(); i++) {
        btt.at(i).destroyTexture();
    }

    for(int i = 0; i < txt.size(); i++) {
        txt.at(i).destroyTxtTexture();
    }
}

/*
Menu::~Menu() {

    if(bckgr_texture != nullptr && bckgr_texture == NULL) {
        SDL_DestroyTexture(bckgr_texture);
        bckgr_texture = nullptr;
    }
    
    bpressed = nullptr;
    bselected = nullptr;

    for(int i = 0; i < btt.size(); i++) {
        btt.at(i).~Button();
    }

    for(int i = 0; i < txt.size(); i++) {
        txt.at(i).~Text();
    }

}
*/