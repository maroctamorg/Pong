#include "text.h"
#include "button.h"
#include "menu.h"
#include "global.h"

//Starts up SDL and creates window
bool Init();

//Frees media and shuts down SDL
void Quit();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Music
Mix_Music *music = NULL;

bool Init()
{
	//Initialization flag
	bool success = true;
    
    //Initialize TTF and MIXER
    TTF_Init();

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow( "Keep Up!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g::W_W, g::W_H, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

    //Initializing AUDIO subsystem
    // SDL_InitSubSystem(SDL_INIT_AUDIO);
    // Mix_AllocateChannels(1);
    // Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

	return success;
}

void Quit()
{
	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	// Mix_CloseAudio();
    // Mix_Quit();
	TTF_Quit();
    IMG_Quit();
	SDL_Quit();
}

int main(int arc, char* argv[]) {

    //Initializing window and renderer
    if (!Init()) {
        std::cout << "Failure to initalise SDL window or renderer.\n";
        return 1;
    }
    
    std::cout << "Succesfully initialised SDL window and renderer.\n";

    //Initializing start menu
    SDL_Rect st_menuRect {100, 75, g::W_W - 200, g::W_H - 150};

    // COLORS ###################################
    // LIGHT THEME
        SDL_Colour st_bckgr_clr {13, 13, 13, 255};
        SDL_Colour st_mn_clr {166, 166, 166, 50};
        SDL_Colour st_mn_btt_clr {77, 0, 0, 255};
        SDL_Colour st_mn_ft_clr {242, 242, 242, 255};
    
    // DARK THEME
        // SDL_Colour st_bckgr_clr {13, 13, 13, 255};
        // SDL_Colour st_mn_clr {166, 166, 166, 50};
        // SDL_Colour st_mn_btt_clr {77, 0, 0, 255};
        // SDL_Colour st_mn_ft_clr {242, 242, 242, 255};
    // ###########################################

    std::cout << "Test printf 1...\n";

    Menu st_menu {st_menuRect, nullptr, st_bckgr_clr, st_mn_clr, renderer};

    std::cout << "Test printf 2...\n";

    //Initializing start menu button objects
    SDL_Rect st_mn_btt1_rect {300, 400, 200, 50};
    SDL_Rect st_mn_btt1_target_rect {310, 410, 150, 30};
    st_menu.addBtt(NULL, "Button Text", g::font, 16, &st_mn_ft_clr, &st_mn_btt1_target_rect, &st_mn_btt1_rect, &st_mn_btt_clr, true, 0, renderer);

    //Initializing start menu text objects
    SDL_Rect st_mn_txt1_target_rect {200, 150, 400, 60};
    SDL_Color color {0, 0, 0, 0};
    st_menu.addTxt("Button Test Writing Here", g::font, 22, &st_mn_ft_clr, &st_mn_txt1_target_rect, &color, true, renderer);

    //Initializing menu loop variables
    bool menu_done {false};
    std::cout << "Starting start menu!";
    SDL_Event event;

    //Start menu
    while(!menu_done) {
        st_menu.display(renderer, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
            menu_done = true;
        }
    }

    // Destroy Textures
    st_menu.destroyTextures();

    Quit();

    return 0;
}