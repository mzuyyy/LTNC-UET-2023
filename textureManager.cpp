//
// Created by MZuy on 3/3/2023.
//

#include "textureManager.h"
SDL_Texture* textureManager::loadTexture(const std::string& path, SDL_Renderer* renderer) {
    consoleTexture->updateStatus("Loading texture: " + path);
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    if (tempSurface == nullptr)
        consoleTexture->updateStatus("IMG_Load Error: " + std::string(IMG_GetError()));
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if (tempTexture == nullptr)
        consoleTexture->updateStatus("SDL_CreateTextureFromSurface Error: " + std::string(SDL_GetError()));
    SDL_FreeSurface(tempSurface);
    return tempTexture;
}
void textureManager::drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer) {
    SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, 0, nullptr, SDL_FLIP_NONE);
    if (SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, 0 , nullptr, SDL_FLIP_NONE) != 0)
        consoleTexture->updateStatus("SDL_RenderCopy Error: " + std::string(SDL_GetError()));
}
void textureManager::drawTexture(SDL_Texture *texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_Renderer *renderer,
                                 SDL_RendererFlip flip) {
    if (SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, 0, nullptr, flip) != 0)
        consoleTexture->updateStatus("SDL_RenderCopy Error: " + std::string(SDL_GetError()));
}
void textureManager::clean(SDL_Texture *texture) {
    consoleTexture->updateStatus("Cleaning texture");
    SDL_DestroyTexture(texture);
}

SDL_Texture *textureManager::setTextureAlphaMod(SDL_Texture *texture, Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
    return texture;
}

SDL_Texture *textureManager::sdlTextureBlendMod(SDL_Texture *texture, SDL_BlendMode blendMode) {
    SDL_SetTextureBlendMode(texture, blendMode);
    return texture;
}




