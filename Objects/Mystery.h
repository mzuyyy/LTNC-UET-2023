//
// Created by MZuy on 4/27/2023.
//

#ifndef BTL_MYSTERY_H
#define BTL_MYSTERY_H

#include "Object.h"

enum MYSTERY_STATE
{
    MYSTERY_INIT,
    MYSTERY_STAND,
    MYSTERY_MOVING,
    MYSTERY_WAITING,
    MYSTERY_SPEECH,
    MYSTERY_RETURNING,
    MYSTERY_STATE_TOTAL
};
const int MYSTERY_STATE_TIME[MYSTERY_STATE_TOTAL] = {
        0, 0, 0, 1800, 7800,  0
};

const int MYSTERY_VELOCITY = 1;
const int MYSTERY_FRAME = 8;

const Position START_MYSTERY_POSITION = {-1 * 24 + 3, 17 * 24 - 9};
const Position MYSTERY_POSITION = {11 * 24 + 3, 17 * 24 - 9};

const std::string MYSTERY_PATH = "../Assets/ghost/Mystery.png";
const std::string SPEECH_PATH = "../Assets/Speech.png";

const Position SPEECH_POSITION = {11 * 24, 13 * 24};

const int ANIMATION_SPEED = 75;

const int SPEECH_WIDTH = 132;
const int SPEECH_HEIGHT = 78;
class Mystery : public Object{
private:
    Log* consoleMystery{};

    textureManager* mysteryManager{};

    SDL_Rect frameClip[8];
    SDL_Rect speechClip[4];

    SDL_Texture* mysteryTexture{};
    SDL_Texture* speechTexture{};

    MYSTERY_STATE currentState;
    Uint32 startState{};
    int currentSpeech{};

    Position target;

    Direction currentDirection;
public:
    Mystery(SDL_Renderer *_renderer, Timer *_timer);
    ~Mystery();

    void update() override;
    void render() override;

    void initState();
    void setState(MYSTERY_STATE _state);
    void handleState();

    void move();
    void setFrameClip();

    Position getTarget() const{
        return target;
    }
    void setTarget(Position _target){
        target = _target;
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
    Position getStartPosition() const{
        Position temp = START_MYSTERY_POSITION;
        temp.x += 21;
        temp.y += 21;
        temp.y -= 24;
        return temp;
    }
    MYSTERY_STATE getCurrentState() const{
        return currentState;
    }
    Direction getDirection() const{
        return currentDirection;
    }
    void setDirection(Direction _direction){
        currentDirection = _direction;
    }
    int getVelocity(){
        return velocity;
    }
};


#endif //BTL_MYSTERY_H
