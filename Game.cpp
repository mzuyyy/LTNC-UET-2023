//
// Created by MZuy on 3/3/2023.
//

#include "Game.h"
#include "logStatus.h"
#include "map.h"
#include "Pacman.h"

Log* consoleGame = new Log("Game");

Pacman* pacman = nullptr;
Map* map = nullptr;

void Game::init() {
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

    pacman = new Pacman("../Assets/pacmanTexture.png", renderer);

    map = new Map(renderer);

    map->initAnimation(renderer);

    while (isRunning) {
        handleEvent();

        map->update();

        pacman->checkTouchWall(map->checkWall(pacman->nextPosition(handleEvent())));
        pacman->update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        map->renderMap(renderer);
        pacman->render();

        SDL_RenderPresent(renderer);

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
                    if (!map->checkWall(pacman->nextPosition(event))){
                        pacman->changeDirection("up");
                    }
                    break;
                case SDLK_DOWN:
                    if(!map->checkWall(pacman->nextPosition(event))){
                        pacman->changeDirection("down");
                    }
                    break;
                case SDLK_LEFT:
                    if(!map->checkWall(pacman->nextPosition(event))){
                        pacman->changeDirection("left");
                    }
                    break;
                case SDLK_RIGHT:
                    if(!map->checkWall(pacman->nextPosition(event))){
                        pacman->changeDirection("right");
                    }
                    break;
                case SDLK_ESCAPE:
                    isPlaying = false;
                    break;
            }
    }
    return event;
}

void Game::clean() {
    pacman->clean();
    map->clean();
}

void Game::render() {

}

