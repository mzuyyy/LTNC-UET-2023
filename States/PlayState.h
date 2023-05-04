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

struct Distance {
    static double Euclidean(const Position& a, const Position& b){
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }
    static int Manhattan(const Position& a, const Position& b){
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
};
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
    PlayState(SDL_Renderer *renderer, Engine *_engine);
    ~PlayState();

    void init(Engine* _engine) override;
    void update() override;
    void render() override;
    void close() override;
    void setState(PLAY_STATE_TYPE state);
    void handleState();

    void handleGhostTarget(Ghost *ghost);
    void handleChaseTarget(Ghost *ghost);
    static Direction calculateDirection(Ghost *ghost);
    static void handleGhostMove(Ghost *ghost);
    static HIT_TYPE handleGhostHit(Ghost* ghost, Pacman* pacman);


    void setControl();

    void keyDown(const int code) override;
    void keyUp(const int code) override;
private:
    Log* consolePlayState;
    int currentState{};

    int controlKey[CONTROL_DIRECTION_TOTAL]{};

    int dotCurrent = 0;
    Pacman* pacman;

    Ghost* blinky;
    Ghost* clyde;
    Ghost* inky;
    Ghost* pinky;
    Ghost* mystery;

    Map* map;
};


#endif //BTL_PLAYSTATE_H
