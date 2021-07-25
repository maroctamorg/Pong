#include "main.hpp"

int main() {
    GraphicsContext g_context = GraphicsContext(800, 600);
    std::cout << "Succesfully initialised graphicsContext!\n";

    SDL_Rect rect { 200, 150, 400, 300 };
    SDL_Color color { 102, 0, 0, 255 };
    SDL_Color border_color { 255, 51, 51, 0 };
    int r { 4 };

    bool done {false};
    SDL_Event event;
    while(!done) {
        SDL_SetRenderDrawColor(g_context.renderer, 255, 255, 255, 255);
        SDL_RenderClear(g_context.renderer);
        drawRoundedRect(g_context.renderer, rect, color, border_color, r);
        // drawCutRect(g_context.renderer, rect, color, border_color, r);
        g_context.display();

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            done = true;
        }
    }
    return 0;
}