//
// Created by MZuy on 3/24/2023.
//

#include "MenuState.h"
MenuState::MenuState(SDL_Renderer *renderer) {
    menuFrameClip.resize(11);
    menuTexture = menuManager->loadTexture(menuPath, renderer);
    setMenuFrameClip();
}

void MenuState::render(const int &type, SDL_Renderer *renderer) {
    sourceRect = menuFrameClip[type];
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = MENU_WIDTH;
    destRect.h = MENU_HEIGHT;
    SDL_RenderCopy(renderer, menuTexture, &sourceRect, &destRect);
}

void MenuState::update() {

}
void MenuState::setMenuFrameClip() {
    for (int i = 0; i < 11; i++){
        menuFrameClip[i].x = i * MENU_TILE_WIDTH;
        menuFrameClip[i].y = 0;
        menuFrameClip[i].w = MENU_WIDTH;
        menuFrameClip[i].h = MENU_HEIGHT;
    }
}