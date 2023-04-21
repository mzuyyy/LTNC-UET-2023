//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_STATEMANAGER_H
#define BTL_STATEMANAGER_H
#include "../Engine.h"
const SDL_Rect MAIN_VIEWPORT = {4*3, 4*3, 224*3, 296*3};
const SDL_Rect PLAYING_VIEWPORT = {4*3, 28*3, 224*3, 248*3};
const SDL_Rect GAME_STATUS_VIEWPORT ={4*3, 4*3, 224*3, 296*3};
class StateManager {
public:
    StateManager(Engine* engine);
    ~StateManager();

    void add();
    void loop();
    void render();
};


#endif //BTL_STATEMANAGER_H
