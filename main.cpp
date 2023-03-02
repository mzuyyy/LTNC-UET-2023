#include "window.h"

int main(int argc, char* argv[]) {
    Window* window = new Window();
    window->init();
    window->runGame();
    window->close();
}
