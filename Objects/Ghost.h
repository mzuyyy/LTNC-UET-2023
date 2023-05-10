//
// Created by MZuy on 3/28/2023.
//

#ifndef BTL_GHOST_H
#define BTL_GHOST_H

#include "Object.h"
#include "pacman.h"
enum GHOST_TYPE{
    BLINKY,
    CLYDE,
    INKY,
    PINKY,
    GHOST_TYPE_TOTAL
};
enum GHOST_STATE {
   GHOST_UNSET = -1,
   GHOST_INIT,
   GHOST_STAND,
   GHOST_CHASE,
   GHOST_SCATTER,
   GHOST_FRIGHTEN,
   GHOST_IS_EATEN,
   GHOST_WAS_EATEN,
   GHOST_REBORN,
   GHOST_BLIND,
   GHOST_FREEZE,
   GHOST_EXIT_CAGE,
   GHOST_ENTER_CAGE,
   GHOST_UPGRADE,
   GHOST_STATE_TOTAL,
};
const int GHOST_STATE_TIME[GHOST_STATE_TOTAL] = {
        0, 0, 30000, 10500, 12450, 1440,-1,
        360, 6000, 8400, -1, -1, 4800,
};
enum GHOST_MODE{
    GHOST_NORMAL_MODE,
    GHOST_FRIGHTEN_MODE,
    GHOST_BLIND_MODE,
    GHOST_FREEZE_MODE,
    GHOST_MODE_TOTAL,
};
const int GHOST_MODE_TIME[GHOST_MODE_TOTAL] = {
        0, 12450, 6000, 8400,
};
const int FRIGHTENED_GHOST_FRAME = 2;
const int EATEN_GHOST_FRAME = 1;
enum HIT_TYPE{
    NONE_HIT = 0,
    GHOST_HIT,
    PACMAN_HIT,
    HIT_TYPE_TOTAL,
};
const int GHOST_FRAME = 2;
const int GHOST_UPGRADE_FRAME = 3;
const Position GHOST_UPGRADE_POSITION = {13 * 24 + 3, 17 * 24 - 9};
const Position SCATTER_POSITION[GHOST_TYPE_TOTAL] = {
        {28 * 24, 5 * 24},
        {0 * 24, 32 * 24},
        {28 * 24, 32 * 24},
        {0 * 24, 5 * 24},
};
const int GHOST_ANIMATION_SPEED = 100;
const int GHOST_VELOCITY = 1;
const std::string DEAD_N_FRIGHTEN_TEXTURE_PATH = "../Assets/ghost/Ghost.png";
const std::string GHOST_TEXTURE_PATH[GHOST_TYPE_TOTAL] = {
        "../Assets/ghost/Blinky.png",
        "../Assets/ghost/Clyde.png",
        "../Assets/ghost/Inky.png",
        "../Assets/ghost/Pinky.png",
};
const Position START_GHOST_POSITION[GHOST_TYPE_TOTAL] = {
        {13 * 24 + 3, 12 * 24 - 9},
        {11 * 24 + 3, 15 * 24 - 9},
        {13 * 24 + 3, 15 * 24 - 9},
        {15 * 24 + 3, 15 * 24 - 9},
};

class Ghost : public Object{
private:
    GHOST_TYPE ghostType;

    SDL_Texture* ghostTextures[GHOST_TYPE_TOTAL];
    SDL_Texture* textures;
    SDL_Texture* ghostTexture;

    textureManager* ghostManager;

    Log* consoleGhost;

    Direction currentDirection{};

    std::queue<GHOST_STATE> stateQueue;
    std::queue<Uint32> startState;

    bool currentMode[GHOST_MODE_TOTAL]{};
    Uint32 startMode[GHOST_MODE_TOTAL]{};

    Position target;

    int velocity{};

    bool isOutCage{};

    SDL_Rect ghostFrameClip[11]{};

    bool isUpgrade = false;

    GHOST_STATE lastGhostState{};
public:
    Ghost(GHOST_TYPE type, SDL_Renderer *renderer, Timer *_timer) ;
    ~Ghost();

    void update() override;
    void render() override;
    //void speedAnimation() override;
    void setGhostFrameClip();
    void move();
    //void upgrade();
    void initState();
    void setState(GHOST_STATE newState);
    void handleState();
    void unsetState();
    void setMode(const GHOST_MODE& mode);
    void handleMode();
    void unsetMode(const GHOST_MODE& mode);

    void setUpgrade(bool flag){
        isUpgrade = flag;
    }
    bool checkCollision(Pacman* _pacman) {
        return (getPosition().x == _pacman->getPosition().x && abs(getPosition().y - _pacman->getPosition().y) <= 21) ||
        (getPosition().y == _pacman->getPosition().y && abs(getPosition().x - _pacman->getPosition().x) <= 21);
    }
    GHOST_STATE getLastState(){
        return lastGhostState;
    }
    Position getPosition() override {
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
    Position getTarget() const{
        return {target.x, target.y};
    }
    void leaveCage(){
        isOutCage = true;
        Position temp = START_GHOST_POSITION[BLINKY];
        temp.y += 24 * 5;
        setPosition(temp);
    };
    GHOST_TYPE getType(){
        return ghostType;
    }
    bool isGhostUpgrade() const{
        return isUpgrade;
    }
    Position getUpgradePosition() const{
        Position temp = GHOST_UPGRADE_POSITION;
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    Position getBlinkyStartPosition() const{
        Position temp = START_GHOST_POSITION[BLINKY];
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    Position getStartPosition() const{
        Position temp = START_GHOST_POSITION[ghostType];
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    bool isGhostOutCage() const{
        return isOutCage;
    }
    Position getScatterPosition() const{
        Position temp = SCATTER_POSITION[ghostType];
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
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
    int getVelocity(){
        return velocity;
    }
};


#endif //BTL_GHOST_H
