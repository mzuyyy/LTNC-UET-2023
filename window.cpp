//
// Created by MZuy on 3/2/2023.
//

#include "window.h"
#include "logStatus.h"

Log* consoleWindow = new Log("Window");

void Window::init() {
    if(SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
       consoleWindow->updateStatus(SDL_GetError());
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITTLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        consoleWindow->updateStatus("Open window success!");
        isRunning = true;

        if (window == nullptr) {
            consoleWindow->updateStatus(SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            std::cout << "Success to create renderer!" << std::endl;
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

        }
    }
}
void Window::close() {
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_Quit();
}

void Window::runGame() {
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}