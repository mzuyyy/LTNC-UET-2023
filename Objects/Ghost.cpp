//
// Created by MZuy on 3/28/2023.
//

#include "Ghost.h"

Ghost::Ghost(GHOST_TYPE type, SDL_Renderer* renderer, Timer* _timer) {
    consoleGhost = new Log("Ghost");

    objectRenderer = renderer;

    timer = _timer;

    ghostType = type;

    frameCount = 2;

    velocity = GHOST_VELOCITY;

    ghostManager = new textureManager();

    Ghost::setGhostFrameClip();

    ghostTexture = ghostManager->loadTexture(GHOST_TEXTURE_PATH[ghostType], objectRenderer);

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
        ghostFrameClip[i].x = i * (OBJECT_PIXEL + 6);
        ghostFrameClip[i].y = 0;
        ghostFrameClip[i].w = OBJECT_PIXEL;
        ghostFrameClip[i].h = OBJECT_PIXEL;
    }
}
void Ghost::speedAnimation() {
    if (!lastPoint.empty() && !currentMode[GHOST_FREEZE_MODE]) {
        lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
        for (int i = 0; i < lastPoint.size(); i++){
            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
            textureManager::setTextureAlphaMod(ghostTexture, lastAlphaMod / (i + 1));
            switch(currentDirection) {
                case UP:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame], lastDest, objectRenderer);
                    break;
                case RIGHT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 2], lastDest, objectRenderer);
                    break;
                case DOWN:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 4], lastDest, objectRenderer);
                    break;
                case LEFT:
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 6], lastDest, objectRenderer);
                    break;
                default:
                    break;
            }
        }
        textureManager::setTextureAlphaMod(ghostTexture, lastAlphaMod);
    }
}
void Ghost::render(){
    switch (stateQueue.front()){
        case GHOST_INIT: case GHOST_STAND:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
        case GHOST_SCATTER: case GHOST_CHASE:
            switch (currentDirection) {
                case UP:
                    if (currentMode[GHOST_SPEED_UP])
                        speedAnimation();
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame], destRect, objectRenderer);
                    break;
                case RIGHT:
                    if (currentMode[GHOST_SPEED_UP])
                        speedAnimation();
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 2], destRect, objectRenderer);
                    break;
                case DOWN:
                    if (currentMode[GHOST_SPEED_UP])
                        speedAnimation();
                    ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame + 4], destRect, objectRenderer);
                    break;
                case LEFT:
                    if (currentMode[GHOST_SPEED_UP])
                        speedAnimation();
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
        case GHOST_REBORN: case GHOST_EATEN:
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

    if(position.x < 0)
        setPosition({631, position.y});
    else if(position.x > 630)
        setPosition({-1, position.y});

    move();

    frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    GHOST_STATE currentState = stateQueue.back();

    switch (currentState){
        case GHOST_INIT: case GHOST_STAND: case GHOST_CHASE: case GHOST_SCATTER: case GHOST_BLIND: case GHOST_FREEZE:
            frameCount = GHOST_FRAME;
            ghostTexture = ghostManager->loadTexture(GHOST_TEXTURE_PATH[ghostType], objectRenderer);
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            break;
        case GHOST_REBORN: case GHOST_EATEN:
            frameCount = 1;
            ghostTexture = ghostManager->loadTexture("../Assets/ghost/Ghost.png", objectRenderer);
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            break;
        case GHOST_FRIGHTEN:
            frameCount = FRIGHTENED_GHOST_FRAME;
            ghostTexture = ghostManager->loadTexture("../Assets/ghost/Ghost.png", objectRenderer);
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            break;
        case GHOST_UPGRADE:
            Uint32 tick = GHOST_STATE_TIME[GHOST_UPGRADE] - (timer->getTicks() - startState.front());
            frameCount = GHOST_UPGRADE_FRAME / 3;
            if(tick <= 2000){
                frameCount = GHOST_UPGRADE_FRAME;
                frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            }
            else{
                frameCount = GHOST_UPGRADE_FRAME / 3;
                frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            }
            return;
    }
    if (currentMode[GHOST_SPEED_UP]){
        lastPoint.emplace_front(position);
        if (lastPoint.size() > 15)
            lastPoint.pop_back();
    }
    else {
        lastPoint.emplace_front(position);
        if (!lastPoint.empty())
            lastPoint.pop_back();
    }
    if (currentMode[GHOST_INVISIBLE]){
        Uint32 tick = GHOST_MODE_TIME[GHOST_INVISIBLE] - (timer->getTicks() - startMode[GHOST_INVISIBLE]);
        Uint32 tempAlpha = 0;

        if (tick <= 2000)
            tempAlpha = 0xFF / (1 - tick / 2000);
        else if (tick >= 4000)
            tempAlpha = 0xFF / (1 - (GHOST_MODE_TIME[GHOST_INVISIBLE] - tick) / 2000);

        textureManager::setTextureAlphaMod(ghostTexture, tempAlpha);
    }

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    if (currentMode[GHOST_FRIGHTEN_MODE]) {
        Uint32 tick = GHOST_MODE_TIME[GHOST_FRIGHTEN_MODE] - (timer->getTicks() - startMode[GHOST_FRIGHTEN_MODE]);
        if (tick <= 2000) {
            frameCount = 4;
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
        } else {
            frameCount = 2;
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
        }
    }
    else{
        frameCount = 2;
        frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
    }
    if (currentMode[GHOST_FREEZE_MODE]) {
        Uint32 tick = GHOST_MODE_TIME[GHOST_FREEZE_MODE] - (timer->getTicks() - startMode[GHOST_FREEZE_MODE]);
        textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
        if (tick <= 2000) {
            frameCount = 2;
            textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
            if (tick % 200 != 0)
                textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        } else {
            frameCount = 1;
            frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
        }
    }
    else{
        frameCount = 2;
        textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        frame = (timer->getTicks() / ANIMATION_SPEED) % frameCount;
    }
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    tileID.x = (position.x - 3) % 24 >= 12 ? (position.x - 3) / 24 + 1 : (position.x - 3) / 24;
    tileID.y = (position.y + 9 - 144) % 24 >= 12 ? (position.y + 9 - 144) / 24 + 1 : (position.y + 9 - 144) / 24;
}
void Ghost::upgrade() {
    setState(GHOST_UPGRADE);
    switch(ghostType){
        case BLINKY:
            ghostType = DEADLY;
            break;
        case PINKY:
            ghostType = SPEEDY;
            break;
        case INKY:
            ghostType = FREEZY;
            break;
        case CLYDE:
            ghostType = INVISY;
            break;
        default:
            break;
    }
    ghostTexture = ghostManager->loadTexture(GHOST_TEXTURE_PATH[ghostType], objectRenderer);
}
void Ghost::move() {
    switch (currentDirection) {
        case UP:
            position.y -= velocity;
            break;
        case DOWN:
            position.y += velocity;
            break;
        case LEFT:
            position.x -= velocity;
            break;
        case RIGHT:
            position.x += velocity;
            break;
        case NONE:
            break;
    }
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};
    if (checkPosition()) {
        tileID = {position.x / 24, position.y / 24};
    }
}
void Ghost::initState(){
    if (stateQueue.empty())
        return;
    GHOST_STATE stateFront = stateQueue.front();
    switch (stateFront)
    {
        case GHOST_INIT:
            velocity = GHOST_VELOCITY;
            frameCount = GHOST_FRAME;

            setPosition(START_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 144 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            isOutCage = false;

            setMode(GHOST_NORMAL_MODE);

            lastPoint.clear();

            while (!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }

            break;
        case GHOST_STAND:
            velocity = 0;
            frameCount = GHOST_FRAME;

            setPosition(START_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 144 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case GHOST_APPEAR:
            velocity = 0;
            frameCount = 1;

            setPosition(UPGRADED_GHOST_APPEAR_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 144 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case GHOST_CHASE: case GHOST_SCATTER:case GHOST_FRIGHTEN: case GHOST_BLIND:
            velocity = GHOST_VELOCITY;
            frameCount = GHOST_FRAME;
            break;
        case GHOST_EATEN:
            velocity = 0;
            break;
        case GHOST_REBORN:
            setPosition(START_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 144 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case GHOST_EAT:
            setMode(GHOST_NORMAL_MODE);

            velocity = EATEN_GHOST_VELOCITY;

            setPosition(position);

            frameCount = EATEN_GHOST_FRAME;

            break;
        case GHOST_FREEZE:
            velocity = 0;

            frameCount = GHOST_FRAME;
            break;
        case GHOST_UPGRADE:
            velocity = 0;

            setPosition(GHOST_UPGRADE_POSITION);

            if (ghostType == MYSTERY) {
                setTarget(UPGRADE_MYSTERY_POSITION);
            }

            position.x = tileID.x * 24 + 3;
            position.y = 144 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            frameCount = GHOST_FRAME;
            break;
        default:
            break;
    }
}
void Ghost::setState(GHOST_STATE newState) {
    if (newState == GHOST_APPEAR || newState == GHOST_UPGRADE) {
        while (!stateQueue.empty()) {
            stateQueue.pop();
            startState.pop();
        }
        stateQueue.emplace(newState);
        startState.emplace(timer->getTicks());
        initState();
        return;
    }
    if (!stateQueue.empty()) {
        if (stateQueue.front() == GHOST_STAND && newState != GHOST_SCATTER)
            return;
    }
    switch (newState){
        case GHOST_INIT:
            if (!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            break;
        case GHOST_FRIGHTEN:
            setMode(GHOST_FRIGHTEN_MODE);
            break;
        case GHOST_EATEN:
            if(!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            setMode(GHOST_NORMAL_MODE);
            break;
        case GHOST_FREEZE:
            if (!stateQueue.empty() && (stateQueue.front() == GHOST_FRIGHTEN || stateQueue.front() == GHOST_EAT || stateQueue.front() == GHOST_REBORN))
                break;
            while(!stateQueue.empty()) {
                stateQueue.pop();
                startState.pop();
            }
            setMode(GHOST_FREEZE_MODE);
            break;
        case GHOST_BLIND:
            setMode(GHOST_BLIND_MODE);
            break;
        default:
            break;
    }
    if (!stateQueue.empty()) {
        if (stateQueue.front() == GHOST_STAND && newState != GHOST_SCATTER) {
            isOutCage = true;
            setPosition(START_GHOST_POSITION[BLINKY]);
            stateQueue.pop();
            startState.pop();
        }
        if (stateQueue.front() == GHOST_EAT)
            return;
    }
    stateQueue.emplace(newState);
    startState.emplace(timer->getTicks());
    if (stateQueue.size() == 1)
        initState();
}

void Ghost::handleState() {
    if(!stateQueue.empty())
        if (stateQueue.back() == GHOST_UPGRADE){
            Uint32 tick = timer->getTicks() - startState.front();
            if(tick > GHOST_STATE_TIME[stateQueue.front()]){
                isUpgrade = true;
                return;
            }
            return;
        }
    if(stateQueue.back() == GHOST_FREEZE || stateQueue.back() == GHOST_REBORN) {
        unsetState();
        return;
    }
    unsetState();
}
void Ghost::unsetState() {
    if (stateQueue.empty() || stateQueue.front() == GHOST_STAND)
        return;
    if (stateQueue.front() == GHOST_REBORN){
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick > GHOST_STATE_TIME[stateQueue.front()]) {
            stateQueue.pop();
            startState.pop();
            setState(GHOST_CHASE);
        }
    }
    if (stateQueue.size() == 2)
        if (stateQueue.front() == GHOST_FREEZE && stateQueue.back() == GHOST_FRIGHTEN) {
            Uint32 tick = timer->getTicks() - startState.front();
            if (tick > GHOST_STATE_TIME[stateQueue.front()]) {
                stateQueue.pop();
                startState.pop();
            }
        }
    if (!checkPosition() && stateQueue.front() != GHOST_EATEN && stateQueue.front() != GHOST_FREEZE && stateQueue.front() != GHOST_INIT)
        return;
    if (stateQueue.size() > 1 && stateQueue.back() == GHOST_FRIGHTEN)
        while (stateQueue.front() != GHOST_FRIGHTEN) {
            stateQueue.pop();
            startState.pop();
        }
    if (stateQueue.size() > 1 && stateQueue.back() == GHOST_EATEN)
        if (checkPosition() && position == target){
            stateQueue.pop();
            startState.pop();
            setState(GHOST_REBORN);
        }
    if (stateQueue.size() > 1 && stateQueue.back() == GHOST_BLIND)
        if (checkPosition())
            while (stateQueue.front() != GHOST_BLIND) {
                stateQueue.pop();
                startState.pop();
            }

    GHOST_STATE lastState = GHOST_INIT;
    while (!stateQueue.empty()) {
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick >= GHOST_STATE_TIME[stateQueue.front()]) {
            lastState = stateQueue.front();
            stateQueue.pop();
            startState.pop();
        }
    }
    if (lastState != GHOST_UNSET) {
        switch (lastState){
            case GHOST_INIT:
                setState(GHOST_STAND);
                break;
            case GHOST_SCATTER: case GHOST_FREEZE: case GHOST_BLIND:
                setState(GHOST_CHASE);
                break;
            case GHOST_CHASE:
                setState(GHOST_SCATTER);
                break;
            case GHOST_FRIGHTEN:
                if (!stateQueue.empty()) {
                    if (stateQueue.front() != GHOST_FREEZE)
                        setState(GHOST_CHASE);
                }
                else setState(GHOST_CHASE);
                break;
            case GHOST_EATEN:
                setState(GHOST_EAT);
                break;
            default:
                break;
        }
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
            unsetMode(GHOST_SLOW_DOWN);
            unsetMode(GHOST_SPEED_UP);
            unsetMode(GHOST_INVISIBLE);
            break;
        case GHOST_SPEED_UP:
            unsetMode(GHOST_SLOW_DOWN);
            if (checkPosition())
                velocity = GHOST_SPEED_UP_VELOCITY;
        case GHOST_FRIGHTEN_MODE:
            unsetMode(GHOST_SPEED_UP);
            unsetMode(GHOST_INVISIBLE);
            break;
        default:
            break;
    }
}
void Ghost::handleMode() {
    if (stateQueue.front() == GHOST_EAT) {
        if (checkPosition())
            velocity = EATEN_GHOST_VELOCITY;
    }
    else{
        if (checkPosition())
            velocity = GHOST_VELOCITY;
    }
    for (int i = GHOST_NORMAL_MODE; i < GHOST_MODE_TOTAL; i++)
        if (currentMode[i]) {
            Uint32 tick = timer->getTicks() - startMode[i];
            if (tick > GHOST_MODE_TIME[i]) {
                unsetMode((GHOST_MODE) i);
            }
            else {
                if (i == GHOST_SPEED_UP && stateQueue.front() != GHOST_EAT){
                    if (tick <= 1600){
                        if (checkPosition())
                            velocity = GHOST_SPEED_UP_VELOCITY;
                    }
                    else {
                        if (checkPosition())
                            velocity = GHOST_VELOCITY;
                    }
                }
            }
        }
    if (currentMode[GHOST_SLOW_DOWN] && stateQueue.front() != GHOST_EAT){
        if (checkPosition())
            velocity = GHOST_SLOW_DOWN_VELOCITY;
    }
}
void Ghost::unsetMode(const GHOST_MODE &mode) {
    currentMode[mode] = false;
    startMode[mode] = 0;
}
Ghost::~Ghost() {
    delete consoleGhost;
    consoleGhost = nullptr;
    SDL_DestroyTexture(ghostTexture);
    ghostTexture = nullptr;
}