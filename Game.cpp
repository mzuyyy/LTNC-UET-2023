//
// Created by MZuy on 3/3/2023.
//

#include "Game.h"
#include "logStatus.h"

Log* consoleGame = new Log("Game");

Object* player;
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

    player = new Object("../Assets/pacman icon.png", 0, 0, renderer);

    while (isRunning) {

        handleEvent();
        player->update();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        player->render();
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::handleEvent() {
    //write function to handle event and movement of player
    // and player stop when no key is pressed
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    player->isMovingUp = true;
                    player->isMovingDown = false;
                    player->isMovingLeft = false;
                    player->isMovingRight = false;
                    break;
                case SDLK_DOWN:
                    player->isMovingUp = false;
                    player->isMovingDown = true;
                    player->isMovingLeft = false;
                    player->isMovingRight = false;
                    break;
                case SDLK_LEFT:
                    player->isMovingUp = false;
                    player->isMovingDown = false;
                    player->isMovingLeft = true;
                    player->isMovingRight = false;
                    break;
                case SDLK_RIGHT:
                    player->isMovingUp = false;
                    player->isMovingDown = false;
                    player->isMovingLeft = false;
                    player->isMovingRight = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    player->isMovingUp = false;
                    break;
                case SDLK_DOWN:
                    player->isMovingDown = false;
                    break;
                case SDLK_LEFT:
                    player->isMovingLeft = false;
                    break;
                case SDLK_RIGHT:
                    player->isMovingRight = false;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

