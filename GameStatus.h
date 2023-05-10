//
// Created by MZuy on 5/7/2023.
//

#ifndef BTL_GAMESTATUS_H
#define BTL_GAMESTATUS_H

#include <vector>
#include "logStatus.h"
#include "Objects/pacman.h"
#include "Objects/ghost.h"
#include "Engine.h"
#include "Timer.h"

enum UPDATED_SCORE{
    NORMAL_DOT,
    BIG_DOT,
    GHOST_EATEN,
    UPDATED_SCORE_TOTAL,
};

const Uint32 SCORE_NORMAL_DOT = 10;
const Uint32 SCORE_BIG_DOT = 50;
const Uint32 SCORE_GHOST_EATEN[GHOST_TYPE_TOTAL * 2] = {
        200, 400, 800, 1600, 3200, 6400, 12800, 25600
};
const Position SCORE_POSITION = {20 * 24, 5 * 24};
const Position HIGH_SCORE_POSITION = {10 * 24, 5 * 24};
const Position LIFE_POSITION = {0, 37 * 24};
const Position LEVEL_POSITION = {14 * 24, 37 * 24};
const Position POWER_POSITION = {0, 4 * 24};
const Position SCORE_LIST_POSITION[10] = {
        {3, 12},
        {3, 13},
        {3, 14},
        {3, 15},
        {3, 16},
        {3, 17},
        {3, 18},
        {3, 19},
        {3, 20},
        {3, 21},
};
const int SCORE_TIME = 500;
const std::string NUMBER_PATH = "../Assets/Number.png";
const std::string STATE_N_POWER_PATH = "../Assets/StateNPower.png";
class GameStatus {
public:
    GameStatus(Pacman *_pacman, Uint32 _highScore, SDL_Renderer *_renderer);
    ~GameStatus();

    void pushScore(UPDATED_SCORE type);
    void setGhostEaten(int ghostEaten = 0);

    void update();
    void render();

    void setFrameClip();

    Uint32 getScore() const;

    int getLevel();
    void updateLevel();
private:
    Log* gameStatusConsole;

    textureManager* gameStatusManager;

    SDL_Texture* numberTexture;
    SDL_Texture* stateNPowerTexture;

    Pacman* pacman;

    Timer* timer{};

    SDL_Renderer* renderer;

    Uint32 score, bonus, highScore;
    int scoreState{};
    int pacmanHealth;
    int level;

    SDL_Rect destRect{};

    bool power[PACMAN_POWER_TOTAL]{};
    int currentPower;

    SDL_Rect frameClip[10]{};
    SDL_Rect powerNStateClip[8]{};

    int currentGhostEaten;
};


#endif //BTL_GAMESTATUS_H
