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
    }
    destRect = {position.x - 12, position.y - 12, OBJECT_SIZE, OBJECT_SIZE};
    if (checkPosition()){
        tileID = {position.x / 24, position.y / 24};
    }
}

bool Object::checkPosition() const{
    int x = position.x + OBJECT_SIZE / 2;
    int y = position.y - 144 + OBJECT_SIZE / 2;
    std::cerr << "x = " << x << ", y =  " << y << std::endl;
    return (x % 24 == 12) && (y % 24 == 12);
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
