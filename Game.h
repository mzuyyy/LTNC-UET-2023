//
// Created by MZuy on 3/3/2023.
//
#pragma once

#include <iostream>
#include "textureManager.h"
#include "Object.h"
#include "Pacman.h"
#include "map.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

class Game {
private:
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;
    const char* WINDOW_TITTLE = "Pac-man";
    static const int rendererFlags =  SDL_RENDERER_ACCELERATED;

    bool isRunning = false;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const int FPS = 240;
    int frameDelay = 1000/FPS;

    Uint32 frameStart{};
    int frameTime{};

    Pacman* pacman = new Pacman("../Assets/pacmanTexture.png", 0, 0, renderer);

    Map* map = new Map("../Assets/mapTexture.png", 280, 0, renderer);
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
    void handleEvent();
    void runGame();
};


