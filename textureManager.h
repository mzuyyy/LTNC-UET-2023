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
    SDL_Texture* setTextureAlphaMod(SDL_Texture* texture, Uint8 alpha);
    SDL_Texture* setTextureBlendMod(SDL_Texture* texture, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND);

    void clean(SDL_Texture* texture);
};


