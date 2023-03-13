//
// Created by MZuy on 3/3/2023.
//

#include "textureManager.h"

SDL_Texture *textureManager::LoadTexture(const std::string& fileName, SDL_Renderer *renderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}
