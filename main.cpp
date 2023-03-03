#include "Game.h"

int main(int argc, char* argv[]) {
    Game* game = new Game();
    game->init();

    game->runGame();

    game->close();
    return 0;
}
