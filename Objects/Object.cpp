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
    destRect = {position.x - 12, position.y - 12, OBJECT_SIZE, OBJECT_SIZE};
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
    destRect = {position.x - 12, position.y - 12, OBJECT_SIZE, OBJECT_SIZE};
    if (checkPosition()){
        tileID = {position.x / 21, position.y / 21};
    }
}

bool Object::checkPosition() const{
    std::cerr << "x = " << position.x + 21 << ", y =  " << position.y  - 72 + 21 << std::endl;
    return (position.x + 21 % 12 == 0 && position.y - 72 + 21 % 12 == 0);
}

bool Object::checkCollision(Object *object) const {
    return (position.x + OBJECT_SIZE - 31 <= object->getPosition().x) ||
           (position.y >= object->getPosition().y + OBJECT_SIZE - 31) ||
           (position.x >= object->getPosition().x + OBJECT_SIZE - 31) ||
           (position.y + OBJECT_SIZE - 31 <= object->getPosition().y);
}

void Object::setTileID(TileID tileID) {
    this->tileID = tileID;
    update();
}

void Object::setPosition(Position position) {
    this->position = position;
    destRect = {position.x - 12, position.y - 12, OBJECT_SIZE, OBJECT_SIZE};
    tileID = {position.x / 24, position.y / 24};
}
