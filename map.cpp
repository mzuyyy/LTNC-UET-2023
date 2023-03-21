//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include "logStatus.h"
#include "textureManager.h"

Log* consoleMap = new Log("Map");
int Map::tile[MAP_HEIGHT][MAP_WIDTH];

//write function to read map.txt
Map::Map(const char *textureSheet, int x, int y, SDL_Renderer *renderer) : Object(textureSheet, x, y, renderer) {
    consoleMap->updateStatus("Map is created");
    std::ifstream mapFile(textureSheet);
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

void Map::loadMap() {

}

void Map::renderMap(SDL_Renderer* renderer) {

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


