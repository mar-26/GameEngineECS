#include <iostream>

#include "../include/GameEngine.hpp"

int main(int argc, char **argv)
{
    GameEngine game(1280, 768, "Game");
    game.run();
    return 0;
}
