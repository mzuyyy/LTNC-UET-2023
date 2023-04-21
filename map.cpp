//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include <fstream>

int Map::tile[MAP_HEIGHT][MAP_WIDTH];

Map::Map(SDL_Renderer *renderer) {
    loadMap();
    setMapFrameClip();
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            destRect[i][j].x = j * mapWidthScreen;
            destRect[i][j].y = 24 * 3 + i * mapHeightScreen;
            destRect[i][j].w = mapWidthScreen;
            destRect[i][j].h = mapHeightScreen;
        }
    }
    consoleMap->updateStatus("Map is created");
    mapTexture = mapManager->loadTexture(MAP_PATH_PNG, renderer);
}

Map::~Map() {
    consoleMap->updateStatus("Map is destroyed");
    delete consoleMap;
}

void Map::loadMap(){
    std::ifstream mapFile(MAP_PATH);
    if(mapFile.is_open()){
        for (auto & i : tile)
            for (int & j : i)
                mapFile >> j;
    }
    else {
        consoleMap->updateStatus("Map file cannot open");
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

void Map::removeDot(Pacman *pacman) {
    if (tile[pacman->getTileID().y][pacman->getTileID().x] == 26){
        tile[pacman->getTileID().y][pacman->getTileID().x] = 30;
        pacman->eatDot();
    }
    else if (tile[pacman->getTileID().y][pacman->getTileID().x] == 27){
        tile[pacman->getTileID().y][pacman->getTileID().x] = 30;
        pacman->eatDot();
    }

}

bool Map::isWallAt(Position position) {
    std::cerr << "Tile[" << position.y / 24 << "][" << position.x / 24 << "] : " << tile[position.y / 24][position.x / 24] << std::endl;
    return  tile[position.y / 24][position.x / 24] != 30 && tile[position.y / 24][position.x / 24] != 26 && tile[position.y / 24][position.x / 24] != 27;
}





