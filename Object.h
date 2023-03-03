//
// Created by MZuy on 3/3/2023.
//

#pragma once
#include "Game.h"

class Object {
private:
    int xPosition;
    int yPosition;

    SDL_Texture* objectTexture;
    SDL_Rect sourceRect{}, destinationRect{};
    SDL_Renderer* renderer;
public:
    Object(const char* textureSheet, int x, int y, SDL_Renderer* renderer);
    ~Object();
    void update();
    void render();
};


