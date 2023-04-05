//
// Created by MZuy on 3/12/2023.
//

#include "pacman.h"
#include "Object.h"

Pacman::Pacman(SDL_Renderer *renderer, PACMAN_TYPE type) {
    this->pacmanRenderer = renderer;

    classicPacmanTexture = pacmanManager->loadTexture(CLASSIC_PACMAN_TEXTURE_SHEET, renderer);
    pacmanDeadTexture = pacmanManager->loadTexture(DEAD_PACMAN_TEXTURE_SHEET, renderer);
    androidPacmanTexture = pacmanManager->loadTexture(ANDROID_PACMAN_TEXTURE_SHEET, renderer);

    tileID.x= startTileID.x;
    tileID.y = startTileID.y;

    position.x = tileID.x * 24 + 8;
    position.y = 72 + tileID.y * 24;

    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;

    pacmanType = type;

    pacmanState = pacmanStartState;

    Pacman::setPacmanFrameClip();
}

void Pacman::update(){
    frame = (SDL_GetTicks() / PACMAN_ANIMATION_SPEED) % PACMAN_FRAME_VALUE;

    if (Pacman::isDead && pacmanHealth > 0) {
        Pacman::resetPosition();
        return;
    }
    handleState();
    checkTunnel();
    if (!Pacman::CanMove || directionQueue.empty()) Pacman::stop();
    else if (directionQueue.front() == LEFT) {
        position.x -= PACMAN_VELOCITY;
    } else if (directionQueue.front() == RIGHT) {
        position.x += PACMAN_VELOCITY;
    } else if (directionQueue.front() == UP) {
        position.y -= PACMAN_VELOCITY;
    } else if (directionQueue.front() == DOWN) {
        position.y += PACMAN_VELOCITY;
    }
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;

    tileID.x = (position.x) / 24;
    tileID.y = (position.y - 72) / 24;
}
void Pacman::render(){
    if (frame >= 3) frame = abs(frame - 4);
    if (pacmanType == CLASSIC){
        if(directionQueue.front() == UP){
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[frame], destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == RIGHT){
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[frame == 0 ? frame : frame + 2],
                                       destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == DOWN){
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[frame == 0 ? frame : frame + 4],
                                       destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == LEFT){
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[frame == 0 ? frame : frame + 6],
                                       destRect, pacmanRenderer);
        }
        else{
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[0], destRect, pacmanRenderer);
        }
    }
    else {
        if(directionQueue.front() == UP){
            pacmanManager->drawTexture(androidPacmanTexture, frameClip[frame], destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == RIGHT){
            pacmanManager->drawTexture(androidPacmanTexture, frameClip[frame + 2],
                                       destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == DOWN){
            pacmanManager->drawTexture(androidPacmanTexture, frameClip[frame + 4],
                                       destRect, pacmanRenderer);
        }
        else if (directionQueue.front() == LEFT){
            pacmanManager->drawTexture(androidPacmanTexture, frameClip[frame + 6],
                                       destRect, pacmanRenderer);
        }
        else{
            pacmanManager->drawTexture(classicPacmanTexture, frameClip[0], destRect, pacmanRenderer);
        }
    }
}
void Pacman::setPacmanFrameClip() {
    for(int i = 0; i < 12; i++){
        frameClip[i].x = i * (6 + widthFrame);
        frameClip[i].y = 0;
        frameClip[i].w = widthFrame;
        frameClip[i].h = heightFrame;
    }
}

void Pacman::queueDirection(Direction direction) {
    if (power[FREEZE_PACMAN]) return;
    if (pacmanState == PACMAN_STAND_STATE) {
        if (direction == RIGHT || direction == LEFT)
            directionQueue.push(direction);
        return;
    }
    if (!directionQueue.empty())
        if (directionQueue.front() == direction){
            while (!directionQueue.empty())
                directionQueue.pop();
        }
    if (directionQueue.size() < 2) {
        directionQueue.push(direction);
        return;
    }
}

void Pacman::move(Position velocity) {
    Object::move(Position());
}

void Pacman::stop(){
    if (directionQueue.size() > 1){
        if (checkPosition() || Pacman::CanMove){
            directionQueue.pop();
        }
    }
    Pacman::CanMove = true;
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
            if (pacmanType == ANDROID)
                objectType = OBJECT_PACMAN_ANDROID;

            velocity = PACMAN_VELOCITY;

            setState(PACMAN_STAND_STATE);

            lastPoint.clear();

            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
        case PACMAN_NEW_STATE:
            setPower(NORMAL_PACMAN);

            objectType = OBJECT_PACMAN;

            if (pacmanType == ANDROID)
                objectType = OBJECT_PACMAN_ANDROID;

            velocity = PACMAN_VELOCITY;

            setState(PACMAN_STAND_STATE);

            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
            break;
        case PACMAN_STAND_STATE:
            while (!directionQueue.empty())
                directionQueue.pop();

            frame = 0;


        case PACMAN_EATING_STATE:
            break;
        case PACMAN_DEATH_STATE:
            break;
        case PACMAN_RUNNING_STATE:
            break;
        case PACMAN_STATE_TOTAL:
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
                switch (directionQueue.front()){
                case UP:
                    move(Position(0, -velocity));
                    break;
                case DOWN:
                    move(Position(0, velocity));
                    break;
                case LEFT:
                    move(Position(-velocity, 0));
                    break;
                case RIGHT:
                    move(Position(velocity, 0));
                    break;
                case NONE:
                    move(Position(0,0));
                }
            break;
        case PACMAN_EATING_STATE: {
            int lastTick = timer->getTicks() - startState;
            if (lastTick > PACMAN_EATING_STATE_TIME) {
                setState(PACMAN_RUNNING_STATE);
            }
            break;
        }
        case PACMAN_DEATH_STATE:
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
            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
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
            SDL_SetTextureAlphaMod(pacmanTexture, 75);
            break;
        case BLIND_PACMAN:
            break;
        case FREE_TIME_PACMAN:
            break;
        case FREEZE_PACMAN:
            SDL_SetTextureAlphaMod(pacmanTexture, 128);
            break;
        case PACMAN_POWER_STATE_TOTAL:
            break;
    }
}

void Pacman::handlePower() {
    bool isPower = false;
    for (int i = 0; i < PACMAN_POWER_STATE_TOTAL; i++)
        if (power[i]){
            int lastTick = timer->getTicks() - startPower[i];
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
            if (pacmanType == ANDROID)
                objectType = OBJECT_PACMAN_ANDROID;
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
            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
            break;
        case INVISIBLE_PACMAN:
            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
            break;
        case BLIND_PACMAN:
            break;
        case FREE_TIME_PACMAN:
            break;
        case FREEZE_PACMAN:
            SDL_SetTextureAlphaMod(pacmanTexture, 0xFF);
            break;
    }
}

Pacman::~Pacman() = default;
