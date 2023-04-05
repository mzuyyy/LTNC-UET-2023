//
// Created by MZuy on 3/3/2023.
//
#pragma once

#include "Engine.h"
#include "Object/Pacman.h"
#include "logStatus.h"
#include "Map.h"
#include <set>


class Game {
private:
    static const int WINDOW_WIDTH = 672;
    static const int WINDOW_HEIGHT = 888;
    const char* WINDOW_TITTLE = "Pac-man";
    static const int rendererFlags =  SDL_RENDERER_ACCELERATED;

    bool isRunning = false;
    bool isPlaying = false;

    Log* consoleGame = nullptr;
    Pacman* pacman = nullptr;
    Map* map = nullptr;

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
    void load(const std::string &path);
    void loadHighScore();
    void save();
    void saveHighScore();
    void close();
    void runGame();

    SDL_Event handleEvent();

    SDL_Renderer* getRenderer() const {
        return renderer;
    }
};


