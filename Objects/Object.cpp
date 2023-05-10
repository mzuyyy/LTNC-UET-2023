//
// Created by MZuy on 3/3/2023.
//

#include "Object.h"

Object::Object(SDL_Renderer *renderer, OBJECT_TYPE type, Timer *_timer) {
    this->objectRenderer = renderer;
    timer = _timer;
    this->objectType = type;
    tileID.x = startTileID.x;
    tileID.y = startTileID.y;
    position.x = tileID.x * objectWidth;
    position.y = tileID.y * objectHeight;
}
void Object::update() {
    position = {tileID.x * 24 + 3, 144 + tileID.y * 24 - 9};
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};
}

TileID Object::getTileID() {
    return tileID;
}

Position Object::getPosition() {
    return position;
}

void Object::move(Direction direction, int _velocity) {
    switch (direction) {
        case UP:
            position.y -= _velocity;
            break;
        case DOWN:
            position.y += _velocity;
            break;
        case LEFT:
            position.x -= _velocity;
            break;
        case RIGHT:
            position.x += _velocity;
            break;
        case NONE:
            break;
    }
    destRect = {position.x, position.y, OBJECT_SIZE, OBJECT_SIZE};
    if (checkPosition()){
        tileID = {position.x / 24, position.y / 24};
    }
}

bool Object::checkPosition() const{
    int x = position.x + OBJECT_SIZE / 2;
    int y = position.y - 144 + OBJECT_SIZE / 2;
    return (x % 24 == 12) && (y % 24 == 12);
}

void Object::setTileID(TileID _tileID) {
    this->tileID = _tileID;
}

void Object::setPosition(Position _position) {
    this->position = _position;
    destRect = {_position.x - 3, _position.y + 9, OBJECT_SIZE, OBJECT_SIZE};
    tileID = {(_position.x  + 3) / 24, (_position.y  - 9) / 24};
}
Object::~Object() {
    SDL_DestroyTexture(objectTexture);
}

void Object::speedAnimation() {
    if (!lastPoint.empty())
        lastDest = {0, 0, OBJECT_SIZE, OBJECT_SIZE};
    for (int i = 0; i < lastPoint.size(); i++) {
        lastDest = {lastPoint[i].x, lastPoint[i].y, OBJECT_SIZE, OBJECT_SIZE};
        textureManager::setTextureAlphaMod(objectTexture, lastAlphaMod / (i + 1));
        objectManager->drawTexture(objectTexture, sourceRect, lastDest, objectRenderer);
    }
    textureManager::setTextureAlphaMod(objectTexture, lastAlphaMod);
}
