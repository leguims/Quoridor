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

    std::cout << "Game result : " << game.getResult();

    return 0;
}

