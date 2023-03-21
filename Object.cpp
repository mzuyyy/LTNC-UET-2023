//
// Created by MZuy on 3/3/2023.
//

#include "Object.h"
#include "textureManager.h"

void Object::update() {
    if (isMovingLeft) xPosition--;
    else if(isMovingRight) xPosition++;
    if (isMovingUp) yPosition--;
    else if(isMovingDown) yPosition++;

    sourceRect.h = 21;
    sourceRect.w = 21;
    sourceRect.x = 0;
    sourceRect.y = 0;

    destinationRect.x = xPosition;
    destinationRect.y = yPosition;
    destinationRect.w = sourceRect.w * 2;
    destinationRect.h = sourceRect.h * 2;
}
void Object::render() {
    SDL_RenderCopy(renderer, objectTexture, &sourceRect, &destinationRect);
}

Object::Object(const char *textureSheet, int x, int y, SDL_Renderer *renderer) {
    this->renderer = renderer;
    objectTexture = objectManager->LoadTexture(textureSheet, renderer);
    xPosition = x;
    yPosition = y;
}

Object::~Object() {
    SDL_DestroyTexture(objectTexture);
}
