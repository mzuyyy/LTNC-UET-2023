## Intro

Mai Ngoc Duy - 22028255 

K67 - CA - CLC1

Project - Pacman

Advanced Programming class, Group 22 (INT2215 22)

Theory lecturer:  [Dr.Tran Thi Minh Chau](https://uet.vnu.edu.vn/~chauttm/)

Assignment lecturer: MSc. Kieu Hai Dang

### Table of Contents  
**[Started](#started)**  
- [Ideas](#ideas)
- [Prepared and references](#prepared-and-references)

**[Overview](#overview)**
- [Playing](#playing)
- [New Features](#new-features)

**[Control](#control)**

**[Ghost AI](#ghost-ai)**

**[Demo videos](#demo-videos)**

**[Review](#review)**

## Started

### Ideas
My first major project at UET will be to clone the game Pacman while adding new features and creative elements to make the game more exciting.
The original game includes:
- Pacman (the main character), who can eat small dots to gain points and large dots to eat the ghosts.
- Ghosts (including 4 types) are characters that prevent Pacman from eating the dots:
  - Blinky: the red ghost, has the ability to chase Pacman.
  - Pinky: the pink ghost, has the ability to cut off Pacman's path.
  - Inky: the blue ghost, has the ability to corral Pacman into a corner.
  - Clyde: the orange ghost, will move at least 8 tiles (24) away from Pacman, otherwise it will return to the scatter state instead of chasing him.
- Pacman gameplay is very simple, you have to eat all the gold dots while trying to avoid the 4 ghosts in the maze. When you eat a large dot, you will gain the ability to attack the ghosts.
### Prepared and references
Please note that I have consulted the following sources for my Pacman game project:
- A video series on game programming with SDL2 by [Madsycode](https://www.youtube.com/watch?v=1KD4Ae0tX0g&list=PL-K0viiuJ2RctP5nlJlqmHGeh66-GOZR).
- The thought process and steps to make Pacman game by [Kofybrek](https://youtu.be/vC0d1rDmPBs).
- Some documents and videos on OOP and SDL2 that I don't remember the sources.
- Additionally, with the help of my seniors and other K67 friends.

## Overview

### Playing
The game consists of 6 levels:
Each level requires the player to control Pacman to move around the map and eat all the yellow dots.
When all the dots are eaten, the player advances to the next level.
After level 1, the ghosts will evolve into new ghosts with special abilities:
- Blinky -> Deadly: now can make Pacman lose 2 lives when bitten.
- Pinky -> Speedy: can speed up but will slow down when approaching Pacman. When bitten, Pacman will be slowed down.
- Clyde -> Invisy: can turn invisible for 1.2 seconds and then reappear. When bitten, Pacman will be blinded.
- Inky -> Freezy: When bitten, Pacman will be frozen.

### New Features
New features that I have added to the game:
- Large dots have been upgraded to 4 separate dots:
    - "P" Dot: the original large dot, allows Pacman to eat ghosts.
    - "S" Dot: the speed dot, gives Pacman super speed.
    - "F" Dot: the freeze dot, freezes all objects except for Pacman.
    - "I" Dot: the invisibility dot, makes Pacman invisible.
- Pacman now have more 2 types that you can choose in setting:
    - Ms Pacman.
    - Android Pacman.
- Can now save settings from previous gameplay. 
- Randomized placement of Big dots on the map.
- Added the feature to display Pacman's current power and current bad effect.
- Added 4 new Ghosts (upgraded versions of the 4 original Ghosts):
    - Deadly
    - Speedy
    - Invisy
    - Freezy
## Control
The player uses 4 keys on the keyboard to control Pacman.
Currently, there are 3 keyboard combinations supported in the game:
- Arrows keys.
- WASD
- IJKL

In addition, using the menu also relies on the keyboard:
- Arrow keys to move the button.
- Enter or space key to select the button.
- Escape key to exit the current State window.
## Ghost AI

### Ghost
Ghost is a subclass of the Object class (an element that can move in the game).  
In essence, Ghost inherits everything from Object:
- Movement parameters, velocity, current position, etc.
- Additionally, Ghost has its own states, such as chase, scared, scatter, dead, etc.

### Ghost AI
```C++
///Calculate new Direction
Direction PlayState::calculateDirection(Ghost *ghost) {
    Position newPosition;
    double distance, minDistance = static_cast<double>(1e12);
    Direction newDirection = NONE;

    for (int direction = UP; direction <= DOWN; direction++) {
        if (ghost->getDirection() == -direction || direction == NONE)
            continue;
        distance = static_cast<double>(1e12);
        if (!Map::isWallAt(ghost->getNextPosition(static_cast<Direction>(direction)))){
            newPosition = ghost->getPosition();
            switch (direction) {
                case UP:
                    newPosition.y -= 42;
                    break;
                case DOWN:
                    newPosition.y += 42;
                    break;
                case LEFT:
                    newPosition.x -= 42;
                    break;
                case RIGHT:
                    newPosition.x += 42;
                    break;
                default:
                    break;
            }
            distance = Distance::Euclidean(newPosition, ghost->getTarget());
        }
        if (minDistance > distance){
            minDistance = distance;
            newDirection = static_cast<Direction>(direction);
        }
    }
    return newDirection;
}
```
The function calculateDirection will compute the next direction for the Ghost if it reaches a junction or cannot continue in its current direction. The calculation formula is based on simulating if the Ghost moves in the four basic directions, which direction is the optimal one to get it closer to its target.

The algorithm returns the optimal direction for the Ghost to move in the move function.
```C++
void PlayState::handleGhostMove(Ghost *ghost) {
    if (ghost->getCurrentState() == GHOST_STAND || ghost->getCurrentState() == GHOST_FREEZE)
        return;
    if(Map::isWallAt(ghost->getNextPosition(ghost->getDirection()))){
        ghost->setDirection(calculateDirection(ghost));
    }
    else {
        if (ghost->checkPosition() || ghost->getPosition() == ghost->getBlinkyStartPosition())
            ghost->setDirection(calculateDirection(ghost));
    }
}
```
# Demo videos
[The google drive link about the gameplay](https://drive.google.com/drive/folders/1XQTxJydGS0o4TwUphzdRI_Zh1Y5PGHTn?usp=share_link)
Includes 3 videos:
- 1 video about Menu.
- 2 video demo the level-1 game play.
- 1 video about the times Ghosts were upgraded and gameplay with upgraded ghosts.


## Review
After a long period of working on the project, I feel that I have made a lot of progress in both programming knowledge and thinking skills. If given more time, I would further develop the game to make it more complete in some aspects. In my personal evaluation, I believe that I deserve a score of 10 for this project.
