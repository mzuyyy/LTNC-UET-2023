//
// Created by MZuy on 4/4/2023.
//

#include "Audio.h"

Audio::Audio() {
    consoleAudio = new Log("Audio");
    consoleAudio->updateStatus("Audio created!");
}

Audio::~Audio() {
    consoleAudio->updateStatus("Audio destroyed!");
    delete consoleAudio;
}
Mix_Music* Audio::loadMusic(const std::string &path) {
    consoleAudio->updateStatus("Loading file : " + path);
    Mix_Music* temp = Mix_LoadMUS(path.c_str());
    if (temp == nullptr) {
        consoleAudio->updateStatus("Mix_LoadMUS Error: " + std::string(Mix_GetError()));
    }
    return temp;
}
Mix_Chunk* Audio::loadEffect(const std::string &path) {
    consoleAudio->updateStatus("Loading file : " + path);
    Mix_Chunk* temp = Mix_LoadWAV(path.c_str());
    if (temp == nullptr) {
        consoleAudio->updateStatus("Mix_LoadWAV Error: " + std::string(Mix_GetError()));
    }
    return temp;
}
void Audio::init(const int frequency, const int channel, const int chunkSize) {
    consoleAudio->updateStatus("Initializing audio...");
    if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channel, chunkSize) < 0) {
        consoleAudio->updateStatus("SDL_mixer could not initialize!: " + std::string(Mix_GetError()));
    }
    consoleAudio->updateStatus("SDL_mixer initialized!");
    std::stringstream mixerInfor;
    mixerInfor << frequency << " Hz - " << channel << "channel - ChunkSize: " << chunkSize;
    consoleAudio->updateStatus(mixerInfor.str());

    musicVolume = MIX_MAX_VOLUME;
    channelVolume = MIX_MAX_VOLUME;

    consoleAudio->updateStatus("Loading audio...");
    gameStart = loadMusic(AUDIO_PATH[GAME_START]);
    gamePause = loadMusic(AUDIO_PATH[GAME_PAUSE]);
    credit = loadEffect(AUDIO_PATH[CREDIT]);
    munch = loadEffect(AUDIO_PATH[MUNCH]);
    extend = loadEffect(AUDIO_PATH[EXTEND]);
    upgrade = loadEffect(AUDIO_PATH[UPGRADE]);
    powerPellet = loadEffect(AUDIO_PATH[POWER_PELLET]);
    siren = loadEffect(AUDIO_PATH[SIREN]);
    death = loadEffect(AUDIO_PATH[DEATH]);
    eatGhost = loadEffect(AUDIO_PATH[EAT_GHOST]);
    eatFruit = loadEffect(AUDIO_PATH[EAT_FRUIT]);
    confused = loadEffect(AUDIO_PATH[CONFUSED]);
    freeze = loadEffect(AUDIO_PATH[FREEZE]);
    speed = loadEffect(AUDIO_PATH[SPEED]);
    bossShowUp = loadEffect(AUDIO_PATH[BOSS_SHOW_UP]);
    upgradeMystery = loadEffect(AUDIO_PATH[UPGRADE_MYSTERY]);
    bossDeath = loadEffect(AUDIO_PATH[BOSS_DEATH]);

    consoleAudio->updateStatus("Audio loaded!");
}

void Audio::free() {
    consoleAudio->updateStatus("Freeing audio...");
    Mix_FreeMusic(gameStart);
    gameStart = nullptr;
    Mix_FreeMusic(gamePause);
    gamePause = nullptr;
    Mix_FreeChunk(credit);
    credit = nullptr;
    Mix_FreeChunk(munch);
    munch = nullptr;
    Mix_FreeChunk(extend);
    extend = nullptr;
    Mix_FreeChunk(upgrade);
    upgrade = nullptr;
    Mix_FreeChunk(powerPellet);
    powerPellet = nullptr;
    Mix_FreeChunk(siren);
    siren = nullptr;
    Mix_FreeChunk(death);
    death = nullptr;
    Mix_FreeChunk(eatGhost);
    eatGhost = nullptr;
    Mix_FreeChunk(eatFruit);
    eatFruit = nullptr;
    Mix_FreeChunk(confused);
    confused = nullptr;
    Mix_FreeChunk(freeze);
    freeze = nullptr;
    Mix_FreeChunk(speed);
    speed = nullptr;
    Mix_FreeChunk(bossShowUp);
    bossShowUp = nullptr;
    Mix_FreeChunk(upgradeMystery);
    upgradeMystery = nullptr;
    Mix_FreeChunk(bossDeath);
    bossDeath = nullptr;

    consoleAudio->updateStatus("Audio freed!");
}

void Audio::close() {
    consoleAudio->updateStatus("Closing audio...");
    free();
    Mix_CloseAudio();
}

void Audio::play(MUSIC_TYPE musicType, bool loop) {
    switch (musicType){
        case GAME_START:
            Mix_PlayMusic(gameStart, loop);
            break;
        case GAME_PAUSE:
            Mix_PlayMusic(gamePause, loop);
            break;
    }
}

void Audio::play(EFFECT_TYPE effectType, bool loop, const int channel) {
    switch (effectType){
        case CREDIT:
            Mix_PlayChannel(channel, credit, loop);
            break;
        case MUNCH:
            Mix_PlayChannel(channel, munch, loop);
            break;
        case EXTEND:
            Mix_PlayChannel(channel, extend, loop);
            break;
        case UPGRADE:
            Mix_PlayChannel(channel, upgrade, loop);
            break;
        case POWER_PELLET:
            Mix_PlayChannel(channel, powerPellet, loop);
            break;
        case SIREN:
            Mix_PlayChannel(channel, siren, loop);
            break;
        case DEATH:
            Mix_PlayChannel(channel, death, loop);
            break;
        case EAT_GHOST:
            Mix_PlayChannel(channel, eatGhost, loop);
            break;
        case EAT_FRUIT:
            Mix_PlayChannel(channel, eatFruit, loop);
            break;
        case CONFUSED:
            Mix_PlayChannel(channel, confused, loop);
            break;
        case FREEZE:
            Mix_PlayChannel(channel, freeze, loop);
            break;
        case SPEED:
            Mix_PlayChannel(channel, speed, loop);
            break;
        case BOSS_SHOW_UP:
            Mix_PlayChannel(channel, bossShowUp, loop);
            break;
        case UPGRADE_MYSTERY:
            Mix_PlayChannel(channel, upgradeMystery, loop);
            break;
        case BOSS_DEATH:
            Mix_PlayChannel(channel, bossDeath, loop);
            break;
    }
}

void Audio::setMusicVolume(const int volume) {
    musicVolume = volume;
    Mix_VolumeMusic(musicVolume);
}

void Audio::setChannelVolume(const int volume) {
    channelVolume = volume;
    Mix_Volume(-1, channelVolume);
}



