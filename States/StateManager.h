//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_STATEMANAGER_H
#define BTL_STATEMANAGER_H

#include "State.h"
#include <stack>

const SDL_Rect MAIN_VIEWPORT = {4*3, 4*3, 224*3, 296*3};
const SDL_Rect PLAYING_VIEWPORT = {4*3, 28*3, 224*3, 248*3};
const SDL_Rect GAME_STATUS_VIEWPORT ={4*3, 4*3, 224*3, 296*3};

class StateManager {
public:
    explicit StateManager(Engine* _engine, SDL_Renderer* renderer);
    ~StateManager();

    void stackState(State *state);
    void close();
    void update();
    void render();
    void keyDown(int code);
    void keyUp(int code);
    bool pullRequest();

private:
    Log* stateManagerConsole;
    Engine* engine;
    std::stack<State*> states;

    SDL_Renderer* stateManagerRenderer;
};


#endif //BTL_STATEMANAGER_H
