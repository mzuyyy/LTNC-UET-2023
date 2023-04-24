//
// Created by MZuy on 3/28/2023.
//

#include "Ghost.h"

Ghost::Ghost(GHOST_TYPE type, SDL_Renderer *renderer) {
    consoleGhost = new Log("Ghost");
    objectRenderer = renderer;
    ghostType = type;
    frameCount = 2;
    Ghost::setGhostFrameClip();
    switch (type){
        case BLINKY:
            objectType = OBJECT_BLINKY;
            ghostTexture = objectManager->loadTexture("../Assets/ghost/Blinky.png", objectRenderer);
            break;
        case PINKY:
            objectType = OBJECT_PINKY;
            ghostTexture = objectManager->loadTexture("../Assets/ghost/Pinky.png", objectRenderer);
            break;
        case INKY:
            objectType = OBJECT_INKY;
            ghostTexture = objectManager->loadTexture("../Assets/ghost/Inky.png", objectRenderer);
            break;
        case CLYDE:
            objectType = OBJECT_CLYDE;
            ghostTexture = objectManager->loadTexture("../Assets/ghost/Clyde.png", objectRenderer);
            break;
        default:
            break;
    }
    tileID.x = startGhostTileID[ghostType].x;
    tileID.y = startGhostTileID[ghostType].y;
    destRect = {tileID.x * 24 + 3, tileID.y * 24 + 144 - 9, OBJECT_SIZE, OBJECT_SIZE};
}
void Ghost::setGhostFrameClip() {
    for (int i = 0; i < 11; i++){
        ghostFrameClip[i].x = i * (OBJECT_PIXEL + 6);
        ghostFrameClip[i].y = 0;
        ghostFrameClip[i].w = OBJECT_PIXEL;
        ghostFrameClip[i].h = OBJECT_PIXEL;
    }
}
void Ghost::render(){
    switch (directionQueue.front()){
        case UP:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[frame], destRect, objectRenderer);
            break;
        case RIGHT:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[frame + 2], destRect, objectRenderer);
            break;
        case DOWN:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[frame + 4], destRect, objectRenderer);
            break;
        case LEFT:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[frame + 6], destRect, objectRenderer);
            break;
        default:
            ghostManager->drawTexture(ghostTexture,ghostFrameClip[ghostType], destRect, objectRenderer);
            break;
    }
}
void Ghost::update() {
    frame = (SDL_GetTicks() / ANIMATION_SPEED) % frameCount;


}
void Ghost::queueDirection(Direction dir) {
}

void Ghost::move(Direction _direction, int _velocity) {
    Object::move(_direction, _velocity);
}

void Ghost::stop() {
    if(directionQueue.size() > 1)
        if(checkPosition())
            directionQueue.pop();
}

void Ghost::setState(GHOST_STATE ghostState) {

}

void Ghost::handleState() {

}
