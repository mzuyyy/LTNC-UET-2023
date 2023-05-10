//
// Created by MZuy on 5/4/2023.
//

#ifndef BTL_UPGRADEDGHOST_H
#define BTL_UPGRADEDGHOST_H

#include "Object.h"
#include "pacman.h"

enum UPGRADED_GHOST_TYPE{
    DEADLY,
    INVISY,
    FREEZY,
    SPEEDY,
    UPGRADED_GHOST_TYPE_TOTAL
};
enum UPGRADED_GHOST_STATE {
    UPGRADED_GHOST_UNSET = -1,
    UPGRADED_GHOST_INIT,
    UPGRADED_GHOST_APPEAR,
    UPGRADED_GHOST_STAND,
    UPGRADED_GHOST_WAIT,
    UPGRADED_GHOST_CHASE,
    UPGRADED_GHOST_SCATTER,
    UPGRADED_GHOST_FRIGHTEN,
    UPGRADED_GHOST_IS_EATING,
    UPGRADED_GHOST_WAS_EATEN,
    UPGRADED_GHOST_REBORN,
    UPGRADED_GHOST_BLIND,
    UPGRADED_GHOST_FREEZE,
    UPGRADED_GHOST_EXIT_CAGE,
    UPGRADED_GHOST_ENTER_CAGE,
    UPGRADED_GHOST_STATE_TOTAL,
};
const int UPGRADED_GHOST_STATE_TIME[UPGRADED_GHOST_STATE_TOTAL] = {
        0, 1500, 0, 0, 30000, 10500, 12450,
        1400, -1, 360, 6000, 8400, -1, -1
};
enum UPGRADED_GHOST_MODE
{
    UPGRADED_GHOST_NORMAL = 0,
    UPGRADED_GHOST_SPEED_UP_MODE,
    UPGRADED_GHOST_SLOW_DOWN_MODE,
    UPGRADED_GHOST_INVISIBLE_MODE,
    UPGRADED_GHOST_FRIGHTEN_MODE,
    UPGRADED_GHOST_BLIND_MODE,
    UPGRADED_GHOST_FREEZE_MODE,
    UPGRADED_GHOST_MODE_TOTAL
};
const int UPGRADED_GHOST_MODE_TIME[UPGRADED_GHOST_MODE_TOTAL] = {
        0, 3000, 600, 7800,
        12450, 6000, 8400,
};
const Position UPGRADED_GHOST_APPEAR_POSITION = {13 * 24 + 3, 17 * 24 - 9};
const int UPGRADED_GHOST_FRAME = 2;
const int UPGRADED_GHOST_VELOCITY = 2;
const int UPGRADED_GHOST_SPEED_UP_VELOCITY = 4;
const int UPGRADED_GHOST_SLOW_DOWN_VELOCITY = 1;
const std::string UPGRADED_GHOST_TEXTURE_PATH[UPGRADED_GHOST_TYPE_TOTAL] = {
        "../Assets/ghost/Deadly.png",
        "../Assets/ghost/Invisy.png",
        "../Assets/ghost/Freezy.png",
        "../Assets/ghost/Speedy.png",
};
const Position START_UPGRADED_GHOST_POSITION[UPGRADED_GHOST_TYPE_TOTAL] = {
        {13 * 24 + 3, 12 * 24 - 9},
        {11 * 24 + 3, 15 * 24 - 9},
        {13 * 24 + 3, 15 * 24 - 9},
        {15 * 24 + 3, 15 * 24 - 9},
};
const Position UPGRADED_SCATTER_POSITION[UPGRADED_GHOST_TYPE_TOTAL] = {
        {28 * 24, 5 * 24},
        {0 * 24, 32 * 24},
        {28 * 24, 32 * 24},
        {0 * 24, 5 * 24},
};

class UpgradedGhost : public Object {
private:
    const int UPGRADED_ANIMATION_SPEED = 100;
    const int UPGRADED_FRIGHTENED_GHOST_FRAME = 2;

    UPGRADED_GHOST_TYPE ghostType;

    SDL_Texture* ghostTexture;

    textureManager* ghostManager;

    Log* consoleGhost;

    Direction currentDirection{};

    std::queue<UPGRADED_GHOST_STATE> stateQueue;
    std::queue<Uint32> startState;

    bool currentMode[UPGRADED_GHOST_MODE_TOTAL]{};
    Uint32 startMode[UPGRADED_GHOST_MODE_TOTAL]{};

    Position target;

    int velocity{};

    bool isOutCage{};

    SDL_Rect ghostFrameClip[11]{};

    bool isAppear;
public:
    UpgradedGhost(UPGRADED_GHOST_TYPE type, SDL_Renderer *renderer, Timer *timer);
    ~UpgradedGhost();

    void init();
    void update() override;
    void render() override;
    void speedAnimation() override;
    void setGhostFrameClip();
    void move();
    void upgrade();
    void initState();
    void setState(UPGRADED_GHOST_STATE newState);
    void handleState();
    void unsetState();
    void setMode(const UPGRADED_GHOST_MODE& mode);
    void handleMode();
    void unsetMode(const UPGRADED_GHOST_MODE& mode);

    bool checkCollision(Pacman* _pacman) const {
        return (position.x - 21 <= _pacman->getPosition().x) ||
               (position.y >= _pacman->getPosition().y - 21) ||
               (position.x >= _pacman->getPosition().x - 21) ||
               (position.y - 21 <= _pacman->getPosition().y);
    }

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
    TileID getTileID() override{
        return tileID;
    }
    void setCanMove(bool _canMove){
        CanMove = _canMove;
    }
    void setTarget(Position _target) {
        target = _target;
    }
    Position getTarget() const{
        return {target.x, target.y};
    }
    void leaveCage(){
        isOutCage = true;
        setTileID({13, 11});
    };
    UPGRADED_GHOST_TYPE getType(){
        return ghostType;
    }
    bool isUpgradedGhostAppear() const{
        return isAppear;
    }
    Position getDeadlyStartPosition() const{
        Position temp = START_UPGRADED_GHOST_POSITION[DEADLY];
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    Position getStartPosition() const{
        Position temp = START_UPGRADED_GHOST_POSITION[ghostType];
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    bool isUpgradedGhostOutCage() const{
        return isOutCage;
    }
    Position getScatterPosition() const{
        return UPGRADED_SCATTER_POSITION[ghostType];
    }
    UPGRADED_GHOST_STATE getCurrentState() const{
        return stateQueue.empty() ? UPGRADED_GHOST_UNSET : stateQueue.front();
    }
    Direction getDirection() const{
        return currentDirection;
    }
    void setDirection(Direction _direction){
        currentDirection = _direction;
    }
    bool isGhostMode(UPGRADED_GHOST_MODE mode){
        return currentMode[mode];
    }
    bool canMove(){
        return CanMove;
    }
};


#endif //BTL_UPGRADEDGHOST_H
