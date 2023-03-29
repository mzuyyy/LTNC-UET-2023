//
// Created by MZuy on 3/3/2023.
//

#ifndef BTL_OBJECT_H
#define BTL_OBJECT_H

#include "../textureManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
enum Direction{
    NONE = 0,
    UP = -1,
    DOWN = 1,
    LEFT = -2,
    RIGHT = 2,
};
struct Position{
    int x;
    int y;
    Position(int x, int y) : x(x), y(y) {}
    Position() : x(0), y(0) {}
};
struct TileID : Position {
    TileID(int x, int y) : Position(x, y) {}
    TileID() : Position(0, 0) {}
};
class Object {
protected:
    TileID tileID{};
    Position position{};

    TileID startTileID{};
    SDL_Texture* objectTexture;

    SDL_Rect sourceRect{}, destRect{};
    SDL_Renderer* objectRenderer = nullptr;

    int objectWidth{};
    int objectHeight{};

    int velocity{};

    std::queue<Direction> directionQueue;

    int frame;
    int animationSpeed;
    int frameCount;
public:
    Object(const std::string &textureSheet, SDL_Renderer *renderer, SDL_Point initPosition);
        ~Object();

    virtual void update();

    virtual void render();

    virtual void move();

    virtual TileID getTileID();

    virtual Position getPosition();

    bool checkPosition();

    textureManager* objectManager = new textureManager();


};
#endif

