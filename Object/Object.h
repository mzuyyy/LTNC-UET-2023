//
// Created by MZuy on 3/3/2023.
//

#ifndef BTL_OBJECT_H
#define BTL_OBJECT_H

#include "../textureManager.h"
#include "../Timer.h"
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
enum OBJECT_TYPE
{
    OBJECT_PACMAN = 0,
    OBJECT_PACMAN_ANDROID,
    OBJECT_PACMAN_MS,
    OBJECT_BLINKY,
    OBJECT_PINKY,
    OBJECT_INKY,
    OBJECT_CLYDE,
    OBJECT_FRIGHTENED_GHOST,
    OBJECT_EATEN_GHOST,
    OBJECT_PACMAN_DEATH,
    OBJECT_DOT,
    OBJECT_DOT_STATUS,
    OBJECT_LEVEL,
    OBJECT_BELL,
    OBJECT_KEY,
    OBJECT_FRUIT,
    OBJECT_PACMAN_LIFE,
    OBJECT_MYSTERY,
    OBJECT_DEADLY,
    OBJECT_SPEEDY,
    OBJECT_INVISY,
    OBJECT_FREEZY,
    OBJECT_GOLDEN,
    OBJECT_GOLDEN_EXHAUSTED_DEFAULT,
    OBJECT_GOLDEN_EXHAUSTED_YELLOW,
    OBJECT_GOLDEN_DEATH,
    OBJECT_TYPE_TOTAL
};
const int OBJECT_PIXEL = 42;
const std::string OBJECT_TEXTURE_SHEET = "../Assets";
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
    Timer* timer;
    TileID tileID{};
    Position position{};

    OBJECT_TYPE objectType;
    TileID startTileID{};
    SDL_Texture* objectTexture;

    SDL_Rect sourceRect{}, destRect{};
    SDL_Renderer* objectRenderer = nullptr;

    int objectWidth{};
    int objectHeight{};

    int velocity{};

    std::queue<Direction> directionQueue;
    std::deque<TileID> lastPoint;

    int frame{};
    int frameCount{};
public:
    explicit Object(const std::string &textureSheet = OBJECT_TEXTURE_SHEET, SDL_Renderer *renderer = nullptr, OBJECT_TYPE type = OBJECT_PACMAN, Timer* _timer = nullptr);
        ~Object();

    virtual void update();

    virtual void render();

    virtual void move(Position _velocity);

    virtual TileID getTileID();
    void setTileID(TileID tileID);

    virtual Position getPosition();
    void setPosition(Position position);

    bool checkPosition() const;

    bool checkCollision(Object* object);
    void checkTunnel();
    textureManager* objectManager = new textureManager();


};
#endif

