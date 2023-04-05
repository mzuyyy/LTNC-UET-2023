//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_STATE_H
#define BTL_STATE_H

#include "Engine.h"

enum STATE_TYPE
{
    STATE_NULL = 0,
    STATE_MENU,
    STATE_GAME,
    STATE_GAME_OVER,
    STATE_TYPE_TOTAL
};
class State {
public:
    State(){
        stateType = STATE_NULL;
        engine = nullptr;
    };
    State(Engine* engine){
        this->engine = engine;
    }
    virtual ~State(){
        delete engine;
        engine = nullptr;
    };

    virtual void init(Engine* engine) = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual void keyDown(const int keyCode) = 0;
    virtual void keyUp(const int keyCode) = 0;

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
