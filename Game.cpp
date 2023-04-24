//
// Created by MZuy on 3/3/2023.
//

#include "Game.h"

void Game::init() {
    consoleGame = new Log("Game");
    consoleGame->updateStatus("Init game...");

    if(SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        consoleGame->updateStatus(SDL_GetError());
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITTLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        consoleGame->updateStatus("Open window success!");
        isRunning = true;

        if (window == nullptr) {
            consoleGame->updateStatus(SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            consoleGame->updateStatus("Create renderer success!");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

        }
    }
}

void Game::close() {
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    delete consoleGame;
    delete pacman;
    delete map;
    SDL_Quit();
}
void Game::runGame() {
    frameStart = SDL_GetTicks();

    engine = new Engine();
    stateManager = new StateManager(engine, renderer);

    engine->init();
    engine->load();
    engine->update();

    while (isRunning) {
        handleEvent();
        handleState();

        SDL_RenderClear(renderer);

        stateManager->render();

        SDL_RenderPresent(renderer);

        stateManager->update();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

SDL_Event Game::handleEvent() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
           stateManager->keyDown(event.key.keysym.sym);
           break;
        case SDL_KEYUP:
            stateManager->keyUp(event.key.keysym.sym);
    }
    return event;
}

void Game::handleState() {
    stateManager->pullRequest();
}
