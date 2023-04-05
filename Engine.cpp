//
// Created by MZuy on 4/5/2023.
//

#include "Engine.h"

Engine::Engine() {
    consoleEngine = new Log("Engine");
    timer = new Timer();
    audio = new Audio();
    controlManager = new ControlManager();
}

Engine::~Engine() {
    consoleEngine->updateStatus("Close engine...");
    delete consoleEngine;
    delete timer;
    delete audio;
}

void Engine::init() {
    timer->start();
    audio->init();
}

void Engine::load(const std::string &path) {
    std::ifstream config(path);
    if (config.is_open()) {
        config >> musicVolume >> channelVolume >> mainCharacter >> controlType;
        config.close();
        consoleEngine->updateStatus("Load game config successfully!");
    } else {
        consoleEngine->updateStatus("Load game config failed!");
    }
}

void Engine::loadHighScore(const std::string &path) {
    std::ifstream highScoreFile(path, std::ios::binary);
    highScoreSet.clear();
    if (highScoreFile.is_open()) {
        int res;
        highScoreFile >> res;
        while (res--){
            Uint32 temp;
            highScoreFile >> temp;
            highScoreSet.insert(temp);
        }
        if (!highScoreSet.empty())
            highScore = *highScoreSet.begin();
        consoleEngine->updateStatus("Load high score successfully!");
    } else {
        highScore = 0;
        consoleEngine->updateStatus("Load high score failed!");
    }
}

void Engine::update() const {
    audio->setMusicVolume(VOLUME_VALUE[musicVolume]);
    audio->setChannelVolume(VOLUME_VALUE[channelVolume]);

    controlManager->setControlType(controlType);
}

void Engine::save(const std::string &path) const {
    std::ofstream config(path);
    if (config.is_open()) {
        config << musicVolume << " " << channelVolume << std::endl << mainCharacter << std::endl << controlType;
        config.close();
        consoleEngine->updateStatus("Save game config successfully!");
    } else {
        consoleEngine->updateStatus("Save game config failed!");
    }
}

void Engine::saveHighScore(const std::string &path, Uint32 score, int level) {
    std::ofstream highScoreFile(path, std::ios::binary);

    lastScore = score;
    lastLevel = level;

    if (highScoreFile.is_open()){
        highScoreSet.insert(score);
        if (highScoreSet.size() >= 10)
            highScoreFile << 10 << std::endl;
        else {
            highScoreFile << highScoreSet.size() << std::endl;
        }
        int _count = 0;
        std::multiset<Uint32, std::greater<>>::iterator it;
        for (it = highScoreSet.begin(); it != highScoreSet.end(); it++){
            Uint32 temp = *it;
            _count++;
            if (_count <= 10)
                highScoreFile << temp << std::endl;
            else
                break;
        }
        consoleEngine->updateStatus("Save high score successfully!");
    } else {
        highScore = 0;
        consoleEngine->updateStatus("Save high score failed!");
    }
}
