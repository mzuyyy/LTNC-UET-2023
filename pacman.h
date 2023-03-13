//
// Created by MZuy on 3/12/2023.
//

#ifndef BTL_PACMAN_H
#define BTL_PACMAN_H

#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>


class Pacman : public Object{
public:
    Pacman(const char * textureSheet, int x, int y, SDL_Renderer* renderer);
    ~Pacman();
    void update();
    void render();
    void updateClip();
    bool isMovingDown = false;
    bool isMovingUp = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

private:
    int xPosition;
    int yPosition;
    int xVelocity = 0;
    int yVelocity = 0;
    SDL_Texture* objectTexture{};
    SDL_Rect sourceRect{}, destinationRect{};

    int widthFrame = 26;
    int heightFrame = 26;

    SDL_Rect frameClip[12]{};
    const char* pacmanTextureSheet = "../Assets/pacman icon.png";
};


#endif //BTL_PACMAN_H
