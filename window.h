//
// Created by MZuy on 3/2/2023.
//

#ifndef BTL_WINDOW_H
#define BTL_WINDOW_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Window {
private:
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;
    const char* WINDOW_TITTLE = "Pac-man";
    static const int rendererFlags =  SDL_RENDERER_ACCELERATED;

    bool isRunning = false;

    SDL_Surface* tempSurface = IMG_Load("../Assets/pacman icon.png");

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
public:
    Window(){
        window = nullptr;
        renderer = nullptr;
    };
    ~Window(){
        renderer = nullptr;
        window = nullptr;
    };
    void init();
    void close();
    void runGame();

};


#endif //BTL_WINDOW_H
