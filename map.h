//
// Created by MZuy on 3/2/2023.
//

#ifndef BTL_MAP_H
#define BTL_MAP_H
#include <iostream>
#include <SDL_render.h>
#include <SDL.h>
#include <SDL_image.h>

class Map {
private:
    std::string mapPath = "../Assets/map.txt";
    static const int MAP_WIDTH = 28;
    static const int MAP_HEIGHT = 31;
    int tile[MAP_HEIGHT][MAP_WIDTH];
public:
    Map();
    ~Map();
    static int getTileID(int x, int y);
    void renderMap(SDL_Renderer* renderer);
};


#endif //BTL_MAP_H
