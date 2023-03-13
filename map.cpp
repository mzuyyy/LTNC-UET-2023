//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include "logStatus.h"
#include "textureManager.h"

Log* consoleMap = new Log("Map");
int Map::tile[MAP_HEIGHT][MAP_WIDTH];

//write function to read map.txt
Map::Map() {
    dot = textureManager::LoadTexture("../Assets/dot.png", renderer);
    pacman = textureManager::LoadTexture("../Assets/pacman.png", renderer);
    inky = textureManager::LoadTexture("../Assets/inky.png", renderer);
    blinky = textureManager::LoadTexture("../Assets/blinky icon.png", renderer);
    pinky = textureManager::LoadTexture("../Assets/pinky icon.png", renderer);
    clyde = textureManager::LoadTexture("../Assets/clyde icon.png", renderer);
    wall = textureManager::LoadTexture("../Assets/wall.png", renderer);
    pellet = textureManager::LoadTexture("../Assets/pellet.png", renderer);
    powerPellet = textureManager::LoadTexture("../Assets/powerPellet.png", renderer);
    fruit = textureManager::LoadTexture("../Assets/fruit.png", renderer);

    consoleMap->updateStatus("Map is created");
    std::ifstream mapFile(mapPath);
    if (mapFile.is_open()) {
        for (auto & i : tile) {
            for (int & j : i) {
                mapFile >> j;
            }
        }
    }
    else {
        consoleMap->updateStatus("Map file is not found");
    }
    mapFile.close();
}
Map::~Map() {
    consoleMap->updateStatus("Map is destroyed");
}
int Map::getTileID(int x, int y) {
    return tile[y][x];
}

void Map::renderMap(SDL_Renderer *renderer) {

}

void Map::setMapFrameClip() {
    if(mapWidthFrame > 0 && mapHeightFrame > 0){
        for(int i = 0; i < 39; i++){
            mapFrameClip[i].x = i * (mapWidthFrame + 1);
            mapFrameClip[i].y = 0;
            mapFrameClip[i].w = mapWidthFrame;
            mapFrameClip[i].h = mapHeightFrame;
        }
    }
}
