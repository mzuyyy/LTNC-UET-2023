//
// Created by MZuy on 4/4/2023.
//

#ifndef BTL_AUDIO_H
#define BTL_AUDIO_H
#include <SDL_mixer.h>
#include "logStatus.h"
enum MUSIC_TYPE{
    GAME_START = 0,
    GAME_PAUSE,
    MUSIC_TYPE_TOTAL,
};
enum EFFECT_TYPE
{
    CREDIT = 2,
    MUNCH,
    EXTEND,
    UPGRADE,
    POWER_PELLET,
    SIREN,
    DEATH,
    EAT_GHOST,
    EAT_FRUIT,
    CONFUSED,
    FREEZE,
    SPEED,
    BOSS_SHOW_UP,
    UPGRADE_MYSTERY,
    BOSS_DEATH,
    EFFECT_TYPE_TOTAL
};
const int TOTAL_PATH_AUDIO = 17;
const std::string AUDIO_PATH[TOTAL_PATH_AUDIO] = {
        "../Assets/Sound/game_start.wav",
        "../Assets/Sound/game_pause.wav",

        "../Assets/Sound/credit.wav",
        "../Assets/Sound/munch_1.wav",
        "../Assets/Sound/extend.wav",
        "../Assets/Sound/ghost_upgrade.wav",
        "../Assets/Sound/power_pellet.wav",
        "../Assets/Sound/siren_1.wav",
        "../Assets/Sound/death_1.wav",
        "../Assets/Sound/eat_ghost.wav",
        "../Assets/Sound/eat_fruit.wav",
        "../Assets/Sound/confused.wav",
        "../Assets/Sound/freeze.wav",
        "../Assets/Sound/speed.wav",
        "../Assets/Sound/boss_show_up.wav",
        "../Assets/Sound/upgrade_mystery.wav",
        "../Assets/Sound/boss_death.wav",
};
class Audio{
public:
    Audio();
    ~Audio();

    Mix_Music* loadMusic(const std::string &path);
    Mix_Chunk* loadEffect(const std::string &path);

    void init(const int frequency = 44100, const int channel = 2, const int chunkSize = 2048);
    void free();
    void close();

    void play(MUSIC_TYPE musicType, bool loop = false);
    void play(EFFECT_TYPE effectType, bool loop = false, int channel = -1);

    void setMusicVolume(int volume);
    void setChannelVolume(int volume);


    static bool isPlaying(){
        return Mix_PlayingMusic();
    };
    static bool isPause(){
        return Mix_PausedMusic();
    };
    static void stop(){
        Mix_HaltMusic();
    };
    void pause(){
        if(!isPause())
            Mix_PauseMusic();
    };
    void resume(){
        if(isPause())
            Mix_ResumeMusic();
    };

    static bool isPlayingChannel(int channel = -1){
        return Mix_Playing(channel);
    };
    static bool isPauseChannel(int channel = -1){
        return Mix_Paused(channel);
    };
    void stopChannel(int channel = -1){
        Mix_HaltChannel(channel);
    };
    static void pauseChannel(int channel = - 1){
        if(!isPauseChannel(channel))
            Mix_Pause(channel);
    };
    static void resumeChannel(int channel = -1){
        if(isPauseChannel(channel))
            Mix_Resume(channel);
    };

    int getMusicVolume(){
        return musicVolume;
    }
    int getChannelVolume(){
        return channelVolume;
    }
private:
    int musicVolume{};
    int channelVolume{};

    Log* consoleAudio = nullptr;

    Mix_Music * gameStart{};
    Mix_Music * gamePause{};

    Mix_Chunk* credit{};
    Mix_Chunk* munch{};
    Mix_Chunk* extend{};
    Mix_Chunk* upgrade{};
    Mix_Chunk* powerPellet{};
    Mix_Chunk* siren{};
    Mix_Chunk* death{};
    Mix_Chunk* eatGhost{};
    Mix_Chunk* eatFruit{};
    Mix_Chunk* confused{};
    Mix_Chunk* freeze{};
    Mix_Chunk* speed{};
    Mix_Chunk* bossShowUp{};
    Mix_Chunk* upgradeMystery{};
    Mix_Chunk* bossDeath{};
};


#endif //BTL_AUDIO_H
