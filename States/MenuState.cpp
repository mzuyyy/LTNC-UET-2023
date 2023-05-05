//
// Created by MZuy on 3/24/2023.
//

#include "MenuState.h"

MenuState::MenuState(SDL_Renderer *renderer) {
    consoleMenu = new Log("MenuState");

    menuManager = new textureManager();

    menuStateRenderer = renderer;

    currentState = MAIN;

    destRect = {startButtonTileID.x * 24, startButtonTileID.y * 24, MENU_WIDTH, MENU_HEIGHT};

    for (int i = 0; i < MENU_BUTTON_TOTAL; i++)
        buttonDest[i] = {MENU_BUTTON_TILE_ID[i].x * 24, MENU_BUTTON_TILE_ID[i].y * 24, BUTTON_WIDTH, BUTTON_HEIGHT};
    for (int i = 0; i < SETTING_BUTTON_TOTAL; i++)
        settingDest[i] = {SETTING_BUTTON_TILE_ID[i].x * 24, SETTING_BUTTON_TILE_ID[i].y * 24, SETTING_BUTTON_WIDTH,
                          SETTING_BUTTON_HEIGHT[i]};
    for (int i = 0; i < MINI_MENU_BUTTON_TOTAL; i++)
        miniDest[i] = {MINI_MENU_BUTTON_TILE_ID[i].x * 24, MINI_MENU_BUTTON_TILE_ID[i].y * 24, BUTTON_WIDTH, BUTTON_HEIGHT};

    consoleMenu->updateStatus("Created menu state");
}

void MenuState::init(Engine *_engine) {
    engine = _engine;

    menuTexture = menuManager->loadTexture(MENU_PATH, menuStateRenderer);

    buttonTexture = menuManager->loadTexture(BUTTON_PATH, menuStateRenderer);

    changeMusicTexture = menuManager->loadTexture(SETTING_BUTTON_PATH[CHANGE_MUSIC_VOLUME], menuStateRenderer);
    changeSoundTexture = menuManager->loadTexture(SETTING_BUTTON_PATH[CHANGE_SOUND_VOLUME], menuStateRenderer);
    chooseControlTexture = menuManager->loadTexture(SETTING_BUTTON_PATH[CHOOSE_CONTROL], menuStateRenderer);
    choosePacmanTexture = menuManager->loadTexture(SETTING_BUTTON_PATH[CHOOSE_PACMAN], menuStateRenderer);


    currentMenuButton = MENU_START;

    isChoosing[MENU_START] = 1;
    for (int i = 1; i < MENU_BUTTON_TOTAL; i++)
        isChoosing[i] = 0;

    isChoosingSetting[CHANGE_MUSIC_VOLUME] = 1;
    for (int i = 1; i < SETTING_BUTTON_TOTAL; i++)
        isChoosingSetting[i] = 0;

    currentSettingButton = CHANGE_SOUND_VOLUME;

    setMenuFrameClip();
    setControl();

    consoleMenu->updateStatus("Initialized menu state");
}

void MenuState::render() {
    menuManager->drawTexture(menuTexture, menuFrameClip[currentState], destRect, menuStateRenderer);
    if (currentState == MAIN){
        menuManager->drawTexture(buttonTexture, buttonFrameClip[isChoosing[currentMenuButton]][currentMenuButton],
                                 buttonDest[currentMenuButton] , menuStateRenderer);
    }
    else if (currentState == SETTING){
        menuManager->drawTexture(changeMusicTexture, changeMusicFrameClip[isChoosingSetting[CHANGE_MUSIC_VOLUME]][musicVolume],
                                 settingDest[CHANGE_MUSIC_VOLUME] , menuStateRenderer);
        menuManager->drawTexture(changeSoundTexture, changeSoundFrameClip[isChoosingSetting[CHANGE_SOUND_VOLUME]][channelVolume],
                                 settingDest[CHANGE_SOUND_VOLUME] , menuStateRenderer);
        menuManager->drawTexture(chooseControlTexture, chooseControlFrameClip[isChoosingSetting[CHOOSE_CONTROL]][controlType],
                                 settingDest[CHOOSE_CONTROL] , menuStateRenderer);
        menuManager->drawTexture(choosePacmanTexture, choosePacmanFrameClip[isChoosingSetting[CHOOSE_PACMAN]][pacmanType],
                                 settingDest[CHOOSE_PACMAN] , menuStateRenderer);
    }
    else if (currentState == SAVE_SETTING){
        menuManager->drawTexture(buttonTexture, buttonFrameClip[isChoosing[MENU_YES]][MENU_YES],
                                 miniDest[YES_RESUME], menuStateRenderer);
        menuManager->drawTexture(buttonTexture, buttonFrameClip[isChoosing[MENU_NO]][MENU_NO],
                                 miniDest[NO_EXIT], menuStateRenderer);
    }
}

void MenuState::update() {

}

void MenuState::close() {

}
void MenuState::setMenuFrameClip() {
    for (int i = 0; i < STATE_OF_MENU_TOTAL; i++)
        menuFrameClip[i] = {i * MENU_WIDTH, 0, MENU_WIDTH, MENU_HEIGHT};

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 11; j++)
            changeMusicFrameClip[i][j] = {i * SETTING_BUTTON_WIDTH, j * SETTING_BUTTON_HEIGHT[CHANGE_MUSIC_VOLUME],
                                           SETTING_BUTTON_WIDTH, SETTING_BUTTON_HEIGHT[CHANGE_MUSIC_VOLUME]};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 11; j++)
            changeSoundFrameClip[i][j] = {i * SETTING_BUTTON_WIDTH, j * SETTING_BUTTON_HEIGHT[CHANGE_SOUND_VOLUME],
                                           SETTING_BUTTON_WIDTH, SETTING_BUTTON_HEIGHT[CHANGE_SOUND_VOLUME]};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < CONTROL_TYPE_TOTAL; j++)
            chooseControlFrameClip[i][j] = {i * SETTING_BUTTON_WIDTH, j * SETTING_BUTTON_HEIGHT[CHOOSE_CONTROL],
                                            SETTING_BUTTON_WIDTH, SETTING_BUTTON_HEIGHT[CHOOSE_CONTROL]};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < PACMAN_TYPE_TOTAL; j++)
            choosePacmanFrameClip[i][j] = {i * SETTING_BUTTON_WIDTH, j * SETTING_BUTTON_HEIGHT[CHOOSE_PACMAN],
                                           SETTING_BUTTON_WIDTH, SETTING_BUTTON_HEIGHT[CHOOSE_PACMAN]};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < MENU_BUTTON_TOTAL; j++)
            buttonFrameClip[i][j] = {j * BUTTON_WIDTH, i * BUTTON_HEIGHT,
                                     BUTTON_WIDTH, BUTTON_HEIGHT};
}
void MenuState::setControl() {
    controlKey[CONTROL_UP] = engine->getControlManager()->getControl(CONTROL_UP);
    controlKey[CONTROL_DOWN] = engine->getControlManager()->getControl(CONTROL_DOWN);
    controlKey[CONTROL_LEFT] = engine->getControlManager()->getControl(CONTROL_LEFT);
    controlKey[CONTROL_RIGHT] = engine->getControlManager()->getControl(CONTROL_RIGHT);
}
void MenuState::keyDown(int key) {
    engine->getAudio()->play(CREDIT);

    if (key == controlKey[CONTROL_UP])
        updateButton(-1);
    if (key == controlKey[CONTROL_DOWN])
        updateButton(1);
    if (key == controlKey[CONTROL_LEFT])
        updateSetting(-1);
    if (key == controlKey[CONTROL_RIGHT])
        updateSetting(1);
}
void MenuState::keyUp(const int key) {
    switch (key){
        case SDLK_RETURN: case SDLK_SPACE:
            handleState();
            break;
        case SDLK_ESCAPE:
            handlePreState();
            break;
        default:
            break;
    }
}
void MenuState::updateButton(const int change) {
    if (currentState == MAIN) {
        currentMenuButton += change;
        isChoosing[currentMenuButton - change] = 0;

        if (currentMenuButton < MENU_START || (currentMenuButton == MENU_RESUME && currentMenuButton - change == MENU_ABOUT))
            currentMenuButton = MENU_EXIT;
        else if (currentMenuButton == MENU_RESUME && currentMenuButton - change == MENU_EXIT)
            currentMenuButton = MENU_ABOUT;
        else if (currentMenuButton > MENU_EXIT) currentMenuButton = MENU_START;

        isChoosing[currentMenuButton] = 1;

        std::cerr << "Current menu button: " << currentMenuButton << std::endl;
    }
    else if (currentState == SETTING) {
        currentSettingButton += change;
        isChoosingSetting[currentSettingButton - change] = 0;

        if (currentSettingButton < CHANGE_MUSIC_VOLUME) currentSettingButton = CHOOSE_PACMAN;
        else if (currentSettingButton > CHOOSE_PACMAN) currentSettingButton = CHANGE_MUSIC_VOLUME;

        isChoosingSetting[currentSettingButton] = 1;
    }
    else if (currentState == SAVE_SETTING){
        currentMenuButton += change;
        isChoosing[currentMenuButton - change] = 0;

        if (currentMenuButton < MENU_YES) currentMenuButton = MENU_NO;
        else if (currentMenuButton > MENU_NO) currentMenuButton = MENU_YES;

        isChoosing[currentMenuButton] = 1;
    }
}
void MenuState::updateSetting(int change) {
    switch (currentSettingButton) {
        case CHANGE_MUSIC_VOLUME:
            musicVolume += change;
            if (musicVolume < 0 || musicVolume > 10)
                musicVolume -= change;
            engine->setMusicVolume(musicVolume);
            break;
        case CHANGE_SOUND_VOLUME:
            channelVolume += change;
            if (channelVolume < 0 || channelVolume > 10)
                channelVolume -= change;
            engine->setChannelVolume(channelVolume);
            break;
        case CHOOSE_CONTROL:
            controlType += change;
            if (controlType > 2) controlType = 0;
            else if (controlType < 0) controlType = 2;
            engine->setControlType(controlType);
            break;
        case CHOOSE_PACMAN:
            pacmanType += change;
            if (pacmanType > 2) pacmanType = 0;
            else if (pacmanType < 0) pacmanType = 2;
            engine->setPacmanType(pacmanType);
            break;
        default:
            break;
    }
}

void MenuState::handleState() {
    if (currentState == MAIN){
        switch (currentMenuButton){
            case MENU_START:
                pull(PLAY_STATE);
                break;
            case MENU_HOW_TO_PLAY:
                currentState = HOW_TO_PLAY;
                engine->loadHighScore();
                break;
            case MENU_SETTING:
                currentState = SETTING;
                currentSettingButton = CHANGE_MUSIC_VOLUME;
                isChoosingSetting[currentSettingButton] = 1;

                pacmanType = engine->getPacmanType();
                controlType = engine->getControlType();

                musicVolume = engine->getMusicVolume();
                channelVolume = engine->getChannelVolume();
                break;
            case MENU_HIGHSCORE:
                currentState = HIGHSCORE;
                break;
            case MENU_ABOUT:
                currentState = ABOUT;
                break;
            case MENU_EXIT:
                pull(EXIT_STATE);
                break;
        }
    }
    else if (currentState == SAVE_SETTING) {
        switch (currentMenuButton){
            case MENU_YES:
                engine->getAudio()->play(CREDIT);
                engine->save();
                break;
            case MENU_NO:
                engine->getAudio()->play(CREDIT);
                engine->load();
                break;
        }
        currentState = MAIN;
        currentMenuButton = MENU_START;
        isChoosing[MENU_START] = 1;
    }
}
void MenuState::handlePreState() {
    if (currentState == SETTING) {
        currentState = SAVE_SETTING;
        currentMenuButton = MENU_YES;
        isChoosing[MENU_YES] = 1;
    }
    else if (currentState == SAVE_SETTING)
        currentState = SETTING;
    else currentState = MAIN;
}
void MenuState::setMenuState(STATE_OF_MENU newState) {
    currentState = newState;
}
MenuState::~MenuState() {
    delete consoleMenu;
    consoleMenu = nullptr;
}

