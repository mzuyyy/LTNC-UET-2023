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
    static char* mapPath = "../Assets/map.txt";
public:
    Map();
    ~Map()= default;
    void renderMap(SDL_Renderer* renderer);
};


#endif //BTL_MAP_H
