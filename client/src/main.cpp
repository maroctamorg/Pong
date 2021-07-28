#include "main.hpp"
#include "ui/test_menu.hpp"

int main() {
    std::shared_ptr<GraphicsContext> g_context = std::make_shared<GraphicsContext>(800, 600);
    std::cout << "Succesfully initialised graphicsContext!\n";

    std::unique_ptr<Menu> test_menu = Test_Menu(g_context);

    bool done {false};
    SDL_Event event;
    while(!done) {
        test_menu->render();
        g_context->display();

        SDL_PollEvent(&event);
        switch (event.type) {
            case (SDL_QUIT): {
                done = true;
                break;
            }
            case (SDL_WINDOWEVENT): {
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    g_context->updateWindowSize(event.window.data1, event.window.data2);
                    test_menu->updateSize();
                    // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
                }
                break;
            }
        }
    }
    return 0;
}