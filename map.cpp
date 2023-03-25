//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include <fstream>

int Map::tile[MAP_HEIGHT][MAP_WIDTH];

Map::Map(SDL_Renderer* renderer) {
    mapFrameCount = 2;
    loadMap();
    setMapFrameClip();
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            destRect[i][j].x = 217 + j * mapWidthFrame;
            destRect[i][j].y = i * mapHeightFrame;
            destRect[i][j].w = mapWidthFrame;
            destRect[i][j].h = mapHeightFrame;
        }
    }
    consoleMap->updateStatus("Map is created");
}

Map::~Map() {
    consoleMap->updateStatus("Map is destroyed");
}

void Map::loadMap() {
    std::ifstream mapFile(mapPath);
    if(mapFile.is_open()){
        for(auto & i : tile){
            for(auto & j : i){
                mapFile >> j;
            }
        }
    }
    else {
        consoleMap->updateStatus("Map file is not open");
    }
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 31; j++){
            if (tile[j][i] != 30 && tile[j][i] != 26 && tile[j][i] != 27) isWall[j][i] = true;
            else isWall[j][i] = false;
        }
    }
}

void Map::renderMap(SDL_Renderer *renderer) {
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            mapManager->drawTexture(mapTexture,
                                    mapFrameClip[tile[i][j]], destRect[i][j], renderer);

        }
    }
}

void Map::setMapFrameClip() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++){
            mapFrameClip[i * 4 + j].x = i * (mapWidthFrame + 1);
            mapFrameClip[i * 4 + j].y = j * (mapHeightFrame + 1);
            mapFrameClip[i * 4 + j].w = mapWidthFrame;
            mapFrameClip[i * 4 + j].h = mapHeightFrame;
        }
}

void Map::update() {

}

bool Map::checkWall(SDL_Point position){
    return isWall[position.y][position.x];
}

void Map::clean() {
    mapManager->clean(mapTexture);
}

void Map::initAnimation(SDL_Renderer *renderer) {
    while (introDelay > 0){
        mapFrame = (SDL_GetTicks() / mapAnimationDelay) % mapFrameCount;
        if (mapFrame == 1)
            mapTexture = mapManager->loadTexture(mapPNG, renderer);
        else
            mapTexture = mapManager->loadTexture(mapInversePNG, renderer);
        Map::renderMap(renderer);
        SDL_Delay(100);
        introDelay -= 0.1;
    }
}




