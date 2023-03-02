#include "window.h"

int main(int argc, char* argv[]) {
    auto* window = new Window();
    window->init();
    window->runGame();
    window->close();
    return 0;
}
