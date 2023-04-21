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

    pacman = new Pacman(renderer, CLASSIC);

    blinky = new Ghost(BLINKY,{0,0}, renderer);
    inky = new Ghost(INKY,{0,1}, renderer);
    pinky = new Ghost(PINKY,{0,2}, renderer);
    clyde = new Ghost(CLYDE,{0,3}, renderer);

    map = new Map(renderer);

    while (isRunning) {
        handleEvent();

        SDL_RenderClear(renderer);

        map->renderMap(renderer);
        pacman->render();
        blinky->render();
        inky->render();
        pinky->render();
        clyde->render();
        map->update();
        pacman->update();

        SDL_RenderPresent(renderer);

        pacman->checkMove(!Map::isWallAt(pacman->getNextPosition()));

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
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    pacman->queueDirection(UP);
                    break;
                case SDLK_DOWN:
                    pacman->queueDirection(DOWN);
                    break;
                case SDLK_LEFT:
                    pacman->queueDirection(LEFT);
                    break;
                case SDLK_RIGHT:
                    pacman->queueDirection(RIGHT);
                    break;
                case SDLK_ESCAPE:
                    isPlaying = false;
                    break;
            }
    }
    return event;
}

void Game::load(const std::string &path) {

}

void Game::loadHighScore() {

}

void Game::save() {

}

void Game::saveHighScore() {

}
