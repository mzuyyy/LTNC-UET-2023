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

enum PACMAN_STATE {
    PACMAN_INIT_STATE = 0,
    PACMAN_NEW_STATE,
    PACMAN_STAND_STATE,
    PACMAN_RUNNING_STATE,
    PACMAN_EATING_STATE,
    PACMAN_DEAD_STATE,
    PACMAN_STATE_TOTAL
};
enum PACMAN_POWER {
    NORMAL_PACMAN = 0,
    POWER_PACMAN,
    CONFUSED_PACMAN,
    SPEED_PACMAN,
    SLOW_DOWN_PACMAN,
    INVISIBLE_PACMAN,
    BLIND_PACMAN,
    FROZEN_PACMAN,
    FREEZE_PACMAN,
    PACMAN_POWER_TOTAL
};
enum PACMAN_TYPE{
    CLASSIC,
    MS,
    ANDROID,
    PACMAN_TYPE_TOTAL,
};
class Pacman : public Object {
public:
    Pacman(SDL_Renderer *renderer, PACMAN_TYPE type, Timer *_timer);

    ~Pacman();

    void update() override;
    void render() override;
    void speedAnimation() override;
    void setPacmanFrameClip();
    void queueDirection(Direction direction);
    void move(Direction _direction, int _velocity);
    void stop();
    void setState(PACMAN_STATE state);
    void handleState();
    void setPower(PACMAN_POWER type);
    void removePower(PACMAN_POWER type);
    void handlePower();

    PACMAN_TYPE getType(){
        return pacmanType;
    }
    SDL_Texture* getTexture(){
        return pacmanTexture;
    };
    PACMAN_STATE getState(){
        return pacmanState;
    }
    void resetPosition(){
        tileID = startTileID;
    }
    Position getPosition() override{
        Position temp = position;
        temp.y -= 24 * 6;
        temp.x += 21;
        temp.y += 21;
        return temp;
    }
    Position getNextPosition(int flag = 1) {
        Position temp = getPosition();
        if (flag)
        switch (directionQueue.empty() ? NONE : directionQueue.front()) {
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
        else
            switch (directionQueue.empty() ? NONE : directionQueue.back()) {
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
    TileID getNextTileID() {
        TileID temp = tileID;
        switch (directionQueue.empty() ? NONE : directionQueue.front()) {
            case UP:
                temp.y--;
                break;
            case DOWN:
                temp.y++;
                break;
            case LEFT:
                temp.x--;
                break;
            case RIGHT:
                temp.x++;
                break;
            default:
                break;
        }
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
    int getEatenDot(){
        return eatenDot;
    }
    bool isPacmanDead() const{
        return isDead;
    }
    void setDead(){
        isDead = true;
    }
    void gainHealth(){
        pacmanHealth++;
    }
    void loseHealth(){
        pacmanHealth--;
    }
    int getHealth() const{
        return pacmanHealth;
    }
    void checkMove(bool check){
        CanMove = check;
    }
    bool isPacmanPower(PACMAN_POWER _power){
        return power[_power];
    }
private:
    const int PACMAN_EATING_STATE_TIME = 1000;
    const int UPGRADE_TIME[PACMAN_POWER_TOTAL] = {0, 8000, 4000, 3000, 4000, 4000, 5000, 5500, 3000};
    const int PACMAN_VELOCITY = 2;
    const int PACMAN_VELOCITY_SLOW = 1;
    const int PACMAN_MAX_HEALTH = 5;
    const int PACMAN_FRAME_VALUE = 5;
    const int PACMAN_SLOW_FRAME_VALUE = 10;
    const int PACMAN_DEATH_ANIMATION_FRAME = 11;
    const int PACMAN_ANIMATION_SPEED = 75;
    const int MS_PIXEL = 45;

    const std::string PACMAN_TEXTURE_SHEET = "../Assets/Pacman/pacmanTexture.png";
    const std::string ANDROID_TEXTURE_SHEET = "../Assets/Pacman/androidPacmanTexture.png";
    const std::string MS_TEXTURE_SHEET = "../Assets/Pacman/msPacmanTexture.png";
    const std::string DEAD_PACMAN_TEXTURE_SHEET = "../Assets/Pacman/PacmanDead.png";

    bool power[PACMAN_POWER_TOTAL]{};
    Uint32 startPower[PACMAN_POWER_TOTAL]{};
    PACMAN_STATE pacmanState;

    const PACMAN_STATE pacmanStartState = PACMAN_INIT_STATE;

    int pacmanHealth = PACMAN_MAX_HEALTH;

    SDL_Renderer* pacmanRenderer = nullptr;

    SDL_Texture* pacmanTexture{};

    PACMAN_TYPE pacmanType;

    textureManager* pacmanManager = new textureManager();

    const TileID startTileID = {13, 23};

    SDL_Rect frameClip[12]{};

    Uint32 eatenDot = 0;

    Uint32 startState{};
};


#endif //BTL_PACMAN_H
