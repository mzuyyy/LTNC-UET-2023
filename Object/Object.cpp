//
// Created by MZuy on 3/3/2023.
//

#include "Object.h"

Object::Object(const std::string &textureSheet, SDL_Renderer *renderer, SDL_Point initPosition) {
    this->objectRenderer = renderer;
    objectTexture = objectManager->loadTexture(textureSheet, objectRenderer);
    tileID.x = initPosition.x;
    tileID.y = initPosition.y;
    position.x = tileID.x * objectWidth;
    position.y = tileID.y * objectHeight;
}
void Object::update() {
    Object::move();

    sourceRect.h = objectHeight;
    sourceRect.w = objectWidth;
    sourceRect.x = 0;
    sourceRect.y = 0;

    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = sourceRect.w * 2;
    destRect.h = sourceRect.h * 2;
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

void Object::move() {
    switch (directionQueue.front()) {
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
        default:
            break;
    }
    if ((position.x + 8) % 16 == 0 && (position.y + 8) % 16 == 0) {
        tileID.x = (position.x + 8) / 16;
        tileID.y = (position.y + 8) / 16;
    }
}

bool Object::checkPosition() {
    return (position.x % 16 == 0 && position.y % 16 == 0);
}
