//
// Created by MZuy on 3/28/2023.
//

#include "Ghost.h"

Ghost::Ghost(GHOST_TYPE type, SDL_Renderer* renderer, Timer* _timer) {
    consoleGhost = new Log("Ghost");

    objectRenderer = renderer;

    timer = _timer;

    ghostType = type;

    frameCount = GHOST_FRAME;

    velocity = GHOST_VELOCITY;

    ghostManager = new textureManager();

    Ghost::setGhostFrameClip();

    textures = ghostManager->loadTexture(DEAD_N_FRIGHTEN_TEXTURE_PATH, objectRenderer);

    ghostTextures[ghostType] = ghostManager->loadTexture(GHOST_TEXTURE_PATH[ghostType], objectRenderer);
    ghostTexture = ghostTextures[ghostType];

    switch (ghostType) {
        case BLINKY:
            objectType = OBJECT_BLINKY;
            break;
        case PINKY:
            objectType = OBJECT_PINKY;
            break;
        case INKY:
            objectType = OBJECT_INKY;
            break;
        case CLYDE:
            objectType = OBJECT_CLYDE;
            break;
        default:
            break;
    }

    position = START_GHOST_POSITION[ghostType];

    tileID = {position.x / 24, position.y / 24};

    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    setState(GHOST_INIT);
    setMode(GHOST_NORMAL_MODE);
}
void Ghost::setGhostFrameClip() {
    for (int i = 0; i < 11; i++){
        ghostFrameClip[i] = {i * (OBJECT_SIZE + 6), 0, OBJECT_SIZE, OBJECT_SIZE};
    }
}

void Ghost::render(){
    switch (stateQueue.front()){
        case GHOST_INIT: case GHOST_STAND:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
        case GHOST_SCATTER: case GHOST_CHASE : case GHOST_BLIND:
            switch (currentDirection) {
                case UP:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame], destRect, objectRenderer);
                    break;
                case RIGHT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 2], destRect, objectRenderer);
                    break;
                case DOWN:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 4], destRect, objectRenderer);
                    break;
                case LEFT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 6], destRect, objectRenderer);
                    break;
                default:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[ghostType], destRect, objectRenderer);
                    break;
            }
            break;
        case GHOST_UPGRADE:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[frame + 8], destRect, objectRenderer);
            break;
        case GHOST_REBORN: case GHOST_WAS_EATEN:
            switch (currentDirection) {
                case UP:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 4], destRect, objectRenderer);
                    break;
                case RIGHT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 5], destRect, objectRenderer);
                    break;
                case DOWN:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 6], destRect, objectRenderer);
                    break;
                case LEFT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 7], destRect, objectRenderer);
                    break;
                case NONE:
                    break;
            }
            break;
        case GHOST_FRIGHTEN:
            ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame], destRect, objectRenderer);
            break;
        case GHOST_FREEZE:
            switch (currentDirection) {
                case UP:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[1], destRect, objectRenderer);
                    break;
                case RIGHT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[3], destRect, objectRenderer);
                    break;
                case DOWN:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[5], destRect, objectRenderer);
                    break;
                case LEFT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[7], destRect, objectRenderer);
                    break;
                case NONE:
                    break;
            }
        default:
            ghostManager->drawTexture(ghostTexture, ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
    }
}
void Ghost::update() {
    if (stateQueue.empty())
        return;

    handleState();
    handleMode();

    if (position.x < 0)
        setPosition({631, position.y});
    else if (position.x > 630)
        setPosition({-1, position.y});
    if (position.y < 156 && currentDirection == UP)
        currentDirection = NONE;
    else if (position.y > 852 && currentDirection == DOWN)
        currentDirection = NONE;

    frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    GHOST_STATE currentState = stateQueue.back();

    if (currentState == GHOST_UPGRADE) {
        Uint32 tick = GHOST_STATE_TIME[GHOST_UPGRADE] - (timer->getTicks() - startState.front());
        frameCount = GHOST_UPGRADE_FRAME / 3;
        if (tick <= 2000) {
            frameCount = GHOST_UPGRADE_FRAME;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
        } else {
            frameCount = GHOST_UPGRADE_FRAME / 3;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
        }
        return;
    }
    switch (currentState) {
        case GHOST_INIT: case GHOST_STAND: case GHOST_CHASE:
        case GHOST_SCATTER: case GHOST_BLIND: case GHOST_FREEZE:
            frameCount = GHOST_FRAME;
            ghostTexture =  ghostTextures[ghostType];
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
            break;
        case GHOST_REBORN: case GHOST_WAS_EATEN:
            frameCount = 1;
            ghostTexture = textures;
            frame = 1;
            break;
        case GHOST_FRIGHTEN:
            frameCount = FRIGHTENED_GHOST_FRAME;
            ghostTexture = textures;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
            break;
        default:
            break;
    }

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    if (currentMode[GHOST_FRIGHTEN_MODE]) {
        Uint32 tick = GHOST_MODE_TIME[GHOST_FRIGHTEN_MODE] - (timer->getTicks() - startMode[GHOST_FRIGHTEN_MODE]);
        if (tick <= 2000) {
            frameCount = 4;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
        } else {
            frameCount = 2;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
        }
    } else {
        frameCount = 2;
        frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
    }
    if (currentMode[GHOST_FREEZE_MODE]) {
        Uint32 tick = GHOST_MODE_TIME[GHOST_FREEZE_MODE] - (timer->getTicks() - startMode[GHOST_FREEZE_MODE]);
        textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
        if (tick <= 2000) {
            frameCount = 2;
            textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
            if (frame)
                textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        } else {
            frameCount = 1;
            frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
        }
    } else {
        frameCount = 2;
        textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        frame = (timer->getTicks() / GHOST_ANIMATION_SPEED) % frameCount;
    }
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    tileID.x = (position.x - 3) % 24 >= 12 ? (position.x - 3) / 24 + 1 : (position.x - 3) / 24;
    tileID.y = (position.y + 9 - 144) % 24 >= 12 ? (position.y + 9 - 144) / 24 + 1 : (position.y + 9 - 144) / 24;
}

void Ghost::move() {
    Object::move(currentDirection, velocity);
    /*if (ghostType == BLINKY){
         std::cerr << "Blinky's velocity " << getVelocity() << std::endl;
         std::cerr << "Blinky x = " << getPosition().x << "; y = " << getPosition().y << std::endl;
     }*/
}
void Ghost::initState(){
    if (stateQueue.empty())
        return;
    switch (stateQueue.front()) {
        case GHOST_INIT:
            currentDirection = NONE;

            velocity = GHOST_VELOCITY;
            frameCount = GHOST_FRAME;

            setPosition(START_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            isOutCage = false;

            setMode(GHOST_NORMAL_MODE);

            unsetState();

            break;
        case GHOST_STAND:
            currentDirection = NONE;

            velocity = 0;
            frameCount = GHOST_FRAME;

            setPosition(START_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case GHOST_CHASE:
        case GHOST_SCATTER:
            velocity = GHOST_VELOCITY;
            frameCount = GHOST_FRAME;

            break;
        case GHOST_FRIGHTEN:
            currentDirection = static_cast<Direction>(-currentDirection);

            velocity = GHOST_VELOCITY;
            frameCount = FRIGHTENED_GHOST_FRAME;

            break;
        case GHOST_BLIND:
            velocity = GHOST_VELOCITY;
            frameCount = GHOST_FRAME;

            break;
        case GHOST_IS_EATEN:
            currentDirection = NONE;

            velocity = 0;

            break;
        case GHOST_REBORN:
            setPosition(START_GHOST_POSITION[BLINKY]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case GHOST_WAS_EATEN:{
            currentDirection = NONE;

            velocity = GHOST_VELOCITY * 6;
            frameCount = GHOST_FRAME / 2;

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        }
        case GHOST_FREEZE:
            velocity = 0;
            frameCount = GHOST_FRAME;

            break;
        case GHOST_UPGRADE:
            currentDirection = NONE;

            velocity = 0;
            frameCount = GHOST_UPGRADE_FRAME;

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

        default:
            break;
    }
}
void Ghost::setState(GHOST_STATE newState) {
    if (newState == GHOST_UPGRADE){
        while (!stateQueue.empty()) {
            stateQueue.pop();
            startState.pop();
        }
        stateQueue.emplace(newState);
        startState.emplace(timer->getTicks());
        initState();
        return;
    }
    if (!stateQueue.empty())
        if (stateQueue.front() == GHOST_STAND && newState != GHOST_SCATTER)
            return;
    switch (newState){
        case GHOST_INIT:
            while (!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            break;
        case GHOST_SCATTER:
            break;
        case GHOST_FRIGHTEN:
            setMode(GHOST_FRIGHTEN_MODE);
            break;
        case GHOST_IS_EATEN:
            while (!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            setMode(GHOST_NORMAL_MODE);
            break;
        case GHOST_FREEZE:
            if (!stateQueue.empty())
                if (stateQueue.front() == GHOST_WAS_EATEN || stateQueue.front() == GHOST_REBORN)
                    return;
            if (stateQueue.front() == GHOST_FRIGHTEN){
                setMode(GHOST_FREEZE_MODE);
                velocity = 0;
                return;
            }
            while (!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            setMode(GHOST_FREEZE_MODE);
            velocity = 0;
            break;
        case GHOST_BLIND:
            setMode(GHOST_BLIND_MODE);
            break;
        default:
            break;
    }
    if (!stateQueue.empty()) {
        if (stateQueue.front() == GHOST_STAND && newState == GHOST_SCATTER) {
            isOutCage = true;
            leaveCage();
            stateQueue.pop();
            startState.pop();
        }
        if (stateQueue.front() == GHOST_WAS_EATEN)
            return;
    }
    stateQueue.emplace(newState);
    startState.emplace(timer->getTicks());
    if (stateQueue.size() == 1)
        initState();
}

void Ghost::handleState() {
    if (!stateQueue.empty()){
        if (stateQueue.back() == GHOST_UPGRADE){
            Uint32 tick = timer->getTicks() - startState.front();
            if (tick > GHOST_STATE_TIME[stateQueue.back()]) {
                isUpgrade = true;
                return;
            }
            return;
        }
        if (stateQueue.back() == GHOST_WAS_EATEN){
            if (checkPosition()){
                velocity = GHOST_VELOCITY * 4;
            }
            else {
                velocity = GHOST_VELOCITY;
            }
        }
        if (stateQueue.front() == GHOST_WAS_EATEN && position.x == START_GHOST_POSITION[BLINKY].x && position.y == START_GHOST_POSITION[BLINKY].y + 24 * 5){
            stateQueue.pop();
            startState.pop();
            setState(GHOST_SCATTER);
            return;
        }
        if (stateQueue.back() == GHOST_FREEZE || stateQueue.back() == GHOST_REBORN){
            unsetState();
            return;
        }
        else if (stateQueue.front() == GHOST_STAND)
            return;
    }
    if (currentDirection != NONE)
        move();
    unsetState();
}
void Ghost::unsetState() {
    if (stateQueue.empty() || stateQueue.front() == GHOST_STAND)
        return;
    if (stateQueue.front() == GHOST_REBORN){
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick >= GHOST_STATE_TIME[stateQueue.front()]){
            stateQueue.pop();
            startState.pop();
            setState(GHOST_CHASE);
        }
    }
    if (stateQueue.size() == 2) {
        if ((stateQueue.front() == GHOST_FRIGHTEN && stateQueue.back() == GHOST_FREEZE)) {
            Uint32 tick = timer->getTicks() - startState.front();
            if (tick >= GHOST_STATE_TIME[stateQueue.front()]) {
                stateQueue.pop();
                startState.pop();
                setState(GHOST_CHASE);
            }
        }
        else if (stateQueue.front() == GHOST_SCATTER && stateQueue.back() == GHOST_FREEZE){
            Uint32 tick = timer->getTicks() - startState.front();
            if (tick >= GHOST_STATE_TIME[stateQueue.front()]) {
                stateQueue.pop();
                startState.pop();
                setState(GHOST_SCATTER);
            }
        }
    }
    if (!checkPosition() && stateQueue.front() != GHOST_IS_EATEN && stateQueue.front() != GHOST_FREEZE
        && stateQueue.front() != GHOST_INIT)
        return;
    if (stateQueue.back() == GHOST_FRIGHTEN && stateQueue.size() > 1)
        while (stateQueue.front() != GHOST_FRIGHTEN){
            stateQueue.pop();
            startState.pop();
        }
    if (stateQueue.front() == GHOST_WAS_EATEN) {
        if (checkPosition() && position == target) {
            stateQueue.pop();
            startState.pop();
            setState(GHOST_REBORN);
        }
        return;
    }
    if (stateQueue.back() == GHOST_BLIND && stateQueue.size() > 1)
        if (checkPosition())
            while (stateQueue.front() != GHOST_BLIND){
                stateQueue.pop();
                startState.pop();
            }
    lastGhostState = GHOST_UNSET;
    while (!stateQueue.empty()){
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick >= GHOST_STATE_TIME[stateQueue.front()]){
            lastGhostState = stateQueue.front();
            stateQueue.pop();
            startState.pop();
        }
        else
            break;
    }
    if (lastGhostState != GHOST_UNSET){
        switch (lastGhostState){
            case GHOST_INIT:
                setState(GHOST_STAND);
                break;
            case GHOST_CHASE:
                setState(GHOST_SCATTER);
                break;
            case GHOST_SCATTER:
                setState(GHOST_CHASE);
                break;
            case GHOST_FRIGHTEN:
                if (!stateQueue.empty())
                    if (stateQueue.front() != GHOST_FREEZE)
                        break;
                setState(GHOST_CHASE);
                break;
            case GHOST_IS_EATEN:
                setState(GHOST_WAS_EATEN);
                break;
            case GHOST_FREEZE: case GHOST_BLIND:
                setState(GHOST_CHASE);
                break;
            default:
                break;
        }
        initState();
    }
}

void Ghost::setMode(const GHOST_MODE &mode) {
    currentMode[mode] = true;
    startMode[mode] = timer->getTicks();
    switch (mode){
        case GHOST_NORMAL_MODE:
            unsetMode(GHOST_FRIGHTEN_MODE);
            unsetMode(GHOST_FREEZE_MODE);
            unsetMode(GHOST_BLIND_MODE);
            break;
        default:
            break;
    }
}
void Ghost::handleMode() {
    for (int i = GHOST_FRIGHTEN_MODE; i < GHOST_MODE_TOTAL; i++)
        if (currentMode[i]){
            Uint32 tick = timer->getTicks() - startMode[i];
            if (tick >= GHOST_MODE_TIME[i])
                unsetMode((GHOST_MODE)i);
        }
}
void Ghost::unsetMode(const GHOST_MODE &mode) {
    currentMode[mode] = false;
    startMode[mode] = 0;
    velocity = GHOST_VELOCITY;
}
Ghost::~Ghost() {
    delete consoleGhost;
    consoleGhost = nullptr;
    SDL_DestroyTexture(ghostTexture);
    ghostTexture = nullptr;
}