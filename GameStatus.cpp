//
// Created by MZuy on 5/7/2023.
//

#include "GameStatus.h"
#include <fstream>

GameStatus::GameStatus(Pacman *_pacman, Uint32 _highScore, SDL_Renderer *_renderer) {
    gameStatusConsole = new Log("GameStatus");

    gameStatusManager = new textureManager();

    pacman = _pacman;
    highScore = _highScore;

    renderer = _renderer;

    numberTexture = gameStatusManager->loadTexture(NUMBER_PATH, renderer);
    stateNPowerTexture = gameStatusManager->loadTexture(STATE_N_POWER_PATH, renderer);

    score = bonus = pacmanHealth = 0;
    level = 0;

    currentGhostEaten = 0;

    setFrameClip();
}
void GameStatus::setFrameClip() {
    for (int i = 0; i < 10; i++) {
        frameClip[i] = {i * 24, 0, 24, 24};
    }
    for (int i = 0; i < 8; i++){
        powerNStateClip[i] = {i * 48, 0, 48, 48};
    }
}
void GameStatus::pushScore(UPDATED_SCORE type) {
    if (type == NORMAL_DOT)
        bonus += SCORE_NORMAL_DOT;
    if (type == BIG_DOT)
        bonus += SCORE_BIG_DOT;
    if (type == GHOST_EATEN) {
        bonus += SCORE_GHOST_EATEN[currentGhostEaten];
        currentGhostEaten++;
    }
}
void GameStatus::setGhostEaten(int ghostEaten) {
    currentGhostEaten = ghostEaten;
}

void GameStatus::update() {
    score = pacman->getEatenDot() + bonus;
    highScore = std::max(highScore,score);
    pacmanHealth = pacman->getHealth();
    for (int i = 0; i < PACMAN_POWER_TOTAL; i++)
        power[i] = pacman->isPacmanPower(static_cast<PACMAN_POWER>(i));
}

void GameStatus::render() {
    std::stringstream ss;
    std::string num;

    ss << highScore;
    ss >> num;
    while (num.size() < 8)
        num = "0" + num;
    if(highScore > score){
        for (int i = 0; i < 8; i++){
            destRect.x = HIGH_SCORE_POSITION.x + i * 24;
            destRect.y = HIGH_SCORE_POSITION.y;
            destRect.w = 24;
            destRect.h = 24;
            gameStatusManager->drawTexture(numberTexture, frameClip[num[i] - '0'], destRect, renderer);
        }
    }
    ss.clear();
    ss << score;
    ss >> num;
    while (num.size() < 8)
        num = "0" + num;
    if (score > highScore){
        for (int i = 0; i < 8; i++){
            destRect.x = HIGH_SCORE_POSITION.x + i * 24;
            destRect.y = HIGH_SCORE_POSITION.y;
            destRect.w = 24;
            destRect.h = 24;
            gameStatusManager->drawTexture(numberTexture, frameClip[num[i] - '0'], destRect, renderer);
        }
    }
    for (int i = 0; i < 8; i++){
        destRect.x = SCORE_POSITION.x + i * 24;
        destRect.y = SCORE_POSITION.y;
        destRect.w = 24;
        destRect.h = 24;
        gameStatusManager->drawTexture(numberTexture, frameClip[num[i] - '0'], destRect, renderer);
    }
    if (level < 5){
        destRect.x = LEVEL_POSITION.x;
        destRect.y = LEVEL_POSITION.y;
        destRect.w = 24;
        destRect.h = 24;
    }

    for (int i = 0; i < pacmanHealth; i++){
        destRect.x = LIFE_POSITION.x + i * OBJECT_PIXEL;
        destRect.y = LIFE_POSITION.y;
        destRect.w = OBJECT_PIXEL;
        destRect.h = OBJECT_PIXEL;
        if (pacman->getType() != MS)
            gameStatusManager->drawTexture(pacman->getTexture(), {(OBJECT_PIXEL + 6) * 3, 0, OBJECT_PIXEL, OBJECT_PIXEL}, destRect, renderer);
        else gameStatusManager->drawTexture(pacman->getTexture(), {(45 + 6) * 3, 0, 45, 45}, destRect, renderer);
    }
    destRect.x = POWER_POSITION.x;
    for (int i = 0; i < PACMAN_POWER_TOTAL; i++) {
        if (power[i]) {
            destRect.y = POWER_POSITION.y;
            destRect.w = 48;
            destRect.h = 48;
            gameStatusManager->drawTexture(stateNPowerTexture, powerNStateClip[i],
                                           destRect, renderer);
            destRect.x += 48;
        }
    }
}
Uint32 GameStatus::getScore() const {
    return score;
}

int GameStatus::getLevel() {
    return level;
}

void GameStatus::updateLevel() {
    level++;
}

GameStatus::~GameStatus() {
    delete gameStatusConsole;
    delete gameStatusManager;
    pacman = nullptr;
    timer = nullptr;
    renderer = nullptr;
}

