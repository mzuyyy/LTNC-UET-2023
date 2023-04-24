//
// Created by MZuy on 4/14/2023.
//

#include "PlayState.h"

PlayState::PlayState(SDL_Renderer* renderer) {
    consolePlayState = new Log("PlayState");

    pacman = new Pacman(renderer, CLASSIC);

    blinky = new Ghost(BLINKY, renderer);
    inky = new Ghost(INKY, renderer);
    pinky = new Ghost(PINKY, renderer);
    clyde = new Ghost(CLYDE, renderer);

    map = new Map(renderer);
}
void PlayState::init(Engine *_engine) {
    engine = _engine;
    engine->loadHighScore();
    setControl();
}
void PlayState::update() {
    handleState();

    map->update(pacman, true);

    pacman->update();

    pacman->checkMove(!Map::isWallAt(pacman->getNextPosition()));

    if (!Map::isWallAt(pacman->getNextPosition(0))){
        pacman->stop();
    }
}
void PlayState::render() {
    map->renderMap();
    pacman->render();
    blinky->render();
    clyde->render();
    inky->render();
    pinky->render();
}
void PlayState::close() {

}
void PlayState::keyDown(const int code) {
    if (currentState != PLAYING && currentState != END_GAME) {
        engine->getAudio()->play(CREDIT);
        return;
    }
    if (code == controlKey[CONTROL_UP]){
        //if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(UP);
        //else pacman->queueDirection(DOWN);
        consolePlayState->updateStatus("Got key up");
    }
    if (code == controlKey[CONTROL_DOWN]){
        //if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(DOWN);
        //else pacman->queueDirection(UP);
        consolePlayState->updateStatus("Got key down");
    }
    if (code == controlKey[CONTROL_LEFT]){
        //if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(LEFT);
        //else pacman->queueDirection(RIGHT);
        consolePlayState->updateStatus("Got key left");
    }
    if (code == controlKey[CONTROL_RIGHT]){
        //if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(RIGHT);
        //else pacman->queueDirection(LEFT);
        consolePlayState->updateStatus("Got key right");
    }
}
void PlayState::keyUp(const int code) {
    if(code == SDLK_ESCAPE){
        engine->getAudio()->pause();
        pull(RESUME_STATE);
    }
}
void PlayState::setState(PLAY_STATE_TYPE state) {
    currentState = state;
    switch(currentState){
        case NEW_GAME:
            engine->getAudio()->play(GAME_START);

            blinky->setState(GHOST_INIT);
            clyde->setState(GHOST_INIT);
            inky->setState(GHOST_INIT);
            pinky->setState(GHOST_INIT);

            dotCurrent = 150;

            break;
        case NEW_LIFE:
            engine->getAudio()->play(GAME_START);

            blinky->setState(GHOST_INIT);
            clyde->setState(GHOST_INIT);
            inky->setState(GHOST_INIT);
            pinky->setState(GHOST_INIT);

            pacman->setState(PACMAN_NEW_STATE);

            break;
        case UPGRADE_GHOST:
            break;
        case PLAYING:
            engine->getAudio()->play(SIREN, true, 0);
            break;
        case NEXT_LEVEL:
            engine->getAudio()->play(GAME_START);

            blinky->setState(GHOST_INIT);
            clyde->setState(GHOST_INIT);
            inky->setState(GHOST_INIT);
            pinky->setState(GHOST_INIT);

            dotCurrent = 150;

            break;
        case END_GAME:
            engine->getAudio()->stopChannel();
            engine->getAudio()->play(DEATH, false);
            pacman->setState(PACMAN_DEAD_STATE);
        case WIN:
            engine->getAudio()->play(GAME_PAUSE);
        default:
            break;
    }
}
void PlayState::handleState() {
    switch (currentState){
        case NEW_GAME:
            if (!engine->getAudio()->isPlaying()){
                blinky->setState(GHOST_SCATTER);
                setState(PLAYING);
            }
            break;
        case NEW_LIFE:
            if(!engine->getAudio()->isPlaying()){
                blinky->setState(GHOST_SCATTER);
                setState(PLAYING);
            }
            break;
        case UPGRADE_GHOST:
            break;
        case PLAYING:
            pacman->handlePower();

    }
}
void PlayState::setControl() {
    controlKey[CONTROL_UP] = engine->getControlManager()->getControl(CONTROL_UP);
    controlKey[CONTROL_DOWN] = engine->getControlManager()->getControl(CONTROL_DOWN);
    controlKey[CONTROL_LEFT] = engine->getControlManager()->getControl(CONTROL_LEFT);
    controlKey[CONTROL_RIGHT] = engine->getControlManager()->getControl(CONTROL_RIGHT);
}

PlayState::~PlayState() noexcept {

}

