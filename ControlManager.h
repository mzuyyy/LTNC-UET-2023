//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_CONTROLMANAGER_H
#define BTL_CONTROLMANAGER_H

#include <SDL.h>
#include "logStatus.h"
enum CONTROL_TYPE{
    ARROW = 0,
    WASD,
    IJKL,
    CONTROL_TYPE_TOTAL,
};
enum CONTROL_DIRETION{
    CONTROL_UP,
    CONTROL_DOWN,
    CONTROL_LEFT,
    CONTROL_RIGHT,
    CONTROL_DIRECTION_TOTAL,
};
const int CONTROL_TYPE[CONTROL_TYPE_TOTAL][CONTROL_DIRECTION_TOTAL] = {
        {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT},
        {SDLK_w, SDLK_s, SDLK_a, SDLK_d},
        {SDLK_i, SDLK_j, SDLK_k, SDLK_l}
};
class ControlManager {
public:
    ControlManager();
    ~ControlManager();

    int getControl(CONTROL_DIRETION direction){
        return keyCode[direction];
    };
    void setControlType(int type);
private:
    Log* consoleControlManager = nullptr;

    int keyCode[CONTROL_DIRECTION_TOTAL];
};


#endif //BTL_CONTROLMANAGER_H
