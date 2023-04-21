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
    CONTROL_TYPE_TOTAL,
};
const int CONTROL_TYPE[CONTROL_TYPE_TOTAL][4] = {
        {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT},
        {SDLK_w, SDLK_s, SDLK_a, SDLK_d},
};
class ControlManager {
public:
    ControlManager();
    ~ControlManager();

    int getControl(SDL_KeyCode controlDirection){
        return keyCode[controlDirection];
    };
    void setControlType(int type);
private:
    Log* consoleControlManager = nullptr;

    int keyCode[4];
};


#endif //BTL_CONTROLMANAGER_H
