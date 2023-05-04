//
// Created by MZuy on 4/5/2023.
//

#ifndef BTL_ENGINE_H
#define BTL_ENGINE_H
#include <SDL.h>
#include "Audio.h"
#include "Timer.h"
#include "ControlManager.h"
#include <set>

const std::string GAME_CONFIG_PATH = "../config/game.config";
const std::string HIGH_SCORE_PATH = "../config/highscore.dat";

const int VOLUME_TOTAL = 11;
const int VOLUME_VALUE[VOLUME_TOTAL] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

class Engine {
public:
    Engine();
    ~Engine();

    void init();

    void load(const std::string& path = GAME_CONFIG_PATH);
    void loadHighScore(const std::string& path = HIGH_SCORE_PATH);

    void update() const;

    void save(const std::string& path = GAME_CONFIG_PATH) const;
    void saveHighScore(const std::string &path, Uint32 score, int level);

    Audio* getAudio(){
        return audio;
    }
    ControlManager* getControlManager(){
        return controlManager;
    }
    Timer* getTimer(){
        return timer;
    }
    void setMusicVolume(int volume){
        musicVolume = volume;
    }
    void setChannelVolume(int volume){
        channelVolume = volume;
    }
    void setPacmanType(int type){
        pacmanType = type;
    }
    void setControlType(int type){
        controlType = type;
    }
private:
    Log* consoleEngine;
    Timer* timer;
    Audio* audio;
    ControlManager* controlManager;
    std::multiset<Uint32, std::greater<>> highScoreSet;

    int lastLevel{};
    Uint32 lastScore{}, highScore{};
    int musicVolume{}, channelVolume{};
    int pacmanType{};
    int controlType{};
};


#endif //BTL_ENGINE_H
