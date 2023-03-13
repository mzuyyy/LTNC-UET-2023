//
// Created by MZuy on 3/2/2023.
//

#pragma  once

#include <iostream>
#include <SDL_render.h>
#include <SDL.h>
#include <SDL_image.h>

class Map {
private:
    std::string mapPath = "../Assets/map.txt";
    static const int MAP_WIDTH = 28;
    static const int MAP_HEIGHT = 31;
    static int tile[MAP_HEIGHT][MAP_WIDTH];
    const int mapWidthFrame = 16;
    const int mapHeightFrame = 16;
    SDL_Rect mapFrameClip[39];
public:
    Map();

    ~Map();

    static int getTileID(int x, int y);

    void renderMap(SDL_Renderer* renderer);
    void  setMapFrameClip();
    SDL_Texture *dot;
    SDL_Texture *pacman;
    SDL_Texture *inky;
    SDL_Texture *blinky;
    SDL_Texture *pinky;
    SDL_Texture *clyde;
    SDL_Texture *wall;
    SDL_Texture *pellet;
    SDL_Texture *powerPellet;
    SDL_Texture *fruit;

    SDL_Rect sourceRect{}, destRect{};

    SDL_Renderer* renderer;

};
