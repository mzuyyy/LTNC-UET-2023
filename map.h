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
#include "Object/pacman.h"
class Map{
private:
    std::string mapPath = "../Assets/map.txt";
    std::string mapPNG = "../Assets/map.png";
    std::string mapInversePNG = "../Assets/mapInverse.png";

    int introDelay = 100;
    int mapFrame{};
    int mapFrameCount;

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
public:
    explicit Map(SDL_Renderer *renderer);

    ~Map();

    void initAnimation(SDL_Renderer *renderer);

    void loadMap();

    void renderMap(SDL_Renderer *renderer);

    void  setMapFrameClip();

    void update();

    static bool isWallAt(TileID tileID);

    static void removeDot(Pacman* pacman);

    SDL_Rect destRect[31][28]{};
};
