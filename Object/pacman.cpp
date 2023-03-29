//
// Created by MZuy on 3/12/2023.
//

#include "pacman.h"
#include "Object.h"

Pacman::Pacman(const std::string &textureSheet, SDL_Renderer *renderer, PACMAN_TYPE type) : Object(textureSheet, renderer, SDL_Point()) {
    this->pacmanRenderer = renderer;
    pacmanTexture = pacmanManager->loadTexture(textureSheet, renderer);

    tileID.x= startTilePosition.x;
    tileID.y = startTilePosition.y;

    position.x = tileID.x * 24 + 8;
    position.y = 72 + tileID.y * 24;

    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;

    frameCount = 5;
    animationSpeed = 100;

    this->type = type;

    Pacman::setPacmanFrameClip();
}

void Pacman::update(){
    frame = (SDL_GetTicks() / animationSpeed) % frameCount;

    if (Pacman::isDead && pacmanHealth > 0) {
        Pacman::resetPosition();
        return;
    }
    if (tileID.x == 1 || tileID.x == 26) {
        if(tileID.y == 14)
            tileID.x = tileID.x <= 0 ? 27 : 0;
        else Pacman::stop();
    }
    if (!Pacman::CanMove || directionQueue.empty()) Pacman::stop();
    else if (directionQueue.front() == LEFT) {
        position.x -= pacmanVelocity;
    } else if (directionQueue.front() == RIGHT) {
        position.x += pacmanVelocity;
    } else if (directionQueue.front() == UP) {
        position.y -= pacmanVelocity;
    } else if (directionQueue.front() == DOWN) {
        position.y += pacmanVelocity;
    }
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = pacmanWidth;
    destRect.h = pacmanHeight;

    tileID.x = (position.x) / 24;
    tileID.y = (position.y - 72) / 24;
}
void Pacman::render(){
    if (frame >= 3) frame = abs(frame - 4);

    if(directionQueue.front() == UP){
        pacmanManager->drawTexture(pacmanTexture, frameClip[frame], destRect, pacmanRenderer);
    }
    else if (directionQueue.front() == RIGHT){
        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? frame : frame + 2],
                                   destRect, pacmanRenderer);
    }
    else if (directionQueue.front() == DOWN){
        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? frame : frame + 4],
                                   destRect, pacmanRenderer);
    }
    else if (directionQueue.front() == LEFT){
        pacmanManager->drawTexture(pacmanTexture, frameClip[frame == 0 ? frame : frame + 6],
                                   destRect, pacmanRenderer);
    }
    else{
        pacmanManager->drawTexture(pacmanTexture, frameClip[0], destRect, pacmanRenderer);
    }
}
void Pacman::setPacmanFrameClip() {
    for(int i = 0; i < 9; i++){
        frameClip[i].x = i * (6 + widthFrame);
        frameClip[i].y = 0;
        frameClip[i].w = widthFrame;
        frameClip[i].h = heightFrame;
    }
}

void Pacman::queueDirection(Direction direction) {
    if (direction == directionQueue.front()) {
        std::cerr << "Direction = " << directionQueue.front() << " " << directionQueue.back() << std::endl;
        return;
    }
    else if (!directionQueue.empty())
        if (directionQueue.front() == -direction){
            std::queue<Direction> empty;
            std::swap(directionQueue, empty);
        }
    if(directionQueue.size() < 2)
        directionQueue.push(direction);
    std::cerr << "Direction = " << directionQueue.front() << " " << directionQueue.back() << std::endl;
}

void Pacman::move() {
    Object::move();
}

void Pacman::stop(){
    if (directionQueue.size() > 1){
        if (checkPosition()){
            directionQueue.pop();
        }
    }
    Pacman::CanMove = true;
}

void Pacman::setState(PACMAN_STATE state) {
    pacmanState = state;
}

Pacman::~Pacman() = default;
