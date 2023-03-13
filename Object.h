//
// Created by MZuy on 3/3/2023.
//

#ifndef BTL_OBJECT_H
#define BTL_OBJECT_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>


class Object {
protected:
    int xPosition;
    int yPosition;

    SDL_Texture* objectTexture;
    SDL_Rect sourceRect{}, destinationRect{};
    SDL_Renderer* renderer;
public:
    Object(const char* textureSheet, int x, int y, SDL_Renderer* renderer);
    ~Object();

    virtual void update();

    virtual void render();
    bool isMovingDown = false;
    bool isMovingUp = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
};
#endif

