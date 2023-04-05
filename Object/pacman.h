//
// Created by MZuy on 3/12/2023.
//

#ifndef BTL_PACMAN_H
#define BTL_PACMAN_H

#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <string>
#include <map>

enum PACMAN_TYPE{
    CLASSIC = 0,
    ANDROID,
    PACMAN_TYPE_TOTAL
};
enum PACMAN_STATE
{
    PACMAN_START_STATE = 0,
    PACMAN_NEW_STATE,
    PACMAN_STAND_STATE,
    PACMAN_RUNNING_STATE,
    PACMAN_EATING_STATE,
    PACMAN_DEATH_STATE,
    PACMAN_STATE_TOTAL
};
enum PACMAN_POWER_STATE
{
    NORMAL_PACMAN = 0,
    POWER_PACMAN,
    CONFUSED_PACMAN,
    SPEED_PACMAN,
    SLOW_DOWN_PACMAN,
    INVISIBLE_PACMAN,
    BLIND_PACMAN,
    FREE_TIME_PACMAN,
    FREEZE_PACMAN,
    PACMAN_POWER_STATE_TOTAL
};
const int PACMAN_EATING_STATE_TIME = 960;
const int UPGRADE_TIME[PACMAN_POWER_STATE_TOTAL] = {0, 8000, 4000, 3000, 4000, 4000, 5000,  5500, 3000};
const int PACMAN_VELOCITY = 2;
const int PACMAN_VELOCITY_SLOW = 1;
const int PACMAN_MAX_HEALTH = 5;
const int PACMAN_FRAME_VALUE = 5;
const int PACMAN_SLOW_FRAME_VALUE = 10;
const int PACMAN_DEATH_ANIMATION_FRAME = 11;
const int PACMAN_DEATH_SPRITE_TOTAL = 11;
const int PACMAN_ANIMATION_SPEED = 100;

const std::string CLASSIC_PACMAN_TEXTURE_SHEET = "../Assets/classicPacmanTexture.png";
const std::string ANDROID_PACMAN_TEXTURE_SHEET = "../Assets/androidPacmanTexture.png";
const std::string DEAD_PACMAN_TEXTURE_SHEET = "../Assets/PacmanDead.png";

class Pacman : public Object{
public:
    Pacman(SDL_Renderer *renderer, PACMAN_TYPE type);

    ~Pacman();

    void update() override;
    void render() override;
    void setPacmanFrameClip();
    void queueDirection(Direction direction);
    void move(Position velocity) override;
    void stop();
    void setState(PACMAN_STATE state);
    void handleState();
    void setPower(PACMAN_POWER_STATE type);
    void removePower(PACMAN_POWER_STATE type);
    void handlePower();

    PACMAN_STATE getState(){
        return pacmanState;
    }
    void resetPosition(){
        tileID = startTileID;
    }
    Position getPosition() override{
        Position temp = position;
        temp.y -= 51;
        return position;
    }
    TileID getTileID() override{
        return tileID;
    }
    TileID getNextTileID(Direction dir){
        TileID temp = tileID;
        if (dir == UP)
            temp.y--;
        else if (dir == DOWN)
            temp.y++;
        else if (dir == LEFT)
            temp.x--;
        else if (dir == RIGHT)
            temp.x++;
        return temp;
    }
    std::queue<Direction> getDirectionQueue(){
        return directionQueue;
    }
    Direction getDirection(){
        return (directionQueue.empty() ? NONE : directionQueue.front());
    }
    Direction getNextDirection(){
        return directionQueue.empty() ? NONE : directionQueue.back();
    }
    void eatDot(){
        eatenDot++;
    };
    void eatFruit(){
        eatenFruit++;
    }
    void setDead(){
        isDead = true;
    }
    void checkMove(bool check){
        CanMove = check;
        std::cerr << "Can Move ? " << CanMove << std::endl;
    }
    void setPacmanType (PACMAN_TYPE type){
        pacmanType = type;
    }
private:
    bool power[PACMAN_POWER_STATE_TOTAL]{};
    Uint32 startPower[PACMAN_POWER_STATE_TOTAL]{};
    PACMAN_STATE pacmanState;
    PACMAN_TYPE pacmanType;

    const PACMAN_STATE pacmanStartState = PACMAN_START_STATE;

    int pacmanHealth = PACMAN_MAX_HEALTH;

    bool isDead = false;

    bool CanMove = true;

    Uint32 pacmanVelocity{};

    SDL_Renderer* pacmanRenderer = nullptr;

    SDL_Texture* pacmanTexture{};
    SDL_Texture* classicPacmanTexture{};
    SDL_Texture* pacmanDeadTexture{};
    SDL_Texture* androidPacmanTexture{};

    textureManager* pacmanManager = new textureManager();

    int widthFrame = 42;
    int heightFrame = 42;

    int pacmanWidth = 24;
    int pacmanHeight = 24;

    const TileID startTileID = {13,23};

    SDL_Rect frameClip[12]{};

    Uint32 eatenDot = 0;
    Uint32 eatenFruit = 0;

    Uint32 startState{};
};


#endif //BTL_PACMAN_H
