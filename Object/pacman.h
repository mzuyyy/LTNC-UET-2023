//
// Created by MZuy on 3/12/2023.
//

#ifndef BTL_PACMAN_H
#define BTL_PACMAN_H

#include "Object.h"
#include "../textureManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <string>
#include <map>
enum PACMAN_TYPE{
    CLASSIC = 0,
    ANDROID,
    MS,
    PACMAN_TYPE_TOTAL = 3,
};
enum PACMAN_STATE
{
    PACMAN_INIT_STATE = 0,
    PACMAN_NEW_STATE,
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
const int UPGRADE_TIME[PACMAN_POWER_STATE_TOTAL] = {0, 8000, 4000, 3000, 4000, 4000, 5000,  5500, 3000};
class Pacman : public Object{
public:
    Pacman(const std::string &textureSheet, SDL_Renderer *renderer, PACMAN_TYPE type = CLASSIC);
    ~Pacman();

    void update() override;
    void render() override;
    void setPacmanFrameClip();
    void queueDirection(Direction direction);
    void move() override;
    void stop();
    void setState(PACMAN_STATE state);

    void resetPosition(){
        tileID = startTilePosition;
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
    std::queue<Direction> getDirection(){
        return directionQueue;
    }
    void eatDot(){
        eatenDot++;
    };
    void setDead(){
        isDead = true;
    }
    void checkMove(bool check){
        CanMove = check;
        std::cerr << "Can Move ? " << CanMove << std::endl;
    }
private:
    PACMAN_STATE pacmanState = PACMAN_INIT_STATE;
    PACMAN_TYPE type;

    int pacmanHealth = 5;

    bool isDead = false;

    int pacmanVelocity = 1;

    bool CanMove = true;

    SDL_Renderer* pacmanRenderer = nullptr;

    SDL_Texture* pacmanTexture{};

    SDL_Rect destRect{};

    textureManager* pacmanManager = new textureManager();

    int widthFrame = 42;
    int heightFrame = 42;

    int pacmanWidth = 24;
    int pacmanHeight = 24;

    const TileID startTilePosition = *new TileID(13, 23);

    SDL_Rect frameClip[9]{};
    std::string pacmanTextureSheet = "../Assets/pacmanTexture.png";

    int eatenDot = 0;
};


#endif //BTL_PACMAN_H
