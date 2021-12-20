/*
  Games Development with C++ GAV1031-N
  Snake ICA start project 
  
  Set up to use the SFML 2.5.1 64-bit API
    SFML documentation: https://www.sfml-dev.org/learn.php
*/

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Snake.h"
#include "PickUp.h"

int main()
{
    //main() is only used to create the game object and to call Run() to start the game
    srand((unsigned int)time(NULL));
    Game game;

    // We can still output to the console window
    std::cout << "SnakeGame: Starting" << std::endl;

    game.Run();
    

    std::cout << "SnakeGame: Finished" << std::endl;

    return 0;
}
