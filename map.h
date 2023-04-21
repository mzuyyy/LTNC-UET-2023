//
// Created by MZuy on 3/2/2023.
//

#pragma  once

#include <iostream>
#include "logStatus.h"
#include "textureManager.h"
#include "Objects/Object.h"
#include "Objects/pacman.h"
#include <SDL_render.h>
#include <SDL.h>
#include <SDL_image.h>

class Map{
private:
    std::string MAP_PATH = "../Assets/map.txt";
    std::string MAP_PATH_PNG = "../Assets/map.png";
    std::string MAP_PATH_PNG_INVERSE = "../Assets/mapInverse.png";

    friend Object;

    static const int MAP_WIDTH = 28;
    static const int MAP_HEIGHT = 31;

    static int tile[MAP_HEIGHT][MAP_WIDTH];

    const int mapWidthFrame = 16;
    const int mapHeightFrame = 16;

    const int mapWidthScreen = 24;
    const int mapHeightScreen = 24;

    SDL_Rect mapFrameClip[32]{};

    Log* consoleMap = new Log("Map");

    textureManager* mapManager = new textureManager();

    SDL_Texture* mapTexture = nullptr;

    TileID pacmanStandID;
    TileID ghostStandID;
    TileID pinkyStandID, inkyStandID, clydeStandID, blinkyStandID;

    bool isDot[MAP_HEIGHT][MAP_WIDTH]{};
    int dotLeft;
public:
    explicit Map(SDL_Renderer *renderer);

    ~Map();

    void loadMap();

    void renderMap(SDL_Renderer *renderer);

    void  setMapFrameClip();

    void update();

    static bool isWallAt(Position position);

    static void removeDot(Pacman* pacman);

    SDL_Rect destRect[31][28]{};
};
