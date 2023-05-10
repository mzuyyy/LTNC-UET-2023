//
// Created by MZuy on 4/27/2023.
//

#include "Mystery.h"

Mystery::Mystery(SDL_Renderer *_renderer, Timer *_timer) {
    consoleMystery = new Log("Mystery");

    mysteryManager = new textureManager();

    velocity = MYSTERY_VELOCITY;

    objectRenderer = _renderer;

    timer = _timer;

    mysteryTexture = mysteryManager->loadTexture(MYSTERY_PATH, objectRenderer);
    speechTexture = mysteryManager->loadTexture(SPEECH_PATH, objectRenderer);

    objectType = OBJECT_MYSTERY;

    position = START_MYSTERY_POSITION;

    tileID = {position.x / 24, position.y / 24};

    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    setFrameClip();

    setState(MYSTERY_INIT);
}
void Mystery::setFrameClip() {
    for (int i = 0; i < MYSTERY_FRAME; i++) {
        frameClip[i] = {i * (OBJECT_PIXEL + 6), 0 , OBJECT_PIXEL, OBJECT_PIXEL};
    }
    for (int i = 0; i < 4; i++) {
        speechClip[i] = {SPEECH_WIDTH * i, 0, SPEECH_WIDTH, SPEECH_HEIGHT};
    }
}
void Mystery::update() {
    frame = (timer->getTicks() / ANIMATION_SPEED) % MYSTERY_FRAME;

    handleState();

    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

    tileID.x = (position.x - 3) % 24 >= 12 ? (position.x - 3) / 24 + 1 : (position.x - 3) / 24;
    tileID.y = (position.y + 9 - 144) % 24 >= 12 ? (position.y + 9 - 144) / 24 + 1 : (position.y + 9 - 144) / 24;
}

void Mystery::render() {
    mysteryManager->drawTexture(mysteryTexture, frameClip[frame], destRect, objectRenderer);
    if (currentState == MYSTERY_SPEECH){
        mysteryManager->drawTexture(speechTexture, speechClip[currentSpeech]
                                    , {SPEECH_POSITION.x, SPEECH_POSITION.y, SPEECH_WIDTH, SPEECH_HEIGHT}, objectRenderer);
    }
}

void Mystery::initState() {
    switch (currentState){
        case MYSTERY_INIT:
            currentDirection = NONE;

            velocity = MYSTERY_VELOCITY;
            frameCount = MYSTERY_FRAME;

            setState(MYSTERY_STAND);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case MYSTERY_STAND:
            currentDirection = NONE;

            setPosition(MYSTERY_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case MYSTERY_MOVING:
            currentDirection = NONE;

            velocity = MYSTERY_VELOCITY;

            setTarget(MYSTERY_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case MYSTERY_WAITING:
            currentDirection = NONE;

            velocity = MYSTERY_VELOCITY;

            setTarget(MYSTERY_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case MYSTERY_SPEECH:
            currentDirection = NONE;

            currentSpeech++;

            setPosition(MYSTERY_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        case MYSTERY_RETURNING:
            currentDirection = NONE;

            velocity = MYSTERY_VELOCITY;

            setTarget(START_MYSTERY_POSITION);

            position.x = tileID.x * 24 + 3;
            position.y = 24 * 6 + tileID.y * 24 - 9;

            destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};

            break;
        default:
            break;
    }
}
void Mystery::setState(MYSTERY_STATE _state) {
    currentState = _state;
    startState = timer->getTicks();
    initState();
}
void Mystery::handleState() {
    switch(currentState){
        case MYSTERY_INIT: case MYSTERY_STAND:
            break;
        case MYSTERY_MOVING:
            if (currentDirection != NONE)
                move();
            if (position == MYSTERY_POSITION)
                setState(MYSTERY_WAITING);
            break;
        case MYSTERY_RETURNING:
            if (currentDirection != NONE)
                move();
            if (position == START_MYSTERY_POSITION)
                setState(MYSTERY_STAND);
        case MYSTERY_WAITING: {
            Uint32 tick = timer->getTicks() - startState;
            if (tick > MYSTERY_STATE_TIME[MYSTERY_WAITING])
                setState(MYSTERY_RETURNING);
        }
        default:
            break;
    }
}
void Mystery::move() {
    if (currentDirection != NONE)
        Object::move(currentDirection, velocity);
    else
        setState(MYSTERY_WAITING);
}
Mystery::~Mystery() {
    delete consoleMystery;
    delete mysteryManager;
    mysteryManager = nullptr;
    consoleMystery = nullptr;
}