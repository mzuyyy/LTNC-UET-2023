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
            destRect[i][j].x = j * MAP_SIZE;
            destRect[i][j].y = MAP_SIZE * 6 + i * MAP_SIZE;
            destRect[i][j].w = MAP_SIZE;
            destRect[i][j].h = MAP_SIZE;
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
            mapFrameClip[i * 4 + j].x = i * (MAP_PIXEL + 1);
            mapFrameClip[i * 4 + j].y = j * (MAP_PIXEL + 1);
            mapFrameClip[i * 4 + j].w = MAP_PIXEL;
            mapFrameClip[i * 4 + j].h = MAP_PIXEL;
        }
}

void Map::update(Pacman *pacman) {
    removeDot(pacman);
}

void Map::removeDot(Pacman *pacman) {
    if (tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] == 26){
        tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] = 30;
        pacman->eatDot();
    }
    else if (tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] == 27){
        tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] = 30;
        pacman->eatDot();
    }

}

bool Map::isWallAt(Position position){
    std::cerr << "Tile[" << position.y / 24 << "][" << position.x / 24 << "] : " << tile[position.y / 24][position.x / 24] << std::endl;
    return  tile[position.y / 24][position.x / 24] != 30 && tile[position.y / 24][position.x / 24] != 26 && tile[position.y / 24][position.x / 24] != 27;
}





