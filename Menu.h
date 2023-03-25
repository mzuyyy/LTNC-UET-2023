//
// Created by MZuy on 3/24/2023.
//

#ifndef BTL_MENU_H
#define BTL_MENU_H

#include "textureManager.h"

class Menu {
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
    Menu(SDL_Renderer *renderer);
    ~Menu();
    void render(const int &type, SDL_Renderer *renderer);
    void update();
    void setMenuFrameClip();
};


#endif //BTL_MENU_H
