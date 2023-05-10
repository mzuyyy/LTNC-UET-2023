//
// Created by MZuy on 4/14/2023.
//

#ifndef BTL_PLAYSTATE_H
#define BTL_PLAYSTATE_H
#include "State.h"
#include "../Objects/pacman.h"
#include "../Objects/ghost.h"
#include "../Objects/UpgradedGhost.h"
#include "../map.h"
#include "../logStatus.h"
#include "../Objects/Mystery.h"
#include "../GameStatus.h"

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
enum GHOST_STATE_TYPE{
    FRIGHTEN_STATE,
    BLIND_STATE,
    FREEZE_STATE,
    GHOST_STATE_TYPE_TOTAL
};
enum BUTTON {
    PAUSED_RESUME,
    PAUSED_EXIT,
    BUTTON_TOTAL
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

    void handleRenderMap();

    void handleGhostRender(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostUpdate(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostState(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostMovement(Ghost *ghost, UpgradedGhost *upgradedGhost);
    void handleGhostDead(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostInit(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostMode(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleGhostOutCage(int dotRequire, Ghost* ghost, UpgradedGhost* upgradedGhost);
    void setGhostState(Ghost *ghost, UpgradedGhost *upgradedGhost, GHOST_STATE_TYPE state);
    void handleGhostTarget(Ghost *ghost);
    void handleChaseTarget(Ghost *ghost);
    void handleGhostMove(Ghost *ghost);
    void handleGhostUpgrade(Ghost *ghost, UpgradedGhost *upgradedGhost);

    void handleUpgradedGhostTarget(UpgradedGhost *upgradedGhost);
    void handleUpgradedChaseTarget(UpgradedGhost *upgradedGhost);
    void handleUpgradedGhostMove(UpgradedGhost* upgradedGhost);

    void handleMysteryMove();

    HIT_TYPE handleGhostHit(Ghost *ghost, Pacman *_pacman);
    HIT_TYPE handleUpgradedGhostHit(UpgradedGhost *upgradedGhost);

    Direction calculateDirection(Ghost *ghost);
    Direction calculateDirection(UpgradedGhost* upgradedGhost);
    Direction calculateMysteryDirection();


    void initUpgraded(Ghost* ghost, UpgradedGhost* upgradedGhost);
    void handleUpgrade(Ghost* ghost, UpgradedGhost* upgradedGhost);

    void setControl();

    void keyDown(int code) override;
    void keyUp(int code) override;

    void updateButton(int change);
private:
    textureManager* playStateManager;

    Log* consolePlayState;
    int currentState{};
    int lastState{};

    int controlKey[CONTROL_DIRECTION_TOTAL]{};

    int currentDot = 0;

    SDL_Renderer* playStateRenderer;

    SDL_Texture* pauseTexture;
    SDL_Rect sourcePause{};
    SDL_Rect destPause{};
    int currentPauseButton{};
    bool isChoosing[2]{};

    SDL_Texture* buttonTexture;
    SDL_Rect buttonPauseFrameClip[2][BUTTON_TOTAL]{};
    SDL_Rect buttonDest[BUTTON_TOTAL]{};

    Pacman* pacman;

    Ghost* blinky;
    Ghost* clyde;
    Ghost* inky;
    Ghost* pinky;

    Mystery* mystery;

    UpgradedGhost* deadly;
    UpgradedGhost* invisy;
    UpgradedGhost* freezy;
    UpgradedGhost* speedy;

    Map* map;

    GameStatus* gameStatus;
};


#endif //BTL_PLAYSTATE_H
