//
// Created by MZuy on 3/3/2023.
//

#include "Object.h"
#include "textureManager.h"

void Object::update() {
    if (objectDir["left"]) objectPosition.x--;
    else if(objectDir["right"]) objectPosition.x++;
    if (objectDir["up"]) objectPosition.y--;
    else if(objectDir["down"]) objectPosition.y++;

    sourceRect.h = 21;
    sourceRect.w = 21;
    sourceRect.x = 0;
    sourceRect.y = 0;

    destRect.x = objectPosition.x;
    destRect.y = objectPosition.y;
    destRect.w = sourceRect.w * 2;
    destRect.h = sourceRect.h * 2;
}
void Object::render() {
    SDL_RenderCopy(pacmanRenderer, objectTexture, &sourceRect, &destRect);
}

Object::Object(const std::string &textureSheet, SDL_Renderer *renderer){
    this->pacmanRenderer = renderer;
    objectTexture = objectManager->loadTexture(textureSheet, pacmanRenderer);
    objectPosition.x = defaultPosition.x;
    objectPosition.y = defaultPosition.y;
}

Object::~Object() {
    SDL_DestroyTexture(objectTexture);
}

SDL_Point Object::getNextPosition() {
    SDL_Point nextPosition = objectPosition;
    if (objectDir["left"]) nextPosition.x--;
    else if(objectDir["right"]) nextPosition.x++;
    if (objectDir["down"]) nextPosition.y--;
    else if(objectDir["down"]) nextPosition.y++;

    return nextPosition;
}
