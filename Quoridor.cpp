// Quoridor.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>

#include "Game.h"

int main()
{
    Game game;

    game.chooseReferee();
    game.choosePlayers();
    game.launch();

    game.move();
    game.move();
    game.move();
    game.move();
    game.move();
    game.move();
    game.move();
    game.move();

    game.save();

    std::cout << "Game result : " << game.getResult();

    return 0;
}

