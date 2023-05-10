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
    UP = -2,
    DOWN = 2,
    LEFT = 1,
    RIGHT = -1,
};

struct Position{
    int x;
    int y;
    Position(int x, int y) : x(x), y(y) {}
    Position() : x(0), y(0) {}

    bool operator==(const Position& other) const{
        return x == other.x && y == other.y;
    }
};
struct TileID : Position {
    TileID(int x, int y) : Position(x, y) {}
    TileID() : Position(0, 0) {}
};

enum OBJECT_TYPE
{
    OBJECT_PACMAN = 0,
    OBJECT_ANDROID_PACMAN,
    OBJECT_MS_PACMAN,
    OBJECT_BLINKY,
    OBJECT_PINKY,
    OBJECT_INKY,
    OBJECT_CLYDE,
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
    OBJECT_TYPE_TOTAL
};
const int OBJECT_PIXEL = 42;
const int OBJECT_SIZE = 42;
//const std::string OBJECT_TEXTURE_SHEET = "../Assets";

class Object {
protected:
    Timer* timer;

    TileID tileID{};
    Position position{};

    OBJECT_TYPE objectType;
    TileID startTileID{};
    SDL_Texture* objectTexture{};

    SDL_Rect sourceRect{}, destRect{};
    SDL_Renderer* objectRenderer = nullptr;

    int objectWidth{};
    int objectHeight{};

    int velocity{};

    std::queue<Direction> directionQueue;
    std::deque<Position> lastPoint;
    int lastAlphaMod{};
    SDL_Rect lastDest{};

    Uint32 frame{};
    int frameCount{};

    int health{};

    bool CanMove = true;
    bool isDead = false;
public:
    explicit Object(SDL_Renderer *renderer = nullptr, OBJECT_TYPE type = OBJECT_TYPE_TOTAL, Timer *_timer = nullptr);
    ~Object();

    virtual void update();

    virtual void render() = 0;

    void move(Direction direction, int _velocity);

    virtual TileID getTileID();
    void setTileID(TileID _tileID);

    virtual Position getPosition();

    void setPosition(Position _position);

    bool checkPosition() const;

    virtual void speedAnimation();
    textureManager* objectManager = new textureManager();


};
#endif

