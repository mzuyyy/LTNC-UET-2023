//
// Created by MZuy on 3/12/2023.
//

#include "pacman.h"
#include "Object.h"

Pacman::Pacman(SDL_Renderer *renderer, PACMAN_TYPE type) {
    this->pacmanRenderer = renderer;

    pacmanType = type;

    switch(pacmanType){
        case CLASSIC:
            objectType = OBJECT_PACMAN;
            pacmanTexture = pacmanManager->loadTexture(PACMAN_TEXTURE_SHEET, pacmanRenderer);
            break;
        case ANDROID:
            objectType = OBJECT_ANDROID_PACMAN;
            pacmanTexture = pacmanManager->loadTexture(ANDROID_TEXTURE_SHEET, pacmanRenderer);
            break;
        case MS:
            objectType = OBJECT_MS_PACMAN;
            pacmanTexture = pacmanManager->loadTexture(MS_TEXTURE_SHEET, pacmanRenderer);
            break;
    }

    tileID.x = startTileID.x;
    tileID.y = startTileID.y;

    position.x = tileID.x * 24 + 3;
    position.y = 144 + tileID.y * 24 - 9;

    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = OBJECT_SIZE;
    destRect.h = OBJECT_SIZE;

    pacmanState = pacmanStartState;

    lastAlphaMod = 255;

    Pacman::setPacmanFrameClip();
}

void Pacman::update(){
    if (!isDead)
        frame = (SDL_GetTicks() / PACMAN_ANIMATION_SPEED) % PACMAN_FRAME_VALUE;
    else {
        frame = (SDL_GetTicks() / PACMAN_ANIMATION_SPEED) % PACMAN_DEATH_ANIMATION_FRAME;
        pacmanTexture = pacmanManager->loadTexture(DEAD_PACMAN_TEXTURE_SHEET, pacmanRenderer);
        if (frame == 11){
            pacmanTexture = pacmanManager->loadTexture(PACMAN_TEXTURE_SHEET, pacmanRenderer);
        }
    }
    if (Pacman::isDead && pacmanHealth > 0) {
        Pacman::resetPosition();
        return;
    }

    handleState();

    if(position.x < 0)
        setPosition({631, position.y});
    else if(position.x > 630)
        setPosition({-1, position.y});

    if (!Pacman::CanMove || directionQueue.empty()) Pacman::stop();
    else {
        Pacman::move(directionQueue.front(), PACMAN_VELOCITY);
    }
    if (power[SPEED_PACMAN]){
        lastPoint.emplace_front(position.x, position.y);
        if (lastPoint.size() > 10)
            lastPoint.pop_back();
    }
    else {
        lastPoint.emplace_front(position.x, position.y);
        lastPoint.pop_back();
        if (!lastPoint.empty())
            lastPoint.pop_back();
    }
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = OBJECT_SIZE;
    destRect.h = OBJECT_SIZE;

    tileID.x = (position.x - 3) % OBJECT_PIXEL >= 12 ? (position.x - 3) / OBJECT_PIXEL + 1 : (position.x - 3) / OBJECT_PIXEL;
    tileID.y = (position.y + 9 - 144) % 24 >= 12 ? (position.y + 9 - 144) / 24 + 1 : (position.y + 9 - 144) / 24;
}
void Pacman::render() {
    if (isDead) {
        pacmanManager->drawTexture(pacmanTexture, frameClip[frame], destRect, pacmanRenderer);
    } else {
        if (frame >= 3) frame = (frame > 4) ? frame - 4 : 4 - frame;
        switch (pacmanType){
            case CLASSIC:
                switch (directionQueue.front()) {
                    case UP:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame], lastDest, pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame], destRect, pacmanRenderer);
                        break;
                    case RIGHT:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 2], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 2], destRect,
                                                   pacmanRenderer);
                        break;
                    case DOWN:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 4], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 4], destRect,
                                                   pacmanRenderer);
                        break;
                    case LEFT:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 6], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? 0 : frame + 6], destRect,
                                                   pacmanRenderer);
                        break;
                    default:
                        pacmanManager->drawTexture(pacmanTexture, frameClip[0], destRect, pacmanRenderer);
                        break;
                }
                break;
            default:
                switch (directionQueue.front()) {
                    case UP:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame], lastDest, pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame], destRect, pacmanRenderer);
                        break;
                    case RIGHT:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 3], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 3], destRect,
                                                   pacmanRenderer);
                        break;
                    case DOWN:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 6], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 6], destRect,
                                                   pacmanRenderer);
                        break;
                    case LEFT:
                        if (!lastPoint.empty())
                            lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
                        for (int i = 0; i < lastPoint.size(); i++) {
                            lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
                            pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod / (i + 1));
                            pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 9], lastDest,
                                                       pacmanRenderer);
                        }
                        pacmanTexture = pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
                        pacmanManager->drawTexture(pacmanTexture, frameClip[frame + 9], destRect,
                                                   pacmanRenderer);
                        break;
                    default:
                        pacmanManager->drawTexture(pacmanTexture, frameClip[9], destRect, pacmanRenderer);
                        break;
                }

        }
    }
}
void Pacman::setPacmanFrameClip() {
    switch(pacmanType){
        case CLASSIC: case ANDROID:
            for(int i = 0; i < 12; i++){
                frameClip[i].x = i * (6 + OBJECT_PIXEL);
                frameClip[i].y = 0;
                frameClip[i].w = OBJECT_PIXEL;
                frameClip[i].h = OBJECT_PIXEL;
            }
            break;
        case MS:
            for (int i = 0; i < 12; i++) {
                frameClip[i].x = i * (3 + MS_PIXEL);
                frameClip[i].y = 0;
                frameClip[i].w = MS_PIXEL;
                frameClip[i].h = MS_PIXEL;
            }
            break;
    }
}

void Pacman::queueDirection(Direction direction) {
    if (power[FREEZE_PACMAN]) return;
    if (pacmanState == PACMAN_STAND_STATE) {
        if (direction == RIGHT || direction == LEFT){
            directionQueue.push(direction);
        }
        return;
    }
    if (!directionQueue.empty())
        if (directionQueue.front() == -direction){
            while (!directionQueue.empty())
                directionQueue.pop();
        }
    if (directionQueue.size() < 2 || directionQueue.back() != direction) {
        directionQueue.emplace(direction);
        return;
    }
    for (int i = 0; i < directionQueue.size(); i++) {
        std::cerr << directionQueue.front() << " ";
    }
    std::cerr << std::endl;
}

void Pacman::move(Direction _direction, int _velocity){
    Object::move(_direction, _velocity);
}

void Pacman::stop(){
    if (directionQueue.size() > 1){
        if (checkPosition()){
            directionQueue.pop();
        }
    }
}

void Pacman::setState(PACMAN_STATE state) {
    pacmanState = state;
    startState = timer->getTicks();
    switch (pacmanState){
        case PACMAN_START_STATE:
            setPower(NORMAL_PACMAN);
            eatenDot = 0;
            eatenFruit = 0;
            pacmanHealth = PACMAN_MAX_HEALTH;

            objectType = OBJECT_PACMAN;

            velocity = PACMAN_VELOCITY;

            pacmanState = PACMAN_STAND_STATE;
            startState = timer->getTicks();

            lastPoint.clear();

            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
        case PACMAN_NEW_STATE:
            setPower(NORMAL_PACMAN);

            objectType = OBJECT_PACMAN;

            velocity = PACMAN_VELOCITY;

            pacmanState = PACMAN_NEW_STATE;
            startState = timer->getTicks();

            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case PACMAN_STAND_STATE:
            while (!directionQueue.empty())
                directionQueue.pop();
            frame = 0;
            break;
        case PACMAN_RUNNING_STATE : case PACMAN_EATING_STATE : case PACMAN_DEAD_STATE :
            break;
    }

}
void Pacman::handleState() {
    switch (pacmanState){
        case PACMAN_START_STATE:
        case PACMAN_NEW_STATE:
            break;
        case PACMAN_STAND_STATE:
            if (!directionQueue.empty())
                setState(PACMAN_RUNNING_STATE);
            break;
        case PACMAN_RUNNING_STATE:
            if (power[FREEZE_PACMAN])
                break;
            if (!directionQueue.empty())
                move(directionQueue.front(), 0);
            else
                setState(PACMAN_STAND_STATE);
            break;
        case PACMAN_EATING_STATE: {
            Uint32 lastTick = timer->getTicks() - startState;
            if (lastTick > PACMAN_EATING_STATE_TIME) {
                setState(PACMAN_RUNNING_STATE);
            }
            break;
        }
        case PACMAN_DEAD_STATE:
            break;
    }
}

void Pacman::setPower(PACMAN_POWER_STATE type) {
    if (type == NORMAL_PACMAN)
        for (int i = 0; i < PACMAN_POWER_STATE_TOTAL; i++){
            power[i] = false;
            startPower[i] = 0;
        }
    power[type] = true;
    startPower[type] = timer->getTicks();
    switch (type){
        case NORMAL_PACMAN:
            if(pacmanVelocity != PACMAN_VELOCITY)
                if (checkPosition())
                    pacmanVelocity = PACMAN_VELOCITY;
            frameCount = PACMAN_FRAME_VALUE;
            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case POWER_PACMAN:
            if (power[CONFUSED_PACMAN])
                removePower(CONFUSED_PACMAN);
            break;
        case CONFUSED_PACMAN:
            if (power[POWER_PACMAN])
                removePower(POWER_PACMAN);
            break;
        case SPEED_PACMAN:
            if (power[SLOW_DOWN_PACMAN])
                removePower(SLOW_DOWN_PACMAN);
            if(checkPosition())
                pacmanVelocity = PACMAN_VELOCITY * 2;
            frameCount = 2;
            break;
        case SLOW_DOWN_PACMAN:
            if (power[SPEED_PACMAN])
                removePower(SPEED_PACMAN);
            if(checkPosition())
                pacmanVelocity = PACMAN_VELOCITY_SLOW;
            frameCount = PACMAN_SLOW_FRAME_VALUE;
            break;
        case INVISIBLE_PACMAN:
            lastAlphaMod = 0x80;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case BLIND_PACMAN: case FREE_TIME_PACMAN:
            break;
        case FREEZE_PACMAN:
            lastAlphaMod = 128;
            SDL_SetTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case PACMAN_POWER_STATE_TOTAL:
            break;
    }
}

void Pacman::handlePower() {
    bool isPower = false;
    for (int i = 0; i < PACMAN_POWER_STATE_TOTAL; i++)
        if (power[i]){
            Uint32 lastTick = timer->getTicks() - startPower[i];
            if (lastTick > UPGRADE_TIME[i]){
                removePower((PACMAN_POWER_STATE)i);
                continue;
            }
            else {
                switch ((PACMAN_POWER_STATE)i){
                    case POWER_PACMAN: case CONFUSED_PACMAN: case INVISIBLE_PACMAN: case BLIND_PACMAN: case FREEZE_PACMAN: case FREE_TIME_PACMAN:
                        break;
                    case SPEED_PACMAN:
                        if (checkPosition())
                            pacmanVelocity = PACMAN_VELOCITY * 2;
                        break;
                    case SLOW_DOWN_PACMAN:
                        if (checkPosition())
                            pacmanVelocity = PACMAN_VELOCITY_SLOW;
                        break;
                }
            }
            isPower = true;
        }
    if (!isPower)
        if(pacmanVelocity != PACMAN_VELOCITY)
            if(checkPosition()){
                pacmanVelocity = PACMAN_VELOCITY;
                frameCount = PACMAN_FRAME_VALUE;
            }
}

void Pacman::removePower(PACMAN_POWER_STATE type) {
    power[type] = false;
    startPower[type] = 0;
    switch (type){
        case POWER_PACMAN:
            objectType = OBJECT_PACMAN;
            break;
        case SPEED_PACMAN:
            lastPoint.clear();
            if (pacmanVelocity != PACMAN_VELOCITY)
                if (checkPosition())
                    pacmanVelocity = PACMAN_VELOCITY;
            frameCount = PACMAN_FRAME_VALUE;
            break;
        case SLOW_DOWN_PACMAN:
            if(pacmanVelocity != PACMAN_VELOCITY)
                if (checkPosition())
                    pacmanVelocity = PACMAN_VELOCITY;
            frameCount = PACMAN_FRAME_VALUE;
            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case INVISIBLE_PACMAN:
            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
        case BLIND_PACMAN: case FREE_TIME_PACMAN:
            break;
        case FREEZE_PACMAN:
            lastAlphaMod = 0xFF;
            pacmanManager->setTextureAlphaMod(pacmanTexture, lastAlphaMod);
            break;
    }
}

Pacman::~Pacman() = default;
