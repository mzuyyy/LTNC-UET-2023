//
// Created by MZuy on 3/28/2023.
//

#ifndef BTL_GHOST_H
#define BTL_GHOST_H
#include "Object.h"

enum GHOST_TYPE{
    BLINKY,
    DEADLY,
    CLYDE,
    INKY,
    FREEZY,
    INVISY,
    PINKY,
    SPEEDY,
    MYSTERY,
    GHOST_TYPE_TOTAL
};
enum GHOST_STATE {
   GHOST_UNSET = -1,
   GHOST_INIT,
   GHOST_STAND,
   GHOST_CHASE,
   GHOST_SCATTER,
   GHOST_FRIGHTEN,
   GHOST_EATEN,
   GHOST_EAT,
   GHOST_REBORN,
   GHOST_BLIND,
   GHOST_FREEZE,
   GHOST_EXIT_CAGE,
   GHOST_ENTER_CAGE,
   GHOST_UPGRADE_STATE,
   GHOST_STATE_TOTAL,
};
const int GHOST_STATE_TIME[GHOST_STATE_TOTAL] = {
        0, 0,  20000, 10000, 12000, 1000, -1,
        360,5000, 7000, -1, -1, 3900,
};
enum GHOST_MODE{
    GHOST_NORMAL_MODE,
    GHOST_FRIGHTEN_MODE,
    GHOST_BLIND_MODE,
    GHOST_FREEZE_MODE,
    GHOST_MODE_TOTAL,
};
const int GHOST_MODE_TIME[GHOST_MODE_TOTAL] = {
        0, 10000, 5000, 7000,
};
const int FRIGHTENED_GHOST_ANIMATION_FRAME = 2;
enum FRIGHTENED_GHOST_SPRITE
{
    FRIGHTENED_GHOST_DEFAULT = 0,
    FRIGHTENED_GHOST_WHITE = 2,
    FRIGHTENED_GHOST_SPRITE_TOTAL = 4
};
const int EATEN_GHOST_ANIMATION_FRAME = 2;
enum EATEN_GHOST_SPRITE
{
    EATEN_GHOST_UP = 0,
    EATEN_GHOST_RIGHT,
    EATEN_GHOST_DOWN,
    EATEN_GHOST_LEFT,
    EATEN_GHOST_SPRITE_TOTAL
};
const int GHOST_ANIMATION_FRAME = 2;
const int GHOST_PIXEL = 42;
const int ANIMATION_SPEED = 100;
class Ghost : public Object{
private:
    GHOST_TYPE ghostType;

    SDL_Texture* ghostTexture = nullptr;

    textureManager* ghostManager = new textureManager();

    Log* consoleGhost = nullptr;

    std::queue<GHOST_STATE> stateQueue;
    std::queue<Uint32> startState;

    bool currentMode[GHOST_MODE_TOTAL]{};
    Uint32 startMode[GHOST_MODE_TOTAL]{};

    Position target;
    Position startPoint, scatter, stand, upgrade;
    Direction direction;

    int velocity{};

    bool isOutCage{};

    SDL_Rect ghostFrameClip[11]{};

    TileID defaultTileID = {0, 0};
public:
    Ghost(GHOST_TYPE type, TileID tile, SDL_Renderer *renderer);
    ~Ghost();

    void update() override;
    void render() override;
    void queueDirection(Direction dir);

    Position getPosition(){
        return position;
    }

    TileID getTileID(){
        return tileID;
    }
    void setGhostFrameClip();

};


#endif //BTL_GHOST_H
