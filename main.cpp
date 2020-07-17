#include "pong.h"

enum DIR {
    UP,
    DOWN,
};

const int W_W { 800 };
const int W_H { 500 };

SDL_Window *window { nullptr };
SDL_Renderer *renderer { nullptr };

bool Init()
{
    Py_Initialize();
    SDL_Init(SDL_INIT_EVERYTHING);
    //TTF_Init();

    window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_W, W_H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    return (window && renderer);
}

void Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //TTF_Quit();
    SDL_Quit();
}

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
 
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
 
	void reset()
	{
		m_beg = clock_t::now();
	}
 
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

class Ball {
    private:
        SDL_Point bPos { 400, 250 };
        SDL_Point bVel { 5, 0 };
        SDL_Rect bRect { 390, 240, 20, 20 };
    
    public:
        Ball(int v): bVel { v, 0 }, bPos { 400, 250 }, bRect { 390, 240, 20, 20 } {};
        
        SDL_Point getPos() {
            return bPos;
        }

        SDL_Point getVel(){
            return bVel;
        }

        void move(){
            bPos.x += bVel.x;
            bPos.y += bVel.y;

            bRect.x = bPos.x - 10;
            bRect.y = bPos.y - 10;
        }

        void checkCollision(SDL_Point padPos, bool lcl) {
            SDL_Rect lcl_goal { 0, 100, 5, 300 };
            SDL_Rect rmt_goal { 795, 100, 5, 300 };

            bool goalX { bPos.x <= 800 && bPos.x >= 795 };
            bool goalY { bPos.y <= 400 && bPos.y >= 100 };

            bool goalLX { bPos.x <= 5 && bPos.x >= 0 };
            bool goalLY { bPos.y <= 400 && bPos.y >= 100 };

            if ((goalX && goalY) || (goalLX && goalLY)) {
                bPos.x = 400;
                bPos.y = 250;

                //bVel.x = 5;
                //bVel.y = 0;
            }
            
            if (bPos.x >= 800) {
                bPos.x = 800;
                bVel.x = -5;
            } else if (bPos.x <= 0) {
                bPos.x = 0;
                bVel.x = 5;
            }

            if (bPos.y >= 500) {
                bPos.y = 500;
                bVel.y = -5;
            } else if (bPos.y <= 0) {
                bPos.y = 0;
                bVel.y = 5;
            }

            bool hitX { bPos.x <= padPos.x + 5 && bPos.x >= padPos.x };
            bool hitY { bPos.y <= padPos.y + 50 && bPos.y >= padPos.y };

            if (hitX && hitY) {
                if (padPos.y <= 400) {
                    bVel.y = 5;
                } else if (padPos.y >= 400) {
                    bVel.y = -5;
                }

                if (lcl) {
                    bVel.x = 5;
                } else {
                    bVel.x = -5;
                }
            }

            bRect.x = bPos.x - 10;
            bRect.y = bPos.y - 10;

        }

        void display(SDL_Renderer *renderer) {
            bRect.x = bPos.x - 10;
            bRect.y = bPos.y - 10;

            SDL_SetRenderDrawColor(renderer, 0, 250, 0, 250);
            SDL_RenderFillRect(renderer, &bRect);
        }
};

class paddle {
    private:
        const SDL_Color padClr { 0, 0, 250, 250 };
        SDL_Rect padRct { 100, 50, 5, 50 };
        //maybe implement active, whereby strong ball knocks paddle into inactivity

    public:
        bool lcl { true };
        DIR dir { UP };

        paddle() = default;

        SDL_Point getPos() {
            return { padRct.x, padRct.y };
        }

        void move(const SDL_Point &cursorPos, bool remote) {
            
            if (cursorPos.x < 400 || remote) {
                padRct.x = cursorPos.x;
            }

            padRct.y = cursorPos.y;
        }

        void display(SDL_Renderer *renderer, bool remote) {
            if (!remote) {
                SDL_SetRenderDrawColor(renderer, padClr.r, padClr.g, padClr.b, padClr.a);
            } else {
                SDL_SetRenderDrawColor(renderer, 250, 0, 0, 250);
            }
            SDL_RenderFillRect(renderer, &padRct);
        }

};

int main() {

    Timer init;

    if (!Init()) {
        std::cout << "Failure to initalise SDL window or renderer.\n";
        return 0;
    }

    std::cout << "Succesfully initialised SDL window and renderer.\n";

    bool done { false };

    std::cout << "Establishing session...\n";
    FILE *establishPy = fopen("httpEstablish.py", "r");
    PyRun_SimpleFile(establishPy, "httpEstablish.py");
    fclose(establishPy);

    std::ifstream iServerData("serverData.json");
    json sData;
    iServerData >> sData;
    iServerData.close();

    paddle lcl_paddle;
    paddle rmt_paddle;
    Ball ball(sData["v"]);

    SDL_Rect lcl_goal { 0, 100, 5, 300 };
    SDL_Rect rmt_goal { 795, 100, 5, 300 };
    SDL_Point cursorPos { 0, 0 };
    SDL_Event event;

    std::cout << "Initialisation time:\t" << init.elapsed() << '\n';

    while(!done){
        Timer gLoop;

        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 250);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
        SDL_RenderDrawLine(renderer, 400, 0, 400, 500);
        //add moving line functionality

        SDL_SetRenderDrawColor(renderer, 50, 150, 250, 250);
        SDL_RenderFillRect(renderer, &lcl_goal);
        SDL_SetRenderDrawColor(renderer, 250, 150, 50, 250);
        SDL_RenderFillRect(renderer, &rmt_goal);

        std::cout << "Time in gameLoop until HTTP requests:\t" << gLoop.elapsed() << '\n';

        Timer get;
        FILE *getPy = fopen("httpGet.py", "r");
        PyRun_SimpleFile(getPy, "httpGet.py");
        fclose(getPy);
        std::cout << "httGet.py call\t:" << get.elapsed() << '\n';

        std::ifstream iServerData("serverData.json");
        json sData;
        iServerData >> sData;
        iServerData.close();
        
        json lData;

        lData["rmt_cPos"]["x"] = sData["rmt_cPos"]["x"];
        lData["rmt_cPos"]["y"] = sData["rmt_cPos"]["y"];
        
        //lData["ball"]["pos"] = sData["ball"]["pos"];
        //lData["ball"]["vel"] = sData["ball"]["vel"];

        SDL_Point rmt_cursorPos { sData["rmt_cPos"]["x"], sData["rmt_cPos"]["y"] };

        SDL_GetMouseState(&cursorPos.x, &cursorPos.y);
        std::cout << "Mouse State obtained: (" << cursorPos.x << ", "<< cursorPos.y << ")\n";

        lcl_paddle.move(cursorPos, false);
        rmt_paddle.move(rmt_cursorPos, true);
        ball.move();
        ball.checkCollision(lcl_paddle.getPos(), true);
        ball.checkCollision(rmt_paddle.getPos(), false);

        lcl_paddle.display(renderer, false);
        rmt_paddle.display(renderer, true);
        ball.display(renderer);

        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
       if (event.type == SDL_QUIT){
           done = true;
       }

        lData["done"] = done;
        lData["lcl_cPos"]["x"] = lcl_paddle.getPos().x;
        lData["lcl_cPos"]["y"] = lcl_paddle.getPos().y;
    /*
        lData["ball"]["pos"][0] = ball.getPos().x;
        lData["ball"]["vel"][0] = ball.getVel().x;
        lData["ball"]["pos"][1] = ball.getPos().y;
        lData["ball"]["vel"][1] = ball.getVel().y;
    */

        std::ofstream oServerData("serverData.json", std::fstream::trunc);
        oServerData << lData;
        oServerData.close();

        Timer post;
        FILE *postPy = fopen("httpPost.py", "r");
        PyRun_SimpleFile(postPy, "httpPost.py");
        fclose(postPy);

        std::cout << "httPost.py call\t:" << post.elapsed() << '\n';
        
        std::cout << "Game Loop Length:\t" << gLoop.elapsed() << '\n';

    }

    Quit();

    return 0;
}