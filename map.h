//
// Created by MZuy on 3/2/2023.
//

#pragma  once

#include <iostream>
#include <SDL_render.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Object.h"

class Map : public Object {
private:
    std::string mapPath = "../Assets/map.txt";

    static const int MAP_WIDTH = 28;
    static const int MAP_HEIGHT = 31;

    static int tile[MAP_HEIGHT][MAP_WIDTH];

    const int mapWidthFrame = 16;
    const int mapHeightFrame = 16;

    SDL_Rect mapFrameClip[30];
public:
    Map(const char *textureSheet, int x, int y, SDL_Renderer *renderer);

    ~Map();

    static int getTileID(int x, int y);

    void loadMap();

    void renderMap(SDL_Renderer* renderer);

    void  setMapFrameClip();

    SDL_Rect sourceRect[31][28], destRect[31][28];

};
