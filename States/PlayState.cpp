//
// Created by MZuy on 4/14/2023.
//

#include "PlayState.h"

PlayState::PlayState(SDL_Renderer *renderer, Engine *_engine) {
    consolePlayState = new Log("PlayState");

    playStateManager = new textureManager();

    engine = _engine;

    playStateRenderer = renderer;

    buttonTexture = playStateManager->loadTexture("../Assets/Pre-play/Button.png", renderer);

    pauseTexture = playStateManager->loadTexture("../Assets/Pre-play/Menu.png", renderer);
    sourcePause = {672 * 5, 0, 672, 888};
    destPause = {0, 6 * 24, 672, 888};

    pacman = new Pacman(renderer, static_cast<PACMAN_TYPE>(engine->getPacmanType()), engine->getTimer());

    mystery = new Mystery(renderer, engine->getTimer());

    blinky = new Ghost(BLINKY, renderer, engine->getTimer());
    deadly = new UpgradedGhost(DEADLY, renderer, engine->getTimer());

    inky = new Ghost(INKY, renderer, engine->getTimer());
    freezy = new UpgradedGhost(FREEZY, renderer, engine->getTimer());

    pinky = new Ghost(PINKY, renderer, engine->getTimer());
    speedy = new UpgradedGhost(SPEEDY, renderer, engine->getTimer());

    clyde = new Ghost(CLYDE, renderer, engine->getTimer());
    invisy = new UpgradedGhost(INVISY, renderer, engine->getTimer());

    map = new Map(renderer);

    gameStatus = new GameStatus(pacman, engine->getHighScore(), renderer);
}
void PlayState::init(Engine *_engine) {
    engine->loadHighScore();

    setState(NEW_GAME);

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < BUTTON_TOTAL; j++){
            buttonPauseFrameClip[i][j] = {(j + 5) * 336, i * 72, 336, 72};
        }
    }
    buttonDest[PAUSED_RESUME] = {7 * 24, 23 * 24, 336, 72};
    buttonDest[PAUSED_EXIT] = {7 * 24, 26 * 24, 336, 72};
    setControl();
}
void PlayState::update() {
    handleState();
}
void PlayState::render() {
    if(currentState != PAUSED) {
        handleRenderMap();

        pacman->render();

        gameStatus->render();

        handleGhostRender(blinky, deadly);
        handleGhostRender(inky, freezy);
        handleGhostRender(pinky, speedy);
        handleGhostRender(clyde, invisy);
        return;
    }
    playStateManager->drawTexture(pauseTexture, sourcePause, destPause, playStateRenderer);
    playStateManager->drawTexture(buttonTexture, buttonPauseFrameClip[isChoosing[currentPauseButton]][currentPauseButton],
                                  buttonDest[currentPauseButton], playStateRenderer);
}
void PlayState::close() {
    consolePlayState->updateStatus("Closing PlayState");
}
void PlayState::keyDown(const int code) {
    if (currentState != PLAYING && currentState != END_GAME && currentState != PAUSED) {
        engine->getAudio()->play(CREDIT);
        return;
    }
    if (currentState == PAUSED){
        if (code == controlKey[CONTROL_UP])
            updateButton(-1);
        if (code == controlKey[CONTROL_DOWN])
            updateButton(1);
    }
    else {
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
}
void PlayState::keyUp(const int code) {
    if (currentState == PAUSED){
        switch (code){
            case SDLK_RETURN: case SDLK_SPACE:
                if (currentPauseButton == PAUSED_RESUME)
                    setState(PLAYING);
                if (currentPauseButton == PAUSED_EXIT)
                    pull(MENU_STATE);
            case SDLK_ESCAPE:
                setState(static_cast<PLAY_STATE_TYPE>(lastState));
            default:
                break;
        }
        return;
    }
    if(code == SDLK_ESCAPE){
        engine->getAudio()->pause();
        engine->getTimer()->pause();

        lastState = currentState;

        setState(PAUSED);
    }
}
void PlayState::setState(PLAY_STATE_TYPE state) {
    currentState = state;
    switch(currentState){
        case NEW_GAME:
            engine->getAudio()->play(GAME_START);

            if (engine->getTimer()->isPaused())
                engine->getTimer()->start();

            pacman->setState(PACMAN_INIT_STATE);

            blinky->setState(GHOST_INIT);
            clyde->setState(GHOST_INIT);
            inky->setState(GHOST_INIT);
            pinky->setState(GHOST_INIT);

            map->renderAnimationMap();

            currentDot = 150;

            break;
        case NEW_LIFE:
            //engine->getAudio()->play(GAME_START);

            if (engine->getTimer()->isPaused())
                engine->getTimer()->start();

            handleGhostInit(blinky, deadly);
            handleGhostInit(inky, freezy);
            handleGhostInit(pinky, speedy);
            handleGhostInit(clyde, invisy);

            pacman->setState(PACMAN_NEW_STATE);

            break;
        case PAUSED:
            engine->getAudio()->pause();
            engine->getTimer()->pause();
            isChoosing[PAUSED_RESUME] = true;
            currentPauseButton = PAUSED_RESUME;
            break;
        case UPGRADE_GHOST:
            if (engine->getTimer()->isPaused())
                engine->getTimer()->start();

            engine->getAudio()->stopChannel(0);

            mystery->setState(MYSTERY_INIT);

            if (gameStatus->getLevel() == 1)
                initUpgraded(blinky, deadly);
            else if (gameStatus->getLevel() == 2)
                initUpgraded(pinky, speedy);
            else if (gameStatus->getLevel() == 3)
                initUpgraded(clyde, invisy);
            else if (gameStatus->getLevel() == 4)
                initUpgraded(inky, freezy);
            break;
        case PLAYING:
            engine->getAudio()->play(SIREN, true, 0);
            if (engine->getTimer()->isPaused())
                engine->getTimer()->start();
            break;
        case NEXT_LEVEL:
            engine->getAudio()->play(GAME_START);

            if (engine->getTimer()->isPaused())
                engine->getTimer()->start();

            mystery->setState(MYSTERY_INIT);

            blinky->setState(GHOST_INIT);
            clyde->setState(GHOST_INIT);
            inky->setState(GHOST_INIT);
            pinky->setState(GHOST_INIT);


            pacman->setState(PACMAN_NEW_STATE);

            currentDot = 150;

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
    switch (currentState) {
        case NEW_GAME:
            if (!engine->getAudio()->isPlaying()) {
                blinky->setState(GHOST_SCATTER);
                setState(PLAYING);
            }
            break;
        case NEW_LIFE:
            //if (!engine->getAudio()->isPlaying()) {
            if (!blinky->isGhostUpgrade()) {
                blinky->setState(GHOST_SCATTER);
                setState(PLAYING);
            } else deadly->setState(UPGRADED_GHOST_SCATTER);
            //}
            setState(PLAYING);
            break;
        case UPGRADE_GHOST:
            if (gameStatus->getLevel() == 1)
                handleGhostUpgrade(blinky, deadly);
            else if (gameStatus->getLevel() == 2)
                handleGhostUpgrade(pinky, speedy);
            else if (gameStatus->getLevel() == 3)
                handleGhostUpgrade(clyde, invisy);
            else if (gameStatus->getLevel() == 4)
                handleGhostUpgrade(inky, freezy);
            break;
        case PAUSED:
            if (engine->getAudio()->isPlaying())
                engine->getAudio()->pause();
            if (engine->getTimer()->isStarted())
                engine->getTimer()->pause();
            std::cerr << "Current pause button : " << currentPauseButton << std::endl;
            break;
        case PLAYING: {
            pacman->handlePower();

            handleGhostMode(blinky, deadly);
            handleGhostMode(pinky, speedy);
            handleGhostMode(clyde, invisy);
            handleGhostMode(inky, freezy);

            handleGhostState(blinky, deadly);
            handleGhostState(inky, freezy);
            handleGhostState(pinky, speedy);
            handleGhostState(clyde, invisy);

            pacman->handleState();

            //std::cerr << "Pacman's state : " << pacman->getState() << std::endl;
            //std::cerr << "Pacman x = " << pacman->getPosition().x << "; y = " << pacman->getPosition().y << std::endl;
            //std::cerr << "Is pacman invincible : " << pacman->isPacmanPower(INVISIBLE_PACMAN) << std::endl;
            //map->update(pacman, false);

            gameStatus->update();

            pacman->update();
            pacman->checkMove(!Map::isWallAt(pacman->getNextPosition()));
            if (!Map::isWallAt(pacman->getNextPosition(0))){
                pacman->stop();
            }
            //std::cerr << "Pacman x = " << pacman->getPosition().x << "; y = " << pacman->getPosition().y << std::endl;

            handleGhostUpdate(blinky, deadly);
            //std::cerr << "Blinky's check position " << std::boolalpha << blinky->checkPosition() << std::endl;
            //std::cerr << "Blinky's start x = " << blinky->getStartPosition().x << "; y = " << blinky->getStartPosition().y << std::endl;
            //std::cerr << "Blinky's state : " << blinky->getCurrentState() << std::endl;
            //std::cerr << "Blinky's target: " << blinky->getTarget().x << " " << blinky->getTarget().y << std::endl;
            //std::cerr << "Blinky's velocity " << blinky->getVelocity() << std::endl;
            //std::cerr << "Blinky x = " << blinky->getPosition().x << "; y = " << blinky->getPosition().y << std::endl;

            handleGhostUpdate(inky, freezy);
            //std::cerr << "Inky's check position " << std::boolalpha << inky->checkPosition() << std::endl;
            //std::cerr << "Inky x = " << inky->getPosition().x << "; y = " << inky->getPosition().y << std::endl;
            //std::cerr << "Inky's state : " << inky->getCurrentState() << std::endl;
            //std::cerr << "Inky's target: " << inky->getTarget().x << " " << inky->getTarget().y << std::endl;

            handleGhostUpdate(pinky, speedy);
            //std::cerr << "Pinky's check position " << std::boolalpha << pinky->checkPosition() << std::endl;
            //std::cerr << "Pinky x = " << pinky->getPosition().x << "; y = " << pinky->getPosition().y << std::endl;
            //std::cerr << "Pinky's state : " << pinky->getCurrentState() << std::endl;
            //std::cerr << "Pinky's target: " << pinky->getTarget().x << " " << pinky->getTarget().y << std::endl;

            handleGhostUpdate(clyde, invisy);
            //std::cerr << "Clyde's check position " << std::boolalpha << clyde->checkPosition() << std::endl;
            //std::cerr << "Clyde x = " << clyde->getPosition().x << "; y = " << clyde->getPosition().y << std::endl;
            //std::cerr << "Clyde's state : " << clyde->getCurrentState() << std::endl;
            //std::cerr << "Clyde's target: " << clyde->getTarget().x << " " << clyde->getTarget().y << std::endl;

            mystery->update();

            if (pacman->getState() == PACMAN_EATING_STATE) {
                pacman->update();

                handleGhostDead(blinky, deadly);
                handleGhostDead(inky, freezy);
                handleGhostDead(pinky, speedy);
                handleGhostDead(clyde, invisy);

                break;
            }
            if (Map::isDotAt(pacman->getPosition())) {
                currentDot++;
                int power = Map::isPowerDotAt(pacman->getPosition());
                if (power == -1) {
                    engine->getAudio()->play(MUNCH);
                    engine->getAudio()->stopChannel(0);
                } else {
                    switch (power) {
                        case SUPER_POWER:
                            std::cerr << "SUPER POWER" << std::endl;
                            pacman->setPower(POWER_PACMAN);

                            setGhostState(blinky, deadly, FRIGHTEN_STATE);
                            setGhostState(inky, freezy, FRIGHTEN_STATE);
                            setGhostState(pinky, speedy, FRIGHTEN_STATE);
                            setGhostState(clyde, invisy, FRIGHTEN_STATE);

                            gameStatus->setGhostEaten();

                            engine->getAudio()->play(POWER_PELLET);

                            break;
                        case SPEED_UP:
                            std::cerr << "SPEED UP" << std::endl;
                            pacman->setPower(SPEED_PACMAN);

                            engine->getAudio()->play(SPEED);

                            break;
                        case INVINCIBLE:
                            std::cerr << "INVINCIBLE" << std::endl;

                            pacman->setPower(INVISIBLE_PACMAN);

                            setGhostState(blinky, deadly, BLIND_STATE);
                            setGhostState(inky, freezy, BLIND_STATE);
                            setGhostState(pinky, speedy, BLIND_STATE);
                            setGhostState(clyde, invisy, BLIND_STATE);

                            engine->getAudio()->play(POWER_PELLET);

                            break;
                        case FROZEN:
                            std::cerr << "FROZEN" << std::endl;

                            pacman->setPower(FROZEN_PACMAN);

                            setGhostState(blinky, deadly, FREEZE_STATE);
                            setGhostState(inky, freezy, FREEZE_STATE);
                            setGhostState(pinky, speedy, FREEZE_STATE);
                            setGhostState(clyde, invisy, FREEZE_STATE);

                            engine->getAudio()->play(FREEZE);

                            break;
                        default:
                            break;
                    }
                }
                map->update(pacman, false);
            }
            handleGhostMovement(blinky, deadly);
            handleGhostMovement(inky, freezy);
            handleGhostMovement(pinky, speedy);
            handleGhostMovement(clyde, invisy);

            if (map->isDotRunOut())
                setState(WIN);

            bool isEndGame = false;

            if (!blinky->isGhostUpgrade()) {
                switch (handleGhostHit(blinky, pacman)) {
                    case GHOST_HIT:
                        setState(END_GAME);
                        isEndGame = true;
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            }
            else {
                switch (handleUpgradedGhostHit(deadly)) {
                    case GHOST_HIT:
                        setState(END_GAME);
                        isEndGame = true;
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            }
            if (!pinky->isGhostUpgrade())
                switch (handleGhostHit(pinky, pacman)) {
                    case GHOST_HIT:
                        setState(END_GAME);
                        isEndGame = true;
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            else {
                switch (handleUpgradedGhostHit(speedy)) {
                    case GHOST_HIT:
                        if (!pacman->isPacmanPower(SLOW_DOWN_PACMAN)) {
                            pacman->setPower(SLOW_DOWN_PACMAN);
                            speedy->setState(UPGRADED_GHOST_SCATTER);
                            speedy->setMode(UPGRADED_GHOST_SPEED_UP_MODE);
                            engine->getAudio()->play(CONFUSED);
                        }
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            }
            if (!inky->isGhostUpgrade())
                switch (handleGhostHit(inky, pacman)) {
                    case GHOST_HIT:
                        setState(END_GAME);
                        isEndGame = true;
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            else
                switch (handleUpgradedGhostHit(freezy)) {
                    case GHOST_HIT:
                        if (!pacman->isPacmanPower(FREEZE_PACMAN)) {
                            pacman->setPower(FREEZE_PACMAN);
                            freezy->setMode(UPGRADED_GHOST_SPEED_UP_MODE);
                            engine->getAudio()->play(FREEZE);
                        }
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            if (!clyde->isGhostUpgrade())
                switch (handleGhostHit(clyde, pacman)) {
                    case GHOST_HIT:
                        setState(END_GAME);
                        isEndGame = true;
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            else
                switch (handleUpgradedGhostHit(invisy)) {
                    case GHOST_HIT:
                        if (!pacman->isPacmanPower(BLIND_PACMAN)) {
                            pacman->setPower(BLIND_PACMAN);
                            invisy->setMode(UPGRADED_GHOST_INVISIBLE_MODE);
                            engine->getAudio()->play(CONFUSED);
                        }
                        break;
                    case PACMAN_HIT:
                        engine->getAudio()->play(EAT_GHOST);
                        gameStatus->pushScore(GHOST_EATEN);
                        break;
                    default:
                        break;
                }
            if (isEndGame)
                break;
            handleGhostOutCage(50, pinky, speedy);
            handleGhostOutCage(100, inky, freezy);
            handleGhostOutCage(150, clyde, invisy);

            break;
        }
        case NEXT_LEVEL:
            if (engine->getAudio()->isPlaying()){
                if (!blinky->isGhostUpgrade())
                    blinky->setState(GHOST_SCATTER);
                else deadly->setState(UPGRADED_GHOST_SCATTER);
                setState(PLAYING);
            }
            break;
        case END_GAME:
            if (engine->getAudio()->isPlayingChannel()){
                if (!pacman->isPacmanDead())
                    setState(NEW_LIFE);
                else {
                    engine->saveHighScore(gameStatus->getScore(), gameStatus->getLevel());
                    pull(GAME_OVER_STATE);
                }
            }
            break;
        case WIN:
            if (!engine->getAudio()->isPlaying()){
                gameStatus->updateLevel();
                if (gameStatus->getLevel() == 0) {
                    engine->saveHighScore(gameStatus->getScore(), gameStatus->getLevel());
                    pull(WIN_GAME_STATE);
                }
                else if (gameStatus->getLevel() < 5)
                    setState(UPGRADE_GHOST);
            }
        default:
            break;
    }
}
void PlayState::setControl() {
    controlKey[CONTROL_UP] = engine->getControlManager()->getControl(CONTROL_UP);
    controlKey[CONTROL_DOWN] = engine->getControlManager()->getControl(CONTROL_DOWN);
    controlKey[CONTROL_LEFT] = engine->getControlManager()->getControl(CONTROL_LEFT);
    controlKey[CONTROL_RIGHT] = engine->getControlManager()->getControl(CONTROL_RIGHT);
}
void PlayState::handleRenderMap() {
    if (currentState == NEW_GAME){
        map->renderAnimationMap();
    }
    else map->renderMap();
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
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<> dis(-2, 2);
            auto random = static_cast<Direction>(dis(gen));
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
                default:
                    break;
            }
            ghost->setTarget(newTarget);
            break;
        }
        case GHOST_WAS_EATEN: case GHOST_IS_EATEN: case GHOST_UNSET: {
            Position temp = START_GHOST_POSITION[BLINKY];
            temp.y += 24 * 5;
            ghost->setTarget(temp);
        }
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
                    newTarget.x +=  4 * 24;
                    break;
                default:
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
                default:
                    break;
            }
            newTarget.x += (newTarget.x * 2 - blinky->getPosition().x);
            newTarget.y += (newTarget.y * 2 - blinky->getPosition().y);
            ghost->setTarget(newTarget);
            break;
        }
        case CLYDE:{
            Position newTarget = pacman->getPosition();
            if (Distance::Euclidean(ghost->getPosition(), newTarget) > 8 * 24)
                ghost->setTarget(newTarget);
            else
                ghost->setTarget(ghost->getScatterPosition());
            break;
        }
        default:
            break;
    }
}
void PlayState::handleGhostUpgrade(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    handleMysteryMove();
    mystery->handleState();

    handleGhostMove(ghost);
    upgradedGhost->handleState();

    if (ghost->getCurrentState() != GHOST_UPGRADE && ghost->getPosition() != ghost->getTarget()){
        mystery->setState(MYSTERY_SPEECH);
        engine->getAudio()->pauseChannel(0);
        engine->getAudio()->play(UPGRADE);
        ghost->setState(GHOST_UPGRADE);
    }
    if (ghost->isGhostUpgrade() && upgradedGhost->getCurrentState() != UPGRADED_GHOST_APPEAR)
        upgradedGhost->setState(UPGRADED_GHOST_APPEAR);
    if (upgradedGhost->isUpgradedGhostAppear()){
        engine->getAudio()->pauseChannel();
        engine->getAudio()->resumeChannel(0);
        setState(NEXT_LEVEL);
    }
}

void PlayState::handleGhostRender(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade())
        ghost->render();
    else
    if (upgradedGhost->isUpgradedGhostAppear() || upgradedGhost->getCurrentState() == UPGRADED_GHOST_APPEAR)
        upgradedGhost->render();
}
void PlayState::handleGhostUpdate(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade())
        ghost->update();
    else
        upgradedGhost->update();
}

Direction PlayState::calculateDirection(Ghost *ghost) {
    Position newPosition;
    double distance, minDistance = static_cast<double>(1e12);
    Direction newDirection = NONE;

    for (int direction = UP; direction <= DOWN; direction++) {
        if (ghost->getDirection() == -direction || direction == NONE)
            continue;
        distance = static_cast<double>(1e12);
        if (!Map::isWallAt(ghost->getNextPosition(static_cast<Direction>(direction)))){
            newPosition = ghost->getPosition();
            switch (direction) {
                case UP:
                    newPosition.y -= 42;
                    break;
                case DOWN:
                    newPosition.y += 42;
                    break;
                case LEFT:
                    newPosition.x -= 42;
                    break;
                case RIGHT:
                    newPosition.x += 42;
                    break;
                default:
                    break;
            }
            distance = Distance::Euclidean(newPosition, ghost->getTarget());
        }
        if (minDistance > distance){
            minDistance = distance;
            newDirection = static_cast<Direction>(direction);
        }
    }
    return newDirection;
}

void PlayState::handleGhostMove(Ghost *ghost) {
    if (ghost->getCurrentState() == GHOST_STAND || ghost->getCurrentState() == GHOST_FREEZE)
        return;
    if(Map::isWallAt(ghost->getNextPosition(ghost->getDirection()))){
        ghost->setDirection(calculateDirection(ghost));
    }
    else {
        if (ghost->checkPosition() || ghost->getPosition() == ghost->getBlinkyStartPosition())
            ghost->setDirection(calculateDirection(ghost));
    }
}
void PlayState::handleUpgradedGhostTarget(UpgradedGhost* upgradedGhost) {
    if (upgradedGhost->getCurrentState() == UPGRADED_GHOST_STAND)
        return;
    switch (upgradedGhost->getCurrentState()){
        case UPGRADED_GHOST_CHASE: case UPGRADED_GHOST_FREEZE:
            handleUpgradedChaseTarget(upgradedGhost);
            break;
        case UPGRADED_GHOST_SCATTER:
            upgradedGhost->setTarget(upgradedGhost->getScatterPosition());
            break;
        case UPGRADED_GHOST_BLIND: case UPGRADED_GHOST_FRIGHTEN:{
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-2, 2);
            auto random = static_cast<Direction>(dis(gen));
            while (random == 0) random = static_cast<Direction>(dis(gen));

            Position newTarget = upgradedGhost->getPosition();
            switch (random) {
                case UP:
                    newTarget.y -= 42;
                    break;
                case DOWN:
                    newTarget.y += 42;
                    break;
                case LEFT:
                    newTarget.x -= 42;
                    break;
                case RIGHT:
                    newTarget.x += 42;
                    break;
                default:
                    break;
            }
            upgradedGhost->setTarget(newTarget);
            break;
        }
        case UPGRADED_GHOST_IS_EATING: case UPGRADED_GHOST_WAS_EATEN: case UPGRADED_GHOST_UNSET:
            upgradedGhost->setTarget(upgradedGhost->getStartPosition());
            break;
        default:
            break;
    }
}

void PlayState::handleUpgradedChaseTarget(UpgradedGhost *upgradedGhost) {
    switch(upgradedGhost->getType()){
        case DEADLY:
            upgradedGhost->setTarget(pacman->getPosition());
            break;
        case SPEEDY: {
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
                default:
                    break;
            }
            upgradedGhost->setTarget(newTarget);
            break;
        }
        case FREEZY:{
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
                default:
                    break;
            }
            newTarget.x += (newTarget.x * 2 - deadly->getPosition().x);
            newTarget.y += (newTarget.y * 2 - deadly->getPosition().y);
            upgradedGhost->setTarget(newTarget);
            break;
        }
        case INVISY:{
            Position newTarget = pacman->getPosition();
            if (Distance::Euclidean(upgradedGhost->getPosition(), newTarget) > 8 * 24)
                upgradedGhost->setTarget(newTarget);
            else
                upgradedGhost->setTarget(upgradedGhost->getScatterPosition());
            break;
        }
        default:
            break;
    }
}
Direction PlayState::calculateDirection(UpgradedGhost *upgradedGhost) {
    Position newPosition;
    double distance, minDistance = static_cast<double>(1e12);
    Direction newDirection = NONE;

    for (int direction = UP; direction <= DOWN; direction++) {
        if (upgradedGhost->getDirection() == -direction || direction == NONE)
            continue;
        distance = static_cast<double>(1e12);
        if (!Map::isWallAt(upgradedGhost->getNextPosition(static_cast<Direction>(direction)))){
            newPosition = upgradedGhost->getPosition();
            switch (direction) {
                case UP:
                    newPosition.y -= 42;
                    break;
                case DOWN:
                    newPosition.y += 42;
                    break;
                case LEFT:
                    newPosition.x -= 42;
                    break;
                case RIGHT:
                    newPosition.x += 42;
                    break;
                default:
                    break;
            }
            distance = Distance::Euclidean(newPosition, upgradedGhost->getTarget());
        }
        if (minDistance > distance){
            minDistance = distance;
            newDirection = static_cast<Direction>(direction);
        }
    }
    return newDirection;
}
void PlayState::handleUpgradedGhostMove(UpgradedGhost *upgradedGhost) {
    if (upgradedGhost->getCurrentState() == UPGRADED_GHOST_STAND || upgradedGhost->getCurrentState() == UPGRADED_GHOST_FREEZE)
        return;
    if (upgradedGhost->checkPosition() || upgradedGhost->getPosition() == upgradedGhost->getStartPosition())
        upgradedGhost->setDirection(calculateDirection(upgradedGhost));

}

HIT_TYPE PlayState::handleGhostHit(Ghost *ghost, Pacman* _pacman) {
    if (ghost->getCurrentState() == GHOST_STAND)
        return NONE_HIT;
    if (ghost->checkCollision(_pacman)) {
        if (ghost->isGhostMode(GHOST_FRIGHTEN_MODE)) {
            _pacman->setState(PACMAN_EATING_STATE);
            ghost->setState(GHOST_IS_EATEN);
            return PACMAN_HIT;
        }
        if (!_pacman->isPacmanPower(INVISIBLE_PACMAN) && ghost->getCurrentState() != GHOST_IS_EATEN
            && ghost->getCurrentState() != GHOST_WAS_EATEN && ghost->getCurrentState() != GHOST_UNSET)
            return GHOST_HIT;
    }
    return NONE_HIT;
}
HIT_TYPE PlayState::handleUpgradedGhostHit(UpgradedGhost *upgradedGhost) {
    if (upgradedGhost->getCurrentState() == UPGRADED_GHOST_STAND)
        return NONE_HIT;
    if (upgradedGhost->checkCollision(pacman)) {
        if (upgradedGhost->isGhostMode(UPGRADED_GHOST_FRIGHTEN_MODE)) {
            pacman->setState(PACMAN_EATING_STATE);
            upgradedGhost->setState(UPGRADED_GHOST_WAS_EATEN);
            return NONE_HIT;
        }
    }
    if (!pacman->isPacmanPower(INVISIBLE_PACMAN) && upgradedGhost->getCurrentState() != UPGRADED_GHOST_WAS_EATEN
        && upgradedGhost->getCurrentState() != UPGRADED_GHOST_IS_EATING && upgradedGhost->getCurrentState() != UPGRADED_GHOST_UNSET) {
        if (upgradedGhost->getType() == DEADLY)
            pacman->loseHealth();
        return GHOST_HIT;
    }
    return NONE_HIT;
}
void PlayState::handleGhostState(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade())
        ghost->handleState();
    else upgradedGhost->handleState();
}
void PlayState::handleGhostMovement(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade()){
        handleGhostTarget(ghost);
        handleGhostMove(ghost);
    }
    else {
        handleUpgradedGhostTarget(upgradedGhost);
        handleUpgradedGhostMove(upgradedGhost);
    }
}
void PlayState::handleGhostDead(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade()){
        if (ghost->getCurrentState() == GHOST_WAS_EATEN || ghost->getCurrentState() == GHOST_REBORN){
            handleGhostTarget(ghost);
            handleGhostMove(ghost);
        }
    }
    else {
        if (upgradedGhost->getCurrentState() == UPGRADED_GHOST_WAS_EATEN || upgradedGhost->getCurrentState() == UPGRADED_GHOST_REBORN){
            handleUpgradedGhostTarget(upgradedGhost);
            handleUpgradedGhostMove(upgradedGhost);
        }
    }
}
void PlayState::handleGhostInit(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade())
        ghost->setState(GHOST_INIT);
    else upgradedGhost->setState(UPGRADED_GHOST_INIT);
}
void PlayState::handleGhostMode(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade())
        ghost->handleMode();
    else {
        upgradedGhost->handleMode();
        if (upgradedGhost->getType() == SPEEDY) {
            int distance = Distance::Euclidean(upgradedGhost->getPosition(), pacman->getPosition());
            if (distance <= 2 * OBJECT_SIZE) {
                if (!pacman->isPacmanPower(SLOW_DOWN_PACMAN))
                    if (!upgradedGhost->isGhostMode(UPGRADED_GHOST_SLOW_DOWN_MODE))
                        upgradedGhost->setMode(UPGRADED_GHOST_SLOW_DOWN_MODE);
            } else if (distance <= 12 * OBJECT_SIZE)
                if (!upgradedGhost->isGhostMode(UPGRADED_GHOST_SPEED_UP_MODE))
                    upgradedGhost->setMode(UPGRADED_GHOST_SPEED_UP_MODE);
        }
        if (upgradedGhost->getType() == INVISY)
            if (!upgradedGhost->isGhostMode(UPGRADED_GHOST_INVISIBLE_MODE))
                upgradedGhost->setMode(UPGRADED_GHOST_INVISIBLE_MODE);
    }
}
void PlayState::handleGhostOutCage(int dotRequire, Ghost *ghost, UpgradedGhost *upgradedGhost) {
    if (!ghost->isGhostUpgrade()) {
        if (currentDot >= dotRequire && !ghost->isGhostOutCage())
            ghost->setState(GHOST_SCATTER);
    }
    else if (currentDot >= dotRequire && !upgradedGhost->isUpgradedGhostOutCage())
        upgradedGhost->setState(UPGRADED_GHOST_SCATTER);
}
void PlayState::setGhostState(Ghost *ghost, UpgradedGhost *upgradedGhost, GHOST_STATE_TYPE state) {
    switch (state) {
        case FRIGHTEN_STATE:
            if (!ghost->isGhostUpgrade())
                ghost->setState(GHOST_FRIGHTEN);
            else upgradedGhost->setState(UPGRADED_GHOST_FRIGHTEN);
            break;
        case FREEZE_STATE:
            if (!ghost->isGhostUpgrade())
                ghost->setState(GHOST_FREEZE);
            else upgradedGhost->setState(UPGRADED_GHOST_FREEZE);
            break;
        case BLIND_STATE:
            if (!ghost->isGhostUpgrade())
                ghost->setState(GHOST_BLIND);
            else upgradedGhost->setState(UPGRADED_GHOST_BLIND);
            break;
        default:
            break;
    }
}
Direction PlayState::calculateMysteryDirection() {
    Position newPosition;
    double distance, minDistance = static_cast<double>(1e12);
    Direction newDirection = NONE;

    for (int direction = LEFT; direction <= RIGHT; direction++) {
        if (mystery->getDirection() == -direction || direction == NONE)
            continue;
        distance = static_cast<double>(1e12);
        if (!Map::isWallAt(mystery->getNextPosition(static_cast<Direction>(direction)))){
            newPosition = mystery->getPosition();
            switch (direction) {
                case UP:
                    newPosition.y -= 42;
                    break;
                case DOWN:
                    newPosition.y += 42;
                    break;
                case LEFT:
                    newPosition.x -= 42;
                    break;
                case RIGHT:
                    newPosition.x += 42;
                    break;
                default:
                    break;
            }
            distance = Distance::Euclidean(newPosition, mystery->getTarget());
        }
        if (minDistance > distance){
            minDistance = distance;
            newDirection = static_cast<Direction>(direction);
        }
    }
    return newDirection;
}

void PlayState::handleMysteryMove() {
    if (mystery->getCurrentState() == MYSTERY_STAND || mystery->getCurrentState() == MYSTERY_SPEECH)
        return;
    if (mystery->checkPosition() || mystery->getPosition() == mystery->getStartPosition())
        mystery->setDirection(calculateMysteryDirection());
}

void PlayState::initUpgraded(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    mystery->setTarget(MYSTERY_POSITION);
    ghost->setTarget(ghost->getUpgradePosition());
    engine->getAudio()->play(SIREN, true, 0);
}

void PlayState::handleUpgrade(Ghost *ghost, UpgradedGhost *upgradedGhost) {
    handleMysteryMove();
    mystery->handleState();

    handleGhostMove(ghost);
    upgradedGhost->handleState();

    if(ghost->getCurrentState() != GHOST_UPGRADE && ghost->getPosition() != ghost->getTarget()){
        mystery->setState(MYSTERY_SPEECH);
        engine->getAudio()->pauseChannel(0);
        engine->getAudio()->play(UPGRADE);
        ghost->setState(GHOST_UPGRADE);
    }
    if (ghost->isGhostUpgrade() && upgradedGhost->getCurrentState() != UPGRADED_GHOST_APPEAR)
        upgradedGhost->setState(UPGRADED_GHOST_APPEAR);
    if (upgradedGhost->isUpgradedGhostAppear()){
        engine->getAudio()->stopChannel();
        engine->getAudio()->resumeChannel(0);
        setState(NEXT_LEVEL);
    }
}
void PlayState::updateButton(int change) {
    currentPauseButton += change;
    isChoosing[currentPauseButton - change] = false;
    if (currentPauseButton > 1){
        currentPauseButton = 0;
        isChoosing[currentPauseButton] = true;
    }
    else if (currentPauseButton < 0){
        currentPauseButton = 1;
        isChoosing[currentPauseButton] = true;
    }
    else isChoosing[currentPauseButton] = true;
}
PlayState::~PlayState() {
    engine = nullptr;

    delete pacman;

    delete mystery;

    delete blinky;
    delete deadly;

    delete pinky;
    delete speedy;

    delete inky;
    delete freezy;

    delete clyde;
    delete invisy;

    delete map;
}







