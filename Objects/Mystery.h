//
// Created by MZuy on 4/27/2023.
//

#ifndef BTL_MYSTERY_H
#define BTL_MYSTERY_H

#include "Object.h"
#include "Ghost.h"

class Mystery : public Ghost{
private:
    GHOST_TYPE ghostType = MYSTERY;
    SDL_Texture* mysteryTexture;
public:
    
};


#endif //BTL_MYSTERY_H
