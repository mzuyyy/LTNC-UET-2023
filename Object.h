//
// Created by MZuy on 3/3/2023.
//

#ifndef BTL_OBJECT_H
#define BTL_OBJECT_H

#include "textureManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>

class Object {
protected:
    SDL_Point objectPosition{};

    SDL_Point defaultPosition = {0, 0};
    SDL_Texture* objectTexture;

    SDL_Rect sourceRect{}, destRect{};
    SDL_Renderer* pacmanRenderer = nullptr;
public:
    Object(const std::string &textureSheet, SDL_Renderer *renderer);
        ~Object();

    virtual void update();

    virtual void render();

    virtual SDL_Point getNextPosition();

    textureManager* objectManager = new textureManager();

    std::map<std::string,bool> objectDir;
};
#endif

