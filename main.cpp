#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include "Globals.h"
#include "Game.h"

int main()
{
    // Set working directory to where the exe is located
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string exeDir(exePath);
    exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));
    SetCurrentDirectoryA(exeDir.c_str());

    sf::RenderWindow win(sf::VideoMode({ WIN_WIDTH, WIN_HEIGHT }), "Flappy Bird");
    Game game(win);
    game.startGameLoop();
    return 0;
}