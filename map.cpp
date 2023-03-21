//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include <fstream>

int Map::tile[MAP_HEIGHT][MAP_WIDTH];

Map::Map(const char *textureSheet, int x, int y, SDL_Renderer *renderer) : Object(textureSheet, x, y, renderer) {
    loadMap();
    setMapFrameClip();
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            sourceRect[i][j].x = mapFrameClip[tile[i][j]].x;
            sourceRect[i][j].y = mapFrameClip[tile[i][j]].y;
            sourceRect[i][j].w = mapFrameClip[tile[i][j]].w;
            sourceRect[i][j].h = mapFrameClip[tile[i][j]].h;
            destRect[i][j].x = j * mapWidthFrame;
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
            for(int & j : i){
                mapFile >> j;
            }
        }
    }
    else {
        consoleMap->updateStatus("Map file is not open");
    }
}

void Map::renderMap(SDL_Renderer* renderer) {
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            mapTexture->drawTexture(mapTexture->LoadTexture("../Assets/map/png", renderer), sourceRect[i][j], destRect[i][j], renderer);
        }
    }
}

void Map::setMapFrameClip() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 8; j++){
            mapFrameClip[i + j * 8].x = j * (mapWidthFrame + 1);
            mapFrameClip[i + j * 8].y = i * (mapHeightFrame + 1);
            mapFrameClip[i + j * 8].w = mapWidthFrame;
            mapFrameClip[i + j * 8].h = mapHeightFrame;
        }
}



