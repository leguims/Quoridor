// Quoridor.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>

#include "Game.h"

int main()
{
    {
        BoardPosition position(5, 9);
        std::cout << "Position : " << position << std::endl;
    }

    {
        PawnPosition pawnPosition(5, 9, "Gerard");
        std::cout << "PawnPosition : " << pawnPosition << std::endl;
    }

    {
        WallPosition wallPosition(5, 9, WallPosition::Direction::horizontal);
        std::cout << "WallPosition : " << wallPosition << std::endl;
    }

    {
        WallPosition wallPosition2(6, 8, "v");
        std::cout << "WallPosition2 : " << wallPosition2 << std::endl;
    }


	std::cout << std::endl << std::endl;
	Game game;

    game.chooseReferee();
    game.choosePlayers();
    game.launch();

    game.move();    game.move();
    game.move();    game.move();
    game.move();    game.move();
    game.move();    game.move();
    game.move();    game.move();
    game.move();    game.move();
    game.move();    game.move();
	game.move();    game.move();
	game.move();    game.move();
	game.move();    game.move();
	game.move();    game.move();

    game.move();    game.move();
	game.move();    game.move();

    //game.save();
    std::cout << game;

    std::cout << "Game result : " << game.getResult();

    return 0;
}

