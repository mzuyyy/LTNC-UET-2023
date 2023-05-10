//
// Created by MZuy on 3/2/2023.
//

#include "map.h"
#include <fstream>

int Map::tile[MAP_HEIGHT][MAP_WIDTH];

Map::Map(SDL_Renderer *renderer) {
    loadMap();
    setMapFrameClip();

    dotLeft = 151;

    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            destRect[i][j].x = j * MAP_SIZE;
            destRect[i][j].y = MAP_SIZE * 6 + i * MAP_SIZE;
            destRect[i][j].w = MAP_SIZE;
            destRect[i][j].h = MAP_SIZE;
        }
    }
    mapRenderer = renderer;

    mapTexture[0] = mapManager->loadTexture(MAP_PATH_PNG, renderer);
    mapTexture[1] = mapManager->loadTexture(MAP_PATH_PNG_INVERSE, renderer);

    backgroundTexture = mapManager->loadTexture(BACKGROUND_PATH, renderer);

    dotTexture = mapManager->loadTexture(DOT_PATH, renderer);

    consoleMap->updateStatus("Map is created");
}

Map::~Map() {
    consoleMap->updateStatus("Map is destroyed");
    delete consoleMap;
}

void Map::loadMap(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 4);

    dotLeft = 0;

    std::ifstream mapFile(MAP_PATH);
    if(!mapFile.eof()){
        for (auto & i : tile)
            for (int & j : i) {
                mapFile >> j;
                if (j == 26 || j == 27)
                    dotLeft++;
                if (j == 27) {
                    int random = dist(gen);
                    j = random + 31;
                }
            }
    }
    else {
        consoleMap->updateStatus("Map file cannot open");
    }
}

void Map::renderMap() {
    mapManager->drawTexture(backgroundTexture, {0, 0, 672, 888},
                            {0, 3 * 24, 672, 888}, mapRenderer);
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            mapManager->drawTexture(mapTexture[0],
                                    mapFrameClip[tile[i][j]], destRect[i][j], mapRenderer);
            if (tile[i][j] >= 32 && tile[i][j] <= 35)
            mapManager->drawTexture(dotTexture,
                                    dotFrameClip[tile[i][j] - 31], destRect[i][j], mapRenderer);
        }
    }
}

void Map::renderAnimationMap() {
    frame = (SDL_GetTicks() / MAP_ANIMATION_SPEED) % 2;
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            if (tile[i][j] < 32)
                mapManager->drawTexture(mapTexture[frame],
                                    mapFrameClip[tile[i][j]], destRect[i][j], mapRenderer);
            else {
                    mapManager->drawTexture(dotTexture,
                                    dotFrameClip[tile[i][j] - 31], destRect[i][j], mapRenderer);
            }
        }
    }
}
void Map::setMapFrameClip() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++){
            mapFrameClip[i * 4 + j] = {i * (MAP_PIXEL + 1), j * (MAP_PIXEL + 1), MAP_PIXEL, MAP_PIXEL};
        }
    for (int i = 0; i < 5; i++){
        dotFrameClip[i] = {i * (30 + 18), 0, 30, 30};
    }
}

void Map::update(Pacman *pacman, bool isNeedAnimation) {
    if (isNeedAnimation)
        Map::renderAnimationMap();
    removeDot(pacman);
}

void Map::removeDot(Pacman *pacman) {
    if (tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] == 26 ||
    tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] >= 32){
        tile[pacman->getPosition().y / MAP_SIZE][pacman->getPosition().x / MAP_SIZE] = 30;
        pacman->eatDot();
        dotLeft--;
    }
}

bool Map::isWallAt(Position position){
    return  tile[position.y / MAP_SIZE][position.x / MAP_SIZE] != 30 && tile[position.y / MAP_SIZE][position.x / MAP_SIZE] != 26
    && tile[position.y / MAP_SIZE][position.x / MAP_SIZE] != 27 && tile[position.y / MAP_SIZE][position.x / MAP_SIZE] < 32;
}

bool Map::canChangeDirectionAt(Position position) {
    return (((position.x - 14) / 24 == 30 || (position.x - 14) / 24 == 26 || (position.x - 14) / 24 == 27)
        ||  ((position.y - 14) / 24 == 30 || (position.y - 14) / 24 == 26 || (position.y - 14) / 24 == 27)
        || ((position.x + 12) / 24 == 30 || (position.x + 12) / 24 == 26 || (position.x + 12) / 24 == 27)
        || ((position.y + 12) / 24 == 30 || (position.y + 12) / 24 == 26 || (position.y + 12) / 24 == 27));
}
bool Map::isDotAt(Position position) {
    return (tile[position.y / MAP_SIZE][position.x / MAP_SIZE] == 26 || tile[position.y / MAP_SIZE][position.x / MAP_SIZE] >= 32);
} int Map::isPowerDotAt(Position position) {
    return (tile[position.y / MAP_SIZE][position.x / MAP_SIZE] >= 32) ? tile[position.y / MAP_SIZE][position.x / MAP_SIZE] - 32 : -1;
}

bool Map::isDotRunOut() const {
    return dotLeft == 0;
}





