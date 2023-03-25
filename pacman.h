//
// Created by MZuy on 3/12/2023.
//

#ifndef BTL_PACMAN_H
#define BTL_PACMAN_H

#include "Object.h"
#include "textureManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <string>
#include <map>

class Pacman : public Object{
public:
    Pacman(const std::string& textureSheet, SDL_Renderer *renderer);
    ~Pacman();

    void update();
    void render();
    void deleteOldFrame();
    void setPacmanFrameClip();
    void changeDirection(const std::string& direction);
    void clean();

    std::map<std::string,bool> pacmanDir = {
            {"left", false},
            {"right", false},
            {"up", false},
            {"down", false}
    };

    SDL_Point currentPosition(){
        SDL_Point tempPosition = pacmanPosition;
        tempPosition.x -= 217;
        tempPosition.x += 11;
        tempPosition.y += 11;
        return tempPosition;
    }
    SDL_Point nextPosition(SDL_Event event) {
        SDL_Point nextPosition = pacmanPosition;
        nextPosition.x += 11;
        nextPosition.y += 11;
        if (event.key.keysym.sym == SDLK_UP) nextPosition.y -= 11;
        else if (event.key.keysym.sym == SDLK_DOWN) nextPosition.y += 11;
        else if (event.key.keysym.sym == SDLK_LEFT) nextPosition.x -= 11;
        else if (event.key.keysym.sym == SDLK_RIGHT) nextPosition.x += 11;
        nextPosition.x -= 217;
        nextPosition.x /= 16;
        nextPosition.y /= 16;
        return nextPosition;
    }
    const SDL_Point defaultPosition = {430 , 366};

    void checkTouchWall(bool check){
        isTouchWall = check;
    }
private:
    int pacmanFrame;
    int pacmanAnimationSpeed;
    int pacmanFrameCount;

    int pacmanVelocity = 1;

    bool isTouchWall = false;

    const char* deletePacmanTexture = "../Assets/map.png";

    SDL_Renderer* pacmanRenderer = nullptr;

    SDL_Point pacmanPosition{};

    SDL_Texture* pacmanTexture{};

    SDL_Rect destRect{};

    textureManager* pacmanManager = new textureManager();

    int widthFrame = 26;
    int heightFrame = 26;

    int pacmanWidth = 22;
    int pacmanHeight = 22;

    SDL_Rect frameClip[13]{};
    std::string pacmanTextureSheet = "../Assets/pacmanTexture.png";
};


#endif //BTL_PACMAN_H
