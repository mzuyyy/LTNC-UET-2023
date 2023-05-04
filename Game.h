//
// Created by MZuy on 3/3/2023.
//
#pragma once

#include "Engine.h"
#include "Objects/Pacman.h"
#include "Objects/Ghost.h"
#include "logStatus.h"
#include "Map.h"
#include "States/StateManager.h"
#include <set>


class Game {
private:
    static const int WINDOW_WIDTH = MAP_WIDTH * MAP_SIZE;
    static const int WINDOW_HEIGHT = MAP_HEIGHT * MAP_SIZE + 24 * 12;
    const char* WINDOW_TITTLE = "Pac-man";
    static const int rendererFlags =  SDL_RENDERER_ACCELERATED;

    bool isRunning = false;

    StateManager* stateManager;
    Engine* engine;

    Log* consoleGame;

    SDL_Window* window;
    SDL_Renderer* renderer;

    const int FPS = 240;
    int frameDelay = 1000/FPS;

    Uint32 frameStart{};
    int frameTime{};
public:
    Game(){
        renderer = nullptr;
        window = nullptr;
    };
    ~Game(){
        renderer = nullptr;
        window = nullptr;
    };
    void init();
    void close();
    void runGame();

    SDL_Event handleEvent();
    void handleState();

    SDL_Renderer* getRenderer() const {
        return renderer;
    }
};


