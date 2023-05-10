//
// Created by MZuy on 5/4/2023.
//

#include "UpgradedGhost.h"

UpgradedGhost::UpgradedGhost(UPGRADED_GHOST_TYPE type, SDL_Renderer* _renderer,  Timer *_timer){
    consoleGhost = new Log("Ghost");

    objectRenderer = _renderer;

    timer = _timer;

    ghostType = type;

    frameCount = UPGRADED_GHOST_FRAME;

    velocity = UPGRADED_GHOST_VELOCITY;

    ghostManager = new textureManager();

    UpgradedGhost::setGhostFrameClip();

    CanMove = true;

    ghostTexture = ghostManager->loadTexture(UPGRADED_GHOST_TEXTURE_PATH[ghostType], objectRenderer);

    switch (ghostType) {
        case DEADLY:
            objectType = OBJECT_DEADLY;
            break;
        case SPEEDY:
            objectType = OBJECT_SPEEDY;
            break;
        case FREEZY:
            objectType = OBJECT_FREEZY;
            break;
        case INVISY:
            objectType = OBJECT_INVISY;
            break;
        default:
            break;
    }

    position = START_UPGRADED_GHOST_POSITION[ghostType];

    tileID = {position.x / 24, position.y / 24};

    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    setState(UPGRADED_GHOST_INIT);
    setMode(UPGRADED_GHOST_NORMAL);
}
void UpgradedGhost::speedAnimation() {
    if (!lastPoint.empty() && !currentMode[UPGRADED_GHOST_FREEZE_MODE]) {
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
void UpgradedGhost::setGhostFrameClip() {
    for (int i = 0; i < 8; i++){
        ghostFrameClip[i] = {i * (OBJECT_PIXEL + 6), 0,OBJECT_PIXEL, OBJECT_PIXEL};
    }
}
void UpgradedGhost::render() {
    switch (stateQueue.front()) {
        case UPGRADED_GHOST_INIT:
        case UPGRADED_GHOST_STAND:
            ghostManager->drawTexture(ghostTexture, ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
        case UPGRADED_GHOST_SCATTER:
        case UPGRADED_GHOST_CHASE:
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
        case UPGRADED_GHOST_REBORN: case UPGRADED_GHOST_IS_EATING:
            if (isGhostMode(UPGRADED_GHOST_SPEED_UP_MODE))
                speedAnimation();
            else
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
        case UPGRADED_GHOST_FRIGHTEN:
            ghostManager->drawTexture(ghostTexture, ghostFrameClip[frame], destRect, objectRenderer);
            break;
        default:
            ghostManager->drawTexture(ghostTexture, ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
    }
}
void UpgradedGhost::update() {
    if (stateQueue.empty())
        return;

    handleState();
    handleMode();

    if (position.x < 0)
        setPosition({631, position.y});
    else if (position.x > 630)
        setPosition({-1, position.y});

    move();

    frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    UPGRADED_GHOST_STATE currentState = stateQueue.back();

    switch (currentState) {
        case UPGRADED_GHOST_INIT: case UPGRADED_GHOST_STAND: case UPGRADED_GHOST_APPEAR: case UPGRADED_GHOST_CHASE:
        case UPGRADED_GHOST_SCATTER: case UPGRADED_GHOST_BLIND: case UPGRADED_GHOST_FREEZE:
            frameCount = UPGRADED_GHOST_FRAME;
            ghostTexture = ghostManager->loadTexture(UPGRADED_GHOST_TEXTURE_PATH[ghostType], objectRenderer);
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
            break;
        case UPGRADED_GHOST_REBORN: case UPGRADED_GHOST_IS_EATING:
            frameCount = 1;
            ghostTexture = ghostManager->loadTexture("../Assets/ghost/Ghost.png", objectRenderer);
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
            break;
        case UPGRADED_GHOST_FRIGHTEN:
            frameCount = UPGRADED_FRIGHTENED_GHOST_FRAME;
            ghostTexture = ghostManager->loadTexture("../Assets/ghost/Ghost.png", objectRenderer);
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
            break;
        default:
            break;
    }

    textureManager::setTextureAlphaMod(ghostTexture, 0xFF);

    if (currentMode[UPGRADED_GHOST_FRIGHTEN_MODE]) {
        Uint32 tick = UPGRADED_GHOST_MODE_TIME[UPGRADED_GHOST_FRIGHTEN_MODE] - (timer->getTicks() - startMode[UPGRADED_GHOST_FRIGHTEN_MODE]);
        if (tick <= 2000) {
            frameCount = 4;
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
        } else {
            frameCount = 2;
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
        }
    } else {
        frameCount = 2;
        frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
    }
    if (currentMode[UPGRADED_GHOST_FREEZE_MODE]) {
        Uint32 tick = UPGRADED_GHOST_MODE_TIME[UPGRADED_GHOST_FREEZE_MODE] - (timer->getTicks() - startMode[UPGRADED_GHOST_FREEZE_MODE]);
        textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
        if (tick <= 2000) {
            frameCount = 2;
            textureManager::setTextureAlphaMod(ghostTexture, 0x7F);
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
            if (tick % 200 != 0)
                textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        } else {
            frameCount = 1;
            frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
        }
    } else {
        frameCount = 2;
        textureManager::setTextureAlphaMod(ghostTexture, 0xFF);
        frame = (timer->getTicks() / UPGRADED_ANIMATION_SPEED) % frameCount;
    }
    if (currentMode[UPGRADED_GHOST_SPEED_UP_MODE]) {
        lastPoint.emplace_front(position);
        if (lastPoint.size() > 15)
            lastPoint.pop_back();
    }
    else {
        lastPoint.emplace_front(position);
        if (!lastPoint.empty())
            lastPoint.pop_back();
    }
    if (currentMode[UPGRADED_GHOST_INVISIBLE_MODE]){
        Uint32 tick = UPGRADED_GHOST_MODE_TIME[UPGRADED_GHOST_INVISIBLE_MODE] - (timer->getTicks() - startMode[UPGRADED_GHOST_INVISIBLE_MODE]);
        Uint32 tickLeft = timer->getTicks() - startMode[UPGRADED_GHOST_INVISIBLE_MODE];
        Uint32 tempAlpha = 0;

        if (tick <= 2000)
            tempAlpha = 0xFF / (1 - tick / 2000);
        else if (tick >= 4000)
            tempAlpha = 0xFF / (1 - tickLeft/ 2000);

        textureManager::setTextureAlphaMod(ghostTexture, tempAlpha);
    }
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    tileID.x = (position.x - 3) % 24 >= 12 ? (position.x - 3) / 24 + 1 : (position.x - 3) / 24;
    tileID.y = (position.y + 9 - 144) % 24 >= 12 ? (position.y + 9 - 144) / 24 + 1 : (position.y + 9 - 144) / 24;
}

void UpgradedGhost::move() {
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

void UpgradedGhost::initState() {
    if (stateQueue.empty())
        return;
    switch (stateQueue.front()){
        case UPGRADED_GHOST_INIT:
            currentDirection = NONE;

            velocity = UPGRADED_GHOST_VELOCITY;
            frameCount = UPGRADED_GHOST_FRAME;

            setPosition(START_UPGRADED_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            isOutCage = false;

            setMode(UPGRADED_GHOST_NORMAL);

            lastPoint.clear();

            unsetState();

            break;
        case UPGRADED_GHOST_WAIT:
            currentDirection = NONE;

            velocity = 0;
            frameCount = UPGRADED_GHOST_FRAME;

            setPosition(UPGRADED_GHOST_APPEAR_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            setMode(UPGRADED_GHOST_NORMAL);

            break;
        case UPGRADED_GHOST_APPEAR:
            isAppear = false;

            currentDirection = NONE;

            velocity = 0;

            setPosition(UPGRADED_GHOST_APPEAR_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            frameCount = UPGRADED_GHOST_FRAME / 2;
        case UPGRADED_GHOST_STAND:
            currentDirection = NONE;

            velocity = UPGRADED_GHOST_VELOCITY;
            frameCount = UPGRADED_GHOST_FRAME;

            setPosition(START_UPGRADED_GHOST_POSITION[ghostType]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case UPGRADED_GHOST_CHASE: case UPGRADED_GHOST_SCATTER:
            velocity = UPGRADED_GHOST_VELOCITY;
            frameCount = UPGRADED_GHOST_FRAME;

            break;
        case UPGRADED_GHOST_FRIGHTEN:
            currentDirection = static_cast<Direction>(-currentDirection);

            velocity = UPGRADED_GHOST_VELOCITY;
            frameCount = UPGRADED_FRIGHTENED_GHOST_FRAME;

            break;
        case UPGRADED_GHOST_BLIND:
            velocity = UPGRADED_GHOST_VELOCITY;
            frameCount = UPGRADED_GHOST_FRAME;

            break;
        case UPGRADED_GHOST_IS_EATING:
            currentDirection = NONE;

            velocity = 0;

            break;
        case UPGRADED_GHOST_REBORN:
            setPosition(START_UPGRADED_GHOST_POSITION[DEADLY]);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case UPGRADED_GHOST_WAS_EATEN:
            currentDirection = NONE;

            velocity = UPGRADED_GHOST_VELOCITY * 4;
            frameCount = UPGRADED_GHOST_FRAME / 2;

            setPosition(position);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case UPGRADED_GHOST_FREEZE:
            velocity = 0;
            frameCount = UPGRADED_GHOST_FRAME;

            break;
        default:
            break;
    }
}

void UpgradedGhost::setState(UPGRADED_GHOST_STATE newState) {
    if (newState == UPGRADED_GHOST_APPEAR){
        while (!stateQueue.empty()){
            stateQueue.pop();
            startState.pop();
        }
        stateQueue.push(newState);
        startState.push(timer->getTicks());
        initState();
        return;
    }
    if (!stateQueue.empty())
        if (stateQueue.front() == UPGRADED_GHOST_STAND && newState != UPGRADED_GHOST_SCATTER)
            return;
    switch (newState){
        case UPGRADED_GHOST_INIT:
        case UPGRADED_GHOST_WAIT:
            while (!stateQueue.empty()){
                stateQueue.pop();
                startState.pop();
            }
            break;
        case UPGRADED_GHOST_SCATTER:
            if (ghostType != DEADLY){
                Position temp = START_UPGRADED_GHOST_POSITION[DEADLY];
                temp.y += 24 * 5;
                setPosition(temp);
            }
        case UPGRADED_GHOST_FRIGHTEN:
            setMode(UPGRADED_GHOST_FRIGHTEN_MODE);
            break;
        case UPGRADED_GHOST_IS_EATING:
            while (!stateQueue.empty()){
                stateQueue.pop();
                startState.pop();
            }

            setMode(UPGRADED_GHOST_NORMAL);
            break;
        case UPGRADED_GHOST_FREEZE:
            if (!stateQueue.empty()){
                if (stateQueue.front() == UPGRADED_GHOST_WAS_EATEN)
                    break;
                if (stateQueue.front() == UPGRADED_GHOST_FRIGHTEN)
                    break;
                if (stateQueue.front() == UPGRADED_GHOST_REBORN)
                    break;
            }
            while (!stateQueue.empty()){
                stateQueue.pop();
                startState.pop();
            }

            setMode(UPGRADED_GHOST_FREEZE_MODE);
            break;
        case UPGRADED_GHOST_BLIND:
            setMode(UPGRADED_GHOST_BLIND_MODE);
            break;
        default:
            break;
    }
    if (!stateQueue.empty()){
        if (stateQueue.front() == UPGRADED_GHOST_STAND && newState == UPGRADED_GHOST_SCATTER){
            isOutCage = true;
            leaveCage();
            stateQueue.pop();
            startState.pop();
        }
        if (stateQueue.front() == UPGRADED_GHOST_WAS_EATEN){
            return;
        }
    }
    stateQueue.push(newState);
    startState.push(timer->getTicks());
    if (stateQueue.size() == 1)
        initState();
}

void UpgradedGhost::handleState() {
    if (!stateQueue.empty()){
        if (stateQueue.back() == UPGRADED_GHOST_APPEAR){
            Uint32 tick= timer->getTicks() - startState.front();
            if (tick >= UPGRADED_GHOST_STATE_TIME[stateQueue.front()]){
                isAppear = true;
                return;
            }
            return;
        }
        if (stateQueue.back() == UPGRADED_GHOST_FREEZE || stateQueue.back() == UPGRADED_GHOST_REBORN){
            unsetState();
            return;
        }
        else
        if (stateQueue.front() == UPGRADED_GHOST_STAND || stateQueue.front() == UPGRADED_GHOST_WAIT)
            return;
    }
    if (currentDirection != NONE && CanMove)
        move();

    unsetState();
}

void UpgradedGhost::unsetState() {
    if (stateQueue.empty() || stateQueue.front() == UPGRADED_GHOST_STAND)
        return;
    if (stateQueue.front() == UPGRADED_GHOST_REBORN){
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick >= UPGRADED_GHOST_STATE_TIME[stateQueue.front()]){
            stateQueue.pop();
            startState.pop();
            setState(UPGRADED_GHOST_CHASE);
        }
    }
    if (stateQueue.size() == 2)
        if (stateQueue.front() == UPGRADED_GHOST_FRIGHTEN && stateQueue.back() == UPGRADED_GHOST_FREEZE){
            Uint32 tick = timer->getTicks() - startState.front();
            if (tick >= UPGRADED_GHOST_STATE_TIME[stateQueue.front()]){
                stateQueue.pop();
                startState.pop();
                setState(UPGRADED_GHOST_CHASE);
            }
        }
    if (!checkPosition() && stateQueue.front() != UPGRADED_GHOST_IS_EATING
        && stateQueue.front() != UPGRADED_GHOST_FREEZE && stateQueue.front() != UPGRADED_GHOST_INIT)
        return;
    if (stateQueue.back() == UPGRADED_GHOST_FRIGHTEN && stateQueue.size() > 1)
        while (stateQueue.front() != UPGRADED_GHOST_FRIGHTEN){
            stateQueue.pop();
            startState.pop();
        }
    if (stateQueue.front() == UPGRADED_GHOST_WAS_EATEN) {
        if (checkPosition() && position == target) {
            stateQueue.pop();
            startState.pop();
            setState(UPGRADED_GHOST_REBORN);
        }
        return;
    }
    if (stateQueue.back() == UPGRADED_GHOST_BLIND && stateQueue.size() > 1)
        if (checkPosition())
            while (stateQueue.front() != UPGRADED_GHOST_BLIND){
                stateQueue.pop();
                startState.pop();
            }
    UPGRADED_GHOST_STATE lastState = UPGRADED_GHOST_UNSET;
    while (!stateQueue.empty()){
        Uint32 tick = timer->getTicks() - startState.front();
        if (tick >= UPGRADED_GHOST_STATE_TIME[stateQueue.front()]){
            lastState = stateQueue.front();
            stateQueue.pop();
            startState.pop();
        }
        else break;
    }
    if (lastState != UPGRADED_GHOST_UNSET) {
        switch (lastState) {
            case UPGRADED_GHOST_INIT:
                setState(UPGRADED_GHOST_STAND);
                break;
            case UPGRADED_GHOST_CHASE:
                setState(UPGRADED_GHOST_SCATTER);
                break;
            case UPGRADED_GHOST_SCATTER:
                setState(UPGRADED_GHOST_CHASE);
                break;
            case UPGRADED_GHOST_FRIGHTEN:
                if (!stateQueue.empty())
                    if (stateQueue.front() != UPGRADED_GHOST_FREEZE)
                        break;
                setState(UPGRADED_GHOST_CHASE);
                break;
            case UPGRADED_GHOST_IS_EATING:
                setState(UPGRADED_GHOST_WAS_EATEN);
                break;
            case UPGRADED_GHOST_FREEZE:
            case UPGRADED_GHOST_BLIND:
                setState(UPGRADED_GHOST_CHASE);
                break;
            default:
                break;
        }
        initState();
    }
}

void UpgradedGhost::setMode(const UPGRADED_GHOST_MODE &mode) {
    currentMode[mode] = true;
    startMode[mode] = timer->getTicks();
    switch (mode){
        case UPGRADED_GHOST_NORMAL:
            unsetMode(UPGRADED_GHOST_SPEED_UP_MODE);
            unsetMode(UPGRADED_GHOST_INVISIBLE_MODE);
            unsetMode(UPGRADED_GHOST_SLOW_DOWN_MODE);
            unsetMode(UPGRADED_GHOST_FRIGHTEN_MODE);
            unsetMode(UPGRADED_GHOST_FREEZE_MODE);
            unsetMode(UPGRADED_GHOST_BLIND_MODE);
        case UPGRADED_GHOST_SPEED_UP_MODE:
            unsetMode(UPGRADED_GHOST_SLOW_DOWN_MODE);
            if (checkPosition())
                velocity = UPGRADED_GHOST_SPEED_UP_VELOCITY;
            break;
        case UPGRADED_GHOST_SLOW_DOWN_MODE:
            unsetMode(UPGRADED_GHOST_SPEED_UP_MODE);
            if (checkPosition())
                velocity = UPGRADED_GHOST_SLOW_DOWN_VELOCITY;
            break;
        default:
            break;
    }
}

void UpgradedGhost::handleMode() {
    if (stateQueue.front() == UPGRADED_GHOST_WAS_EATEN){
        if (checkPosition())
            velocity = UPGRADED_GHOST_VELOCITY * 4;
    }
    else if (checkPosition())
        velocity = UPGRADED_GHOST_VELOCITY;
    for (int i = UPGRADED_GHOST_FRIGHTEN_MODE; i < UPGRADED_GHOST_MODE_TOTAL; i++)
        if (currentMode[i]){
            Uint32 tick = timer->getTicks() - startMode[i];
            if (tick >= UPGRADED_GHOST_MODE_TIME[i])
                unsetMode((UPGRADED_GHOST_MODE)i);
            else if (i == UPGRADED_GHOST_SPEED_UP_MODE && stateQueue.front() != UPGRADED_GHOST_WAS_EATEN){
                if (tick <= 2400 && checkPosition())
                    velocity = UPGRADED_GHOST_SPEED_UP_VELOCITY;
                else if (checkPosition())
                    velocity = UPGRADED_GHOST_SPEED_UP_VELOCITY;
            }
        }
    if (currentMode[UPGRADED_GHOST_SLOW_DOWN_MODE] && stateQueue.front() != UPGRADED_GHOST_WAS_EATEN)
        if (checkPosition())
            velocity = UPGRADED_GHOST_VELOCITY;
}

void UpgradedGhost::unsetMode(const UPGRADED_GHOST_MODE &mode) {
    currentMode[mode] = false;
    startMode[mode] = 0;
}
UpgradedGhost::~UpgradedGhost() {
    delete timer;
    timer = nullptr;
    delete consoleGhost;
    consoleGhost = nullptr;
}

