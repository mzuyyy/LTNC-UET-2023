//
// Created by MZuy on 3/2/2023.
//

#pragma  once

#include <iostream>
#include "logStatus.h"
#include "textureManager.h"
#include <SDL_render.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Object.h"

class Map{
private:
    std::string mapPath = "../Assets/map.txt";
    std::string mapPNG = "../Assets/map.png";
    std::string mapInversePNG = "../Assets/mapInverse.png";

    double introDelay = 1;
    int mapFrame;
    int mapFrameCount;
    int mapAnimationDelay = 100;

    static const int MAP_WIDTH = 28;
    static const int MAP_HEIGHT = 31;

    static int tile[MAP_HEIGHT][MAP_WIDTH];

    const int mapWidthFrame = 16;
    const int mapHeightFrame = 16;

    SDL_Rect mapFrameClip[32]{};

    Log* consoleMap = new Log("Map");

    textureManager* mapManager = new textureManager();

    std::vector<bool> isPacmanEaten;

    bool isWall[448][498]{};

    bool isPacmanThrough[MAP_HEIGHT][MAP_WIDTH]{};

    SDL_Texture* mapTexture = nullptr;
public:
    Map(SDL_Renderer* renderer);

    ~Map();

    void initAnimation(SDL_Renderer *renderer);

    void loadMap();

    void renderMap(SDL_Renderer *renderer);

    void  setMapFrameClip();

    void update();

    bool checkWall(SDL_Point position);

    void clean();
    SDL_Rect destRect[31][28]{};

};
