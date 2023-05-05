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
const int MAP_PIXEL = 16;

const int MAP_SIZE = 24;

static const int MAP_WIDTH = 28;
static const int MAP_HEIGHT = 31;

class Map{
private:
    std::string MAP_PATH = "../Assets/map.txt";
    std::string MAP_PATH_PNG = "../Assets/map.png";
    std::string MAP_PATH_PNG_INVERSE = "../Assets/mapInverse.png";

    friend Object;

    static int tile[MAP_HEIGHT][MAP_WIDTH];

    SDL_Rect mapFrameClip[32]{};

    Log* consoleMap = new Log("Map");

    SDL_Renderer* mapRenderer;

    textureManager* mapManager = new textureManager();

    SDL_Texture* mapTexture[2];

    TileID pacmanStandID;
    TileID ghostStandID;
    TileID pinkyStandID, inkyStandID, clydeStandID, blinkyStandID;

    bool isDot[MAP_HEIGHT][MAP_WIDTH]{};
    int dotLeft;

    int frame{};
    const int frameCount = 6;
    const int MAP_ANIMATION_SPEED = 75;
public:
    explicit Map(SDL_Renderer *renderer);

    ~Map();

    void loadMap();

    void renderMap();

    void animationMap();

    void  setMapFrameClip();

    void update(Pacman *pacman, bool isNeedAnimation);

    static bool isWallAt(Position position);

    static bool canChangeDirectionAt(Position position);

    void removeDot(Pacman* pacman);

    SDL_Rect destRect[31][28]{};
};
