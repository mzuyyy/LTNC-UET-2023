//
// Created by MZuy on 3/3/2023.
//

#include "Object.h"

Object::Object(const std::string &textureSheet, SDL_Renderer *renderer, OBJECT_TYPE type, Timer *_timer) {
    this->objectRenderer = renderer;
    timer = _timer;
    this->objectType = type;
    objectTexture = objectManager->loadTexture(textureSheet, objectRenderer);
    tileID.x = startTileID.x;
    tileID.y = startTileID.y;
    position.x = tileID.x * objectWidth;
    position.y = tileID.y * objectHeight;
}
void Object::update() {
    position = {tileID.x * 24, tileID.y * 24};
    destRect = {position.x - 12, position.y - 12, OBJECT_PIXEL, OBJECT_PIXEL};
}
void Object::render() {
    SDL_RenderCopy(objectRenderer, objectTexture, &sourceRect, &destRect);
}
Object::~Object() {
    SDL_DestroyTexture(objectTexture);
}

TileID Object::getTileID() {
    return tileID;
}

Position Object::getPosition() {
    return position;
}

void Object::move(Position _velocity) {
    position.x += _velocity.x;
    position.y += _velocity.y;
    destRect = {position.x - 12, position.y - 12, OBJECT_PIXEL, OBJECT_PIXEL};
    if (checkPosition()){
        tileID = {position.x / 24, position.y / 24};
    }
}

bool Object::checkPosition() const {
    return (position.x % 24 == 0 && position.y % 24 == 0);
}
bool Object::checkCollision(Object *object) {
    return (position.x + OBJECT_PIXEL - 36 <= object->getPosition().x) ||
           (position.y >= object->getPosition().y + OBJECT_PIXEL - 36) ||
           (position.x >= object->getPosition().x + OBJECT_PIXEL - 26)||
           (position.y  + OBJECT_PIXEL - 36 <= object->getPosition().y);
}
void Object::checkTunnel() {
    if (checkPosition()){
        if (tileID.x == 28)
            tileID = {0, tileID.y};
        if (tileID.x == -1)
            tileID = {27, tileID.y};
        if (tileID.y == 31)
            tileID = {tileID.x, 0};
        if (tileID.y == -1)
            tileID = {tileID.x, 30};
    }
}

void Object::setTileID(TileID tileID) {
    this->tileID = tileID;
    update();
}

void Object::setPosition(Position position) {
    this->position = position;
    destRect = {position.x - 12, position.y - 12, OBJECT_PIXEL, OBJECT_PIXEL};
    tileID = {position.x / 24, position.y / 24};
}
