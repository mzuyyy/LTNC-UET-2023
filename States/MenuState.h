//
// Created by MZuy on 3/24/2023.
//

#ifndef BTL_MENUSTATE_H
#define BTL_MENUSTATE_H

#include "../textureManager.h"
#include "State.h"
#include "../Objects/Object.h"
#include "../Objects/pacman.h"

enum MENU_BUTTON {
    MENU_START = 0,
    MENU_HOW_TO_PLAY,
    MENU_SETTING,
    MENU_HIGHSCORE,
    MENU_ABOUT,
    MENU_RESUME,
    MENU_EXIT,
    MENU_YES,
    MENU_NO,
    MENU_BUTTON_TOTAL
};
enum STATE_OF_MENU {
    MAIN = 0,
    HOW_TO_PLAY,
    SETTING,
    HIGHSCORE,
    ABOUT,
    PAUSE,
    SAVE_SETTING,
    EXIT,
    GAME_OVER,
    YOU_WIN,
    STATE_OF_MENU_TOTAL
};
enum SETTING_BUTTON{
    CHANGE_MUSIC_VOLUME = 0,
    CHANGE_SOUND_VOLUME,
    CHOOSE_CONTROL,
    CHOOSE_PACMAN,
    SETTING_BUTTON_TOTAL
};
enum MINI_MENU_BUTTON{
    YES_RESUME,
    NO_EXIT,
    MINI_MENU_BUTTON_TOTAL,
};
const TileID MENU_BUTTON_TILE_ID[MENU_BUTTON_TOTAL] = {
        //{7,16},
        {7,19},
        {7,22},
        {7,25},
        {7,28},
        {7,31},
        {0,0},
        {7,34},
        {7,34},
};
const TileID SETTING_BUTTON_TILE_ID[SETTING_BUTTON_TOTAL] = {
        {3, 12},
        {3, 16},
        {3, 22},
        {3, 29},
};
const TileID MINI_MENU_BUTTON_TILE_ID[MINI_MENU_BUTTON_TOTAL]{
        {7, 20},
        {7, 23},
};
class MenuState : public State{
private:
    const int MENU_WIDTH = 672;
    const int MENU_HEIGHT = 888;

    const int BUTTON_WIDTH = 336;
    const int BUTTON_HEIGHT = 72;

    const int SETTING_BUTTON_WIDTH = 528;
    const int SETTING_BUTTON_HEIGHT[SETTING_BUTTON_TOTAL] = {
            72,
            72,
            96,
            144,
    };

    const TileID startButtonTileID = {0,3};

    Log* consoleMenu;

    Engine* engine{};

    SDL_Rect buttonDest[MENU_BUTTON_TOTAL];
    SDL_Rect settingDest[SETTING_BUTTON_TOTAL];
    SDL_Rect  miniDest[MINI_MENU_BUTTON_TOTAL];

    const std::string BUTTON_PATH = "../Assets/Pre-play/Button.png";
    const std::string MENU_PATH = "../Assets/Pre-play/Menu.png";
    const std::string SETTING_BUTTON_PATH[SETTING_BUTTON_TOTAL] = {
            "../Assets/Pre-play/ChangeMusicButton.png",
            "../Assets/Pre-play/ChangeSoundButton.png",
            "../Assets/Pre-play/ChooseControlButton.png",
            "../Assets/Pre-play/ChoosePacmanButton.png"
    };

    SDL_Texture* menuTexture;
    SDL_Texture* buttonTexture;
    SDL_Texture* changeMusicTexture;
    SDL_Texture* changeSoundTexture;
    SDL_Texture* chooseControlTexture;
    SDL_Texture* choosePacmanTexture;

    SDL_Renderer* menuStateRenderer;

    SDL_Rect destRect;
    textureManager* menuManager;

    int controlKey[CONTROL_DIRECTION_TOTAL];

    int currentMenuButton;
    int currentSettingButton;

    STATE_OF_MENU currentState;

    SDL_Rect menuFrameClip[STATE_OF_MENU_TOTAL];
    SDL_Rect changeMusicFrameClip[2][11];
    SDL_Rect changeSoundFrameClip[2][11];
    SDL_Rect chooseControlFrameClip[2][3];
    SDL_Rect choosePacmanFrameClip[2][3];
    SDL_Rect buttonFrameClip[2][MENU_BUTTON_TOTAL];

    int isChoosing[MENU_BUTTON_TOTAL];
    int isChoosingSetting[SETTING_BUTTON_TOTAL];

    int musicVolume, channelVolume;
    int pacmanType;
    int controlType;
public:
    explicit MenuState(SDL_Renderer* renderer);
    ~MenuState();

    void init(Engine* _engine) override;
    void update() override;
    void render() override;
    void close() override;
    void setMenuFrameClip();
    void setControl();
    void handleState();
    void handlePreState();
    void keyDown(int key) override;
    void keyUp(int key) override;
    void updateButton(int change);
    void updateSetting(int change);
    void setMenuState(STATE_OF_MENU newState);
};


#endif //BTL_MENUSTATE_H
