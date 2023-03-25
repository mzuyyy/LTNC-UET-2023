//
// Created by MZuy on 3/3/2023.
//
#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

class Game {
private:
    static const int WINDOW_WIDTH = 882;
    static const int WINDOW_HEIGHT = 498;
    const char* WINDOW_TITTLE = "Pac-man";
    static const int rendererFlags =  SDL_RENDERER_ACCELERATED;

    bool isRunning = false;
    bool isPlaying = false;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
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
    void clean();

    void render();
    void runGame();
    SDL_Event handleEvent();

};


