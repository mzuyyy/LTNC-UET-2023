//
// Created by MZuy on 3/12/2023.
//

#include "pacman.h"
#include "Object.h"
#include "textureManager.h"

Pacman::Pacman(const std::string& textureSheet, SDL_Renderer *renderer) : Object(textureSheet, renderer) {
    this->pacmanRenderer = renderer;
    pacmanTexture = pacmanManager->loadTexture(textureSheet, renderer);

    pacmanPosition.x= defaultPosition.x;
    pacmanPosition.y = defaultPosition.y;

    destRect.x = pacmanPosition.x;
    destRect.y = pacmanPosition.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;

    pacmanFrameCount = 3;
    pacmanAnimationSpeed = 100;

    Pacman::setPacmanFrameClip();
}

void Pacman::update(){
    //Pacman::deleteOldFrame();
    pacmanFrame = (SDL_GetTicks() / pacmanAnimationSpeed) % pacmanFrameCount;
    if (pacmanDir["left"]){
        pacmanPosition.x -= pacmanVelocity;
    }
    else if (pacmanDir["right"]){
        pacmanPosition.x += pacmanVelocity;
    }
    else if (pacmanDir["up"] && pacmanPosition.y > 8) {
        pacmanPosition.y -= pacmanVelocity;
    }
    else if (pacmanDir["down"] && pacmanPosition.y < 472) {
        pacmanPosition.y += pacmanVelocity;
    }
    if (pacmanPosition.x <= 217 || pacmanPosition.x >= 643) {
        pacmanPosition.x = pacmanPosition.x <= 217 ? 643 : 217;
    }
    if (pacmanPosition.y <= 16 || pacmanPosition.y >= 471)
        pacmanPosition.y += pacmanPosition.y <= 0 ? 1 : -1;

    destRect.x = pacmanPosition.x;
    destRect.y = pacmanPosition.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;
}

void Pacman::deleteOldFrame() {
    pacmanTexture = pacmanManager->loadTexture(deletePacmanTexture, pacmanRenderer);
    pacmanManager->drawTexture(pacmanTexture, frameClip[12], destRect, pacmanRenderer);
    pacmanTexture = pacmanManager->loadTexture(pacmanTextureSheet, pacmanRenderer);
}
void Pacman::render(){
    if(pacmanDir["up"]) {
        pacmanManager->drawTexture(pacmanTexture, frameClip[pacmanFrame], destRect, pacmanRenderer);
    }
    else if (pacmanDir["down"]){
        pacmanManager->drawTexture(pacmanTexture, frameClip[pacmanFrame + 3], destRect, pacmanRenderer);
    }
    else if (pacmanDir["left"]){
        pacmanManager->drawTexture(pacmanTexture, frameClip[pacmanFrame + 6], destRect, pacmanRenderer);
    }
    else if (pacmanDir["right"]){
        pacmanManager->drawTexture(pacmanTexture, frameClip[pacmanFrame + 9], destRect, pacmanRenderer);
    }
    else{
        pacmanManager->drawTexture(pacmanTexture, frameClip[0], destRect, pacmanRenderer);
    }
}
void Pacman::setPacmanFrameClip() {
    for(int i = 0; i < 12; i++){
        frameClip[i].x = 5 * i + widthFrame * i;
        frameClip[i].y = 0;
        frameClip[i].w = widthFrame;
        frameClip[i].h = heightFrame;
    }
    frameClip[12] = {7 * 17, 2 * 17, 26, 26};
}

void Pacman::changeDirection(const std::string& direction) {
    if (direction == "up") {
        //if ((lenOfLine[direction] == 8 && !pacmanDir.isMovingUp) || pacmanDir.isMovingUp) {
        pacmanDir["up"] = true;
        pacmanDir["down"] = false;
        pacmanDir["left"] = false;
        pacmanDir["right"] = false;
        //}
    }
    else if (direction == "down") {
        //if((lenOfLine[direction] == 8 && !pacmanDir.isMovingDown) || pacmanDir.isMovingDown) {
        pacmanDir["up"] = false;
        pacmanDir["down"] = true;
        pacmanDir["left"] = false;
        pacmanDir["right"] = false;
        //}
    }
    else if (direction == "left") {
        //if((lenOfLine[direction] == 8 && !pacmanDir.isMovingLeft) || pacmanDir.isMovingLeft) {
        pacmanDir["up"] = false;
        pacmanDir["down"] = false;
        pacmanDir["left"] = true;
        pacmanDir["right"] = false;
        //}
    }
    else if (direction == "right") {
        //if ((lenOfLine[direction] == 8 && !pacmanDir.["right"]) || pacmanDir.["right"]) {
        pacmanDir["up"] = false;
        pacmanDir["down"] = false;
        pacmanDir["left"] = false;
        pacmanDir["right"] = true;
        //}
    }
}
void Pacman::clean() {
    pacmanManager->clean(pacmanTexture);
}

Pacman::~Pacman() = default;
