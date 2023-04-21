//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_STATE_H
#define BTL_STATE_H

#include "../Engine.h"

enum STATE_TYPE
{
    STATE_NULL = -1,
    MENU_STATE,
    PLAYING_STATE,
    HOW_TO_PLAY_STATE,
    SETTING_STATE,
    HIGHSCORE_STATE,
    ABOUT_STATE,
    RESUME_STATE,
    SAVE_SETTING_STATE,
    EXIT_GAME_STATE,
    GAME_OVER_STATE,
    WIN_GAME_STATE,
    CLOSE_STATE,
    RETURN_STATE,
    EXIT_STATE,
    STATE_TYPE_TOTAL
};
class State {
public:
    State(){
        stateType = STATE_NULL;
        engine = nullptr;
    };
    explicit State(Engine* engine){
        this->engine = engine;
    }
    virtual ~State(){
        delete engine;
        engine = nullptr;
    };

    virtual void init(Engine* engine) = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual void keyDown(int keyCode) = 0;
    virtual void keyUp(int keyCode) = 0;

    void pull(STATE_TYPE type){
        stateType = type;
    };
    STATE_TYPE request(){
        return stateType;
    };
    void pulled(){
        stateType = STATE_NULL;
    };
protected:
    STATE_TYPE stateType;
    Engine* engine;
};


#endif //BTL_STATE_H