//
// Created by MZuy on 3/3/2023.
//

#pragma once

#include <iostream>
#include "logStatus.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>


class textureManager {
private:
    Log* consoleTexture = new Log("Texture");
    std::map<std::string, SDL_Texture*> mapTexture;
public:
    SDL_Texture * loadTexture(const std::string& path, SDL_Renderer* renderer);
    void drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer);
    void drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer, SDL_RendererFlip flip);
    void clean(SDL_Texture* texture);
};


