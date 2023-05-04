//
// Created by MZuy on 4/14/2023.
//

#include "PlayState.h"

PlayState::PlayState(SDL_Renderer *renderer, Engine *_engine) {
    consolePlayState = new Log("PlayState");

    engine = _engine;

    pacman = new Pacman(renderer, static_cast<PACMAN_TYPE>(engine->getPacmanType()), engine->getTimer());

    blinky = new Ghost(BLINKY, renderer, engine->getTimer());
    inky = new Ghost(INKY, renderer, engine->getTimer());
    pinky = new Ghost(PINKY, renderer, engine->getTimer());
    clyde = new Ghost(CLYDE, renderer, engine->getTimer());
    mystery = new Ghost(MYSTERY, renderer, engine->getTimer());
    map = new Map(renderer);
}
void PlayState::init(Engine *_engine) {
    engine->loadHighScore();

    setState(NEW_GAME);

    setControl();
}
void PlayState::update() {
    handleState();

    blinky->handleState();
    inky->handleState();
    clyde->handleState();
    pinky->handleState();

    pacman->handleState();

    map->update(pacman, true);

    pacman->update();
    pacman->checkMove(!Map::isWallAt(pacman->getNextPosition()));
    if (!Map::isWallAt(pacman->getNextPosition(0))){
        pacman->stop();
    }

    //blinky->update();

    inky->update();

    pinky->update();

    clyde->update();
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
        if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(UP);
        else pacman->queueDirection(DOWN);
        consolePlayState->updateStatus("Got key up");
    }
    if (code == controlKey[CONTROL_DOWN]){
        if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(DOWN);
        else pacman->queueDirection(UP);
        consolePlayState->updateStatus("Got key down");
    }
    if (code == controlKey[CONTROL_LEFT]){
        if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(LEFT);
        else pacman->queueDirection(RIGHT);
        consolePlayState->updateStatus("Got key left");
    }
    if (code == controlKey[CONTROL_RIGHT]){
        if(!pacman->isPacmanPower(CONFUSED_PACMAN))
            pacman->queueDirection(RIGHT);
        else pacman->queueDirection(LEFT);
        consolePlayState->updateStatus("Got key right");
    }
}
void PlayState::keyUp(const int code) {
    if(code == SDLK_ESCAPE){
        engine->getAudio()->pause();
        engine->getTimer()->pause();
        pull(MENU_STATE);
    }
}
void PlayState::setState(PLAY_STATE_TYPE state) {
    currentState = state;
    switch(currentState){
        case NEW_GAME:
            engine->getAudio()->play(GAME_START);

            pacman->setState(PACMAN_START_STATE);

            //blinky->setState(GHOST_INIT);
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
            engine->getAudio()->stopChannel(0);

            mystery->setState(GHOST_INIT);

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
            mystery->setState(GHOST_INIT);

            pacman->setState(PACMAN_NEW_STATE);

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
        case NEW_GAME: case NEW_LIFE:
            if (!engine->getAudio()->isPlaying()){
                blinky->setState(GHOST_SCATTER);
                setState(PLAYING);
            }
            break;
        case UPGRADE_GHOST:
            break;
        case PLAYING:
            handleGhostTarget(blinky);
            handleGhostMove(blinky);
            blinky->handleMode();

            handleGhostTarget(inky);
            handleGhostMove(inky);
            inky->handleMode();

            handleGhostTarget(pinky);
            handleGhostMove(pinky);
            pinky->handleMode();

            handleGhostTarget(clyde);
            handleGhostMove(clyde);
            clyde->handleMode();

            pacman->handlePower();
            break;
    }
}
void PlayState::setControl() {
    controlKey[CONTROL_UP] = engine->getControlManager()->getControl(CONTROL_UP);
    controlKey[CONTROL_DOWN] = engine->getControlManager()->getControl(CONTROL_DOWN);
    controlKey[CONTROL_LEFT] = engine->getControlManager()->getControl(CONTROL_LEFT);
    controlKey[CONTROL_RIGHT] = engine->getControlManager()->getControl(CONTROL_RIGHT);
}
void PlayState::handleGhostTarget(Ghost *ghost) {
    if (ghost->getCurrentState() == GHOST_STAND)
        return;
    switch (ghost->getCurrentState()){
        case GHOST_CHASE: case GHOST_FREEZE:
            handleChaseTarget(ghost);
            break;
        case GHOST_SCATTER:
            ghost->setTarget(ghost->getScatterPosition());
            break;
        case GHOST_BLIND: case GHOST_FRIGHTEN: {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-2, 2);
            Direction random = static_cast<Direction>(dis(gen));
            while (random == 0) random = static_cast<Direction>(dis(gen));

            Position newTarget = ghost->getPosition();
            switch (random) {
                case UP:
                    newTarget.y -= 14;
                    break;
                case DOWN:
                    newTarget.y += 12;
                    break;
                case LEFT:
                    newTarget.x -= 14;
                    break;
                case RIGHT:
                    newTarget.x += 12;
                    break;
            }
            ghost->setTarget(newTarget);
            break;
        }
        case GHOST_EAT: case GHOST_EATEN: case GHOST_UNSET:
            ghost->setTarget(ghost->getStartPosition());
        default:
            break;
    }
}

void PlayState::handleChaseTarget(Ghost *ghost) {
    switch(ghost->getType()){
        case BLINKY:
            ghost->setTarget(pacman->getPosition());
            break;
        case PINKY: {
            Position newTarget = pacman->getPosition();
            switch (pacman->getDirection()) {
                case UP:
                    newTarget.y -= 4 * 24;
                    break;
                case DOWN:
                    newTarget.y += 4 * 24;
                    break;
                case LEFT:
                    newTarget.x -= 4 * 24;
                    break;
                case RIGHT:
                    newTarget.x += 4 * 24;
                    break;
            }
            ghost->setTarget(newTarget);
            break;
        }
        case INKY:{
            Position newTarget = pacman->getPosition();
            switch (pacman->getDirection()) {
                case UP:
                    newTarget.y -= 2 * 24;
                    break;
                case DOWN:
                    newTarget.y += 2 * 24;
                    break;
                case LEFT:
                    newTarget.x -= 2 * 24;
                    break;
                case RIGHT:
                    newTarget.x += 2 * 24;
                    break;
            }
            newTarget.x += (newTarget.x * 2 - blinky->getPosition().x);
            newTarget.y += (newTarget.y * 2 - blinky->getPosition().y);
            ghost->setTarget(newTarget);
            break;
        }
        case CLYDE:{
            Position newTarget = pacman->getPosition();
            if (Distance::Manhattan(ghost->getPosition(), newTarget) > 8 * 24)
                ghost->setTarget(newTarget);
            else
                ghost->setTarget(ghost->getScatterPosition());
            break;
        }
        default:
            break;
    }
}

Direction PlayState::calculateDirection(Ghost *ghost) {
    Position newPosition;
    int distance, minDistance = static_cast<int>(1e12);
    Direction newDirection = NONE;

    for (Direction direction = UP; direction <= RIGHT; direction = static_cast<Direction>(direction + 1)) {
        if (ghost->getDirection() == -direction)
            continue;
        distance = static_cast<int>(1e12);
        if (!Map::isWallAt(ghost->getNextPosition(direction))){
            newPosition = ghost->getPosition();
            switch (direction) {
                case UP:
                    newPosition.y -= 14;
                    break;
                case DOWN:
                    newPosition.y += 12;
                    break;
                case LEFT:
                    newPosition.x -= 14;
                    break;
                case RIGHT:
                    newPosition.x += 12;
                    break;
            }
            distance = Distance::Manhattan(newPosition, ghost->getTarget());
        }
        if (minDistance > distance){
            minDistance = distance;
            newDirection = direction;
        }
    }
    return newDirection;
}

void PlayState::handleGhostMove(Ghost *ghost) {
    if (ghost->getCurrentState() == GHOST_STAND)
        return;
    ghost->setDirection(calculateDirection(ghost));
}

HIT_TYPE PlayState::handleGhostHit(Ghost *ghost, Pacman *pacman) {
    if (ghost->getCurrentState() == GHOST_STAND)
        return NONE_HIT;
    if (ghost->checkCollision(pacman)) {
        if (ghost->isGhostMode(GHOST_FRIGHTEN_MODE)) {
            pacman->setState(PACMAN_EATING_STATE);
            ghost->setState(GHOST_EATEN);
            return GHOST_HIT;
        }
        if (!pacman->isPacmanPower(INVISIBLE_PACMAN) && ghost->getCurrentState() != GHOST_EAT
        && ghost->getCurrentState() != GHOST_EATEN && ghost->getCurrentState() != GHOST_UNSET) {
            if (ghost->getType() == DEADLY)
                pacman->loseHealth();
            return PACMAN_HIT;
        }
    }
    return NONE_HIT;
}

PlayState::~PlayState() noexcept {

}

