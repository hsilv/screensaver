#include "window.h"

int main(int argc, char *argv[])
{
    Window window("SDL2 Window", 800, 600);
    window.mainLoop();
    return 0;
}