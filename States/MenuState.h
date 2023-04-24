//
// Created by MZuy on 3/24/2023.
//

#ifndef BTL_MENUSTATE_H
#define BTL_MENUSTATE_H

#include "../textureManager.h"

class MenuState {
private:
    const int MENU_TILE_WIDTH = 671;
    const int MENU_TILE_HEIGHT = 888;

    const int MENU_WIDTH = 448;
    const int MENU_HEIGHT = 498;

    SDL_Texture* menuTexture;
    SDL_Rect sourceRect{}, destRect{};
    textureManager* menuManager = new textureManager();
    std::string menuPath = "../Assets/menu.png";
    std::vector<SDL_Rect> menuFrameClip;
public:
    MenuState(SDL_Renderer *renderer);
    ~MenuState();
    void render(const int &type, SDL_Renderer *renderer);
    void update();
    void setMenuFrameClip();
};


#endif //BTL_MENUSTATE_H
