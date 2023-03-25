//
// Created by MZuy on 3/3/2023.
//

#include "textureManager.h"

SDL_Texture* textureManager::loadTexture(std::string path, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tempTexture;
}
void textureManager::drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer) {
    SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, 0, nullptr, SDL_FLIP_NONE);
    if (SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, 0 , nullptr, SDL_FLIP_NONE) != 0)
        consoleTexture->updateStatus("SDL_RenderCopy Error: " + std::string(SDL_GetError()));
}

void textureManager::clean(SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
}
