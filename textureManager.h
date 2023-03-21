//
// Created by MZuy on 3/3/2023.
//

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>


class textureManager {
public:
    SDL_Texture* LoadTexture(const std::string& fileName, SDL_Renderer* renderer);
    void drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer);
};


