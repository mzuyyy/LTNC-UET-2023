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
    INVISY,
    INKY,
    FREEZY,
    PINKY,
    SPEEDY,
    MYSTERY,
    GHOST_TYPE_TOTAL
};
enum GHOST_STATE {
   GHOST_UNSET = -1,
   GHOST_INIT,
   GHOST_STAND,
   GHOST_APPEAR,
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
   GHOST_UPGRADE,
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
    GHOST_SPEED_UP,
    GHOST_SLOW_DOWN,
    GHOST_INVISIBLE,
    GHOST_MODE_TOTAL,
};
const int GHOST_MODE_TIME[GHOST_MODE_TOTAL] = {
        0, 10000, 5000, 7000, 6500, 1000, 6000,
};
const int FRIGHTENED_GHOST_FRAME = 2;
const int EATEN_GHOST_FRAME = 1;
enum HIT_TYPE{
    NONE_HIT = 0,
    PACMAN_HIT,
    GHOST_HIT,
    HIT_TYPE_TOTAL,
};
const int GHOST_FRAME = 2;
const int GHOST_UPGRADE_FRAME = 3;
const Position UPGRADE_MYSTERY_POSITION = {11 * 24 + 3, 17 * 24 - 9};
const Position GHOST_UPGRADE_POSITION = {13 * 24 + 3, 17 * 24 - 9};
const Position UPGRADED_GHOST_APPEAR_POSITION = GHOST_UPGRADE_POSITION;
const TileID SCATTER_POSITION[GHOST_TYPE_TOTAL - 1] = {
        {27 * 24 + 3, -1 * 24 - 9},
        {27 * 24 + 3, -1 * 24 - 9},
        {0 * 24 + 3, 32 * 24 - 9},
        {0 * 24 + 3, 32 * 24 - 9},
        {27 * 24 + 3, 32 * 24 - 9},
        {27 * 24 + 3, 32 * 24 - 9},
        {0 * 24 + 3, -1 * 24 - 9},
        {0 * 24 + 3, -1 * 24 - 9},
};
const int ANIMATION_SPEED = 100;
const int GHOST_VELOCITY = 2;
const int GHOST_SPEED_UP_VELOCITY = 4;
const int GHOST_SLOW_DOWN_VELOCITY = 1;
const int EATEN_GHOST_VELOCITY = 8;
const std::string GHOST_TEXTURE_PATH[GHOST_TYPE_TOTAL] = {
        "../Assets/ghost/Blinky.png",
        "../Assets/ghost/Deadly.png",
        "../Assets/ghost/Clyde.png",
        "../Assets/ghost/Invisy.png",
        "../Assets/ghost/Inky.png",
        "../Assets/ghost/Freezy.png",
        "../Assets/ghost/Pinky.png",
        "../Assets/ghost/Speedy.png",
        "../Assets/ghost/Mystery.png",
};
const Position START_GHOST_POSITION[GHOST_TYPE_TOTAL] = {
        {13 * 24 + 3, 12 * 24 - 9},
        {13 * 24 + 3, 12 * 24 - 9},
        {11 * 24 + 3, 15 * 24 - 9},
        {11 * 24 + 3, 15 * 24 - 9},
        {13 * 24 + 3, 15 * 24 - 9},
        {13 * 24 + 3, 15 * 24 - 9},
        {15 * 24 + 3, 15 * 24 - 9},
        {15 * 24 + 3, 15 * 24 - 9},
        {-1 * 24 + 3, 15 * 24 - 9},
};
const TileID MYSTERY_GIVE_STRENGTH_TILE_ID = {11, 14};
class Ghost : public Object{
private:
    GHOST_TYPE ghostType;

    SDL_Texture* ghostTexture;

    textureManager* ghostManager;

    Log* consoleGhost;

    Direction currentDirection{};

    std::queue<GHOST_STATE> stateQueue;
    std::queue<Uint32> startState;

    bool currentMode[GHOST_MODE_TOTAL]{};
    Uint32 startMode[GHOST_MODE_TOTAL]{};

    Position target;
    Position startPoint, scatterPosition, standPosition;

    int velocity{};

    bool isOutCage{};

    SDL_Rect ghostFrameClip[11]{};

    bool isUpgrade = false;
public:
    Ghost(GHOST_TYPE type, SDL_Renderer *renderer, Timer *_timer) ;
    ~Ghost();

    void update() override;
    void render() override;
    void speedAnimation() override;
    void setGhostFrameClip();
    void move();
    void upgrade();
    void initState();
    void setState(GHOST_STATE newState);
    void handleState();
    void unsetState();
    void setMode(const GHOST_MODE& mode);
    void handleMode();
    void unsetMode(const GHOST_MODE& mode);

    Position getPosition() override{
        Position temp = position;
        temp.y -= 24 * 6;
        temp.x += 21;
        temp.y += 21;
        return temp;
    }
    Position getNextPosition(Direction _direction){
        Position temp = getPosition();
        switch (_direction) {
            case UP:
                temp.y -= 14;
                break;
            case DOWN:
                temp.y += 12;
                break;
            case LEFT:
                temp.x -= 14;
                break;
            case RIGHT:
                temp.x += 12;
                break;
            default:
                break;
        }
        return temp;
    }
    void checkMove(bool flag){
        CanMove = flag;
    }
    TileID getTileID() override{
        return tileID;
    }
    void setTarget(Position _target) {
        target = _target;
    }
    TileID getTarget() const{
        return {target.x / 24, target.y / 24};
    }
    void leaveCage(){
        isOutCage = true;
        setTileID({13, 11});
    };
    GHOST_TYPE getType(){
        return ghostType;
    }
    bool isUpgradeState() const{
        return isUpgrade;
    }
    const Position getStartPosition() const{
        return START_GHOST_POSITION[ghostType];
    }
    bool isGhostOutCage() const{
        return isOutCage;
    }
    Position getScatterPosition() const{
        return SCATTER_POSITION[ghostType];
    }
    GHOST_STATE getCurrentState() const{
        return stateQueue.empty() ? GHOST_UNSET : stateQueue.front();
    }
    Direction getDirection() const{
        return currentDirection;
    }
    void setDirection(Direction _direction){
        currentDirection = _direction;
    }
    bool isGhostMode(GHOST_MODE mode){
        return currentMode[mode];
    }
    bool canMove(){
        return CanMove;
    }
};


#endif //BTL_GHOST_H
