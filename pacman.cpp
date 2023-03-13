//
// Created by MZuy on 3/12/2023.
//

#include "pacman.h"
#include "Object.h"
#include "textureManager.h"

Pacman::Pacman(const char* textureSheet, int x, int y, SDL_Renderer *renderer) : Object(textureSheet, x, y, renderer) {
    this->renderer = renderer;
    objectTexture = textureManager::LoadTexture(textureSheet, renderer);
    xPosition = x;
    yPosition = y;
}

void Pacman::update() {
    if (isMovingLeft) xPosition--;
    else if(isMovingRight) xPosition++;
    if (isMovingUp) yPosition--;
    else if(isMovingDown) yPosition++;

    destinationRect.x = xPosition;
    destinationRect.y = yPosition;
    destinationRect.w = widthFrame ;
    destinationRect.h = heightFrame ;
}

void Pacman::render() {
    if(isMovingUp){
        SDL_RenderCopy(renderer, objectTexture, &frameClip[1], &destinationRect);
    }
    else if(isMovingDown){
        SDL_RenderCopy(renderer, objectTexture, &frameClip[4], &destinationRect);
    }
    else if(isMovingLeft){
        SDL_RenderCopy(renderer, objectTexture, &frameClip[7], &destinationRect);
    }
    else if(isMovingRight){
        SDL_RenderCopy(renderer, objectTexture, &frameClip[10], &destinationRect);
    }
}
void Pacman::updateClip() {
    if(widthFrame > 0 && heightFrame > 0){
        for(int i = 0; i < 12; i++){
            frameClip[i].x = 5 * i + widthFrame * i;
            frameClip[i].y = 0;
            frameClip[i].w = widthFrame;
            frameClip[i].h = heightFrame;
        }
    }
}
Pacman::~Pacman() = default;
