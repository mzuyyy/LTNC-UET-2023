//
// Created by MZuy on 4/14/2023.
//

#ifndef BTL_PLAYSTATE_H
#define BTL_PLAYSTATE_H
#include "State.h"
#include "../Objects/pacman.h"
#include "../Objects/ghost.h"
#include "../map.h"
#include "../logStatus.h"

enum PLAY_STATE_TYPE{
    NEW_GAME = 0,
    NEW_LIFE,
    PLAYING,
    UPGRADE_GHOST,
    PAUSED,
    NEXT_LEVEL,
    END_GAME,
    WIN,
    PLAY_STATE_TYPE_TOTAL,
};
class PlayState : public State{
public:
    PlayState(SDL_Renderer *renderer);
    ~PlayState();

    void init(Engine* _engine) override;
    void update() override;
    void render() override;
    void close() override;
    void setState(PLAY_STATE_TYPE state);
    void handleState();

    void setControl();

    void keyDown(const int code);
    void keyUp(const int code);
private:
    Log* consolePlayState = nullptr;
    int currentState{};

    int controlKey[CONTROL_DIRECTION_TOTAL]{};

    int dotCurrent = 0;
    Pacman* pacman = nullptr;
    Ghost* blinky = nullptr;
    Ghost* clyde = nullptr;
    Ghost* inky = nullptr;
    Ghost* pinky = nullptr;
    Map* map = nullptr;
};


#endif //BTL_PLAYSTATE_H
