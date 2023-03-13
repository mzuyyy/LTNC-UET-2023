//
// Created by MZuy on 3/3/2023.
//

#include "Game.h"
#include "logStatus.h"

Log* consoleGame = new Log("Game");

Pacman* pacman = nullptr;


int count = 0;
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
            std::cout << "Success to create renderer!" << std::endl;
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
    SDL_Quit();
}
void Game::runGame() {
    frameStart = SDL_GetTicks();
    pacman = new Pacman("../Assets/pacmanTexture.png", 0, 0, renderer);

    while (isRunning) {
        handleEvent();
        pacman->update();
        pacman->updateClip();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        pacman->render();
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::handleEvent() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    pacman->isMovingUp = true;
                    pacman->isMovingDown = false;
                    pacman->isMovingLeft = false;
                    pacman->isMovingRight = false;
                    break;
                case SDLK_DOWN:
                    pacman->isMovingUp = false;
                    pacman->isMovingDown = true;
                    pacman->isMovingLeft = false;
                    pacman->isMovingRight = false;
                    break;
                case SDLK_LEFT:
                    pacman->isMovingUp = false;
                    pacman->isMovingDown = false;
                    pacman->isMovingLeft = true;
                    pacman->isMovingRight = false;
                    break;
                case SDLK_RIGHT:
                    pacman->isMovingUp = false;
                    pacman->isMovingDown = false;
                    pacman->isMovingLeft = false;
                    pacman->isMovingRight = true;
                    break;
                default:
                    break;
            }
    }
}

