//
// Created by MZuy on 3/28/2023.
//

#ifndef BTL_GHOST_H
#define BTL_GHOST_H
#include "Object.h"

enum GHOST_TYPE{
    BLINKY,
    DEADLY,
    CLYDE,
    INKY,
    FREEZY,
    INVISY,
    PINKY,
    SPEEDY,
};
class Ghost : public Object{
private:
    GHOST_TYPE type;

    bool isFrighten;

    bool scattering;
    bool  outOfCage;

public:
    Ghost(GHOST_TYPE type, TileID tile);

    Position getPosition(){
        return position;
    }

    TileID getTileID(){
        return tileID;
    }

};


#endif //BTL_GHOST_H
