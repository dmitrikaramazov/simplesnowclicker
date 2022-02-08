
#include "Game.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <SFML/System.hpp>

// add -mwindows to linker section when done

int main()
{
    sf::Clock fpsClock;
    float lastTime=0, currentTime=0, fps=0;
    sf::Time fpsTime;
    std::srand(std::time(nullptr));
    Game game;
    while (game.running())
    {
        
        game.update();
        game.render();
        
        fpsTime = fpsClock.getElapsedTime();
        std::cout << "\nFramerate: " << 1.0f / fpsTime.asSeconds() << "\r";
        fpsClock.restart();
        

    }
    return 0;
}