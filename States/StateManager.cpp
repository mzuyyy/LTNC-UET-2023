//
// Created by MZuy on 4/5/2023.
//

#include "StateManager.h"
#include "PlayState.h"
#include "MenuState.h"

StateManager::StateManager(Engine* _engine, SDL_Renderer* renderer) {
    engine = _engine;
    stateManagerRenderer = renderer;
    stackState(new MenuState(renderer));
    stateManagerConsole = new Log("StateManager");
    stateManagerConsole->updateStatus("Created state manager");
}
void StateManager::stackState(State *state) {
    states.emplace(state);
    states.top()->init(engine);
}
void StateManager::close(){
    while (!states.empty()){
        State* temp = states.top();
        states.top()->close();
        states.pop();

        delete temp;
    }
}

void StateManager::update() {
    if(!states.empty())
        states.top()->update();
}
void StateManager::render() {
    if (!states.empty())
        states.top()->render();
}
void StateManager::keyDown(int code) {
    if(!states.empty()){
        states.top()->keyDown(code);
    }
}

void StateManager::keyUp(int code) {
    if(!states.empty())
        states.top()->keyUp(code);
}
bool StateManager::pullRequest() {
    bool isRunning = true;
    if(!states.empty()){
        STATE_TYPE type = states.top()->request();
        states.top()->pulled();
        switch(type){
            case MENU_STATE:
                close();
                stackState(new MenuState(stateManagerRenderer));
                break;
            case PLAY_STATE:
                stateManagerConsole->updateStatus("play state");
                stackState(new PlayState(stateManagerRenderer, engine));
                break;
            case PAUSED_STATE:{
                auto* newMenuState = new MenuState(stateManagerRenderer);
                //newMenuState->setMenuState(PAUSE);
                stackState(newMenuState);
                delete newMenuState;
                break;
            }
            case GAME_OVER_STATE: {
                auto *newMenuState = new MenuState(stateManagerRenderer);
                newMenuState->setMenuState(GAME_OVER);
                stackState(newMenuState);
                delete newMenuState;
                break;
            }
            case WIN_GAME_STATE: {
                auto *newMenuState = new MenuState(stateManagerRenderer);
                newMenuState->setMenuState(YOU_WIN);
                stackState(newMenuState);
                delete newMenuState;
                break;
            }
            case CLOSE_STATE:
                close();
                break;
            case RETURN_STATE:

                break;
            case EXIT_STATE:
                isRunning = false;
                break;
        }
    }
    return isRunning;
}
StateManager::~StateManager() {
    stateManagerConsole->updateStatus("Destroying state manager");
    delete stateManagerConsole;
    stateManagerConsole = nullptr;
    delete engine;
    engine = nullptr;
}







