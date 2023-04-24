//
// Created by MZuy on 4/5/2023.
//

#include "StateManager.h"
#include "PlayState.h"

StateManager::StateManager(Engine* _engine, SDL_Renderer* renderer) {
    engine = _engine;
    stateManagerRenderer = renderer;
    stackState(new PlayState(renderer));
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
        temp = nullptr;
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
        stateManagerConsole->updateStatus("Getting key down...");
        states.top()->keyDown(code);
    }
}

void StateManager::keyUp(int code) {
    if(!states.empty())
        states.top()->keyUp(code);
}
bool StateManager::pullRequest() {
    if(!states.empty()){
        STATE_TYPE type = states.top()->request();
        states.top()->pulled();
        switch(type){
            case MENU_STATE:
                close();
                break;
            case PLAY_STATE:
                stateManagerConsole->updateStatus("play state");
                stackState(new PlayState(stateManagerRenderer));
                break;
            case STATE_NULL:
                break;
            case HOW_TO_PLAY_STATE:
                break;
            case SETTING_STATE:
                break;
            case HIGHSCORE_STATE:
                break;
            case ABOUT_STATE:
                break;
            case RESUME_STATE:
                break;
            case SAVE_SETTING_STATE:
                break;
            case EXIT_GAME_STATE:
                break;
            case GAME_OVER_STATE:
                break;
            case WIN_GAME_STATE:
                break;
            case CLOSE_STATE:
                break;
            case RETURN_STATE:
                break;
            case EXIT_STATE:
                break;
        }
    }
}
StateManager::~StateManager() {
    stateManagerConsole->updateStatus("Detroying state manager");
    delete stateManagerConsole;
    stateManagerConsole = nullptr;
    delete engine;
    engine = nullptr;
}







