#include "main.hpp"

int main() {
    GraphicsContext g_context = GraphicsContext(800, 600);
    std::cout << "Succesfully initialised graphicsContext!\n";

    SDL_Rect rect { 200, 150, 400, 300 };
    SDL_Color color { 102, 0, 0, 255 };
    SDL_Color border_color { 255, 51, 51, 0 };
    int r { 5 };
    bool shadow { false };

    bool done {false};
    SDL_Event event;
    while(!done) {
        SDL_SetRenderDrawColor(g_context.renderer, 255, 255, 255, 255);
        SDL_RenderClear(g_context.renderer);
        // drawRoundedRect(g_context.renderer, rect, color, border_color, r, shadow);
        drawCutRect(g_context.renderer, rect, color, border_color, r, shadow);
        g_context.display();

        SDL_PollEvent(&event);
        switch (event.type) {
            case (SDL_QUIT): {
                done = true;
                break;
            }
            case (SDL_WINDOWEVENT): {
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    g_context.updateWindowSize(event.window.data1, event.window.data2);
                    // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
                }
                break;
            }
        }
    }
    return 0;
}