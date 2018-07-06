// Quoridor.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <thread>

// #define NDEBUG
#include <cassert>

#include "Game.h"

void test_BoardPosition();
void test_Board();
void test_Game_IA_linear();
void test_Game_IA_random();

int main()
{
    test_BoardPosition();
    test_Board();
    test_Game_IA_linear();
    test_Game_IA_random();

    {
        using namespace std::chrono_literals;
        std::cout << "End in ";
        for (auto i = 5; i > 0; --i)
        {
            std::cout << i << " ";
            std::this_thread::sleep_for(1s);
        }
    }

    return 0;
}

void test_Game_IA_random()
{
    auto test = [](std::string title, Player *p1, Player *p2, bool showMoves) {
        std::cout << "test_Game(" << title << ") : START" << std::endl;
        Game game;

        game.chooseReferee();
        game.choosePlayers(p1, p2);
        game.showMoves(showMoves);
        game.launch();
        assert(game.getResult() == Game::Result::inProgress);

        while (game.getResult() == Game::Result::inProgress)
            game.move();

        std::cout << game;
        std::cout << "Game result : " << game.getResult() << std::endl;
        std::cout << "Save game to file : " << game.save() << std::endl;
        std::cout << "test_Game(" << title << ") : END\n\n";
    };

    {
        auto ia1 = new IA_random_pawn("IA_random_pawn.1", Color::black, BoardPosition("e1"));
        auto ia2 = new IA_random_pawn("IA_random_pawn.2", Color::white, BoardPosition("e9"));
        test("Test Random pawn 1", ia1, ia2, false);
        delete ia1, ia2;
    }

    {
        auto ia1 = new IA_random_wall_pawn("IA_random_wall_pawn.1", Color::black, BoardPosition("e1"));
        auto ia2 = new IA_random_wall_pawn("IA_random_wall_pawn.2", Color::white, BoardPosition("e9"));
        test("Test Random wall/pawn 1", ia1, ia2, false);
        delete ia1, ia2;
    }
}

void test_Game_IA_linear()
{
    auto test = [](std::string title, Player *p1, Player *p2, Game::Result result, bool showMoves = false) {
        std::cout << "test_Game(" << title << ") : START" << std::endl;
        Game game;

        game.chooseReferee();
        game.choosePlayers(p1, p2);
        game.showMoves(showMoves);
        game.launch();
        assert(game.getResult() == Game::Result::inProgress);

        while (game.getResult() == Game::Result::inProgress)
            game.move();

        assert(game.getResult() == result);
        std::cout << game;
        std::cout << "Game result : " << game.getResult() << std::endl;
        std::cout << "test_Game(" << title << ") : END\n\n";
    };

    {
        // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
        //   Only legal moves.
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "g6v",
            "e5", "e4",
            "e6", "d4",
            "c3h", "a3h",
            "e7", "e7h",
            "d6v", "d4v" // Game not over, player 1 resigns
        };

        auto ia1 = new IA_linear("*IA_1: Player 1*", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("*IA_1: Player 2*", Color::white, BoardPosition("e9"), move_list);
        test("Test 1", ia1, ia2, Game::Result::win2);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "g6v",
            "e5", "e4",
            "e6", "d4",
            "c3h", "a3h",
            "e7", "e7h",
            "d6v", "d4v",
            "e8", "d3", // e8 illegal because of "e7h" !
            "e9", "d2"
        };

        auto ia1 = new IA_linear("*Player 1*", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("*Player 2*", Color::white, BoardPosition("e9"), move_list);
        test("Test 2", ia1, ia2, Game::Result::win2);
        delete ia1, ia2;
    }

    {
        // Meet on center and create closed square around player and then jump each other.
        // Wall "c4v" is ILLEGAL MOVE !
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "d3h", "e5",
            "e4v", "d5h",
            "c4v", "d4", // Illegal c4v, pawn is in jail !
            "d5", "e5",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 3", ia1, ia2, Game::Result::win2);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "e5",
            "g6v", "d4",
            "e5", "d3",
            "e6", "d2",
            "e7", "d1", // Player 2 wins, end of game.
            "e8", "e1",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 4", ia1, ia2, Game::Result::win2);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "b5v", "b6v",  // Illegal b6v, wall cannot use b6v twice !
            "e3", "e7",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 5", ia1, ia2, Game::Result::win1);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "f1", "h5v",
            "e3h", "b8v",
            "d4v", "d9",
            "a4h", "c9",
            "g1", "b9",  // Illegal b9, cannot jump over the wall b8v !
            "f1", "c9",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 6", ia1, ia2, Game::Result::win1);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e5", "e5", // Illegal move, e5 is not empty.
            "g6v", "d4",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 1", ia1, ia2, Game::Result::win1);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e5", "e5v",
            "c6h", "d6h",  // Illegal d6h, wall cannot use d6h twice !
            "f6", "e5",
            "f7", "e4",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 2", ia1, ia2, Game::Result::win1);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "i1", // Illegal move, i1 is too far from e9.
            "e3", "e7",
        };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 3", ia1, ia2, Game::Result::win1);
        delete ia1, ia2;
    }
}

void test_BoardPosition()
{
    std::cout << "test_BoardPosition() : START" << std::endl;
    {
        BoardPosition position(5, 9);
        std::cout << "Position : " << position << std::endl;
        assert(BoardPosition(5, 9) == position);
    }

    {
        BoardPosition position("e1");
        std::cout << "Position : " << position << std::endl;
        assert(BoardPosition(5, 1) == position);
    }

    {
        PawnPosition pawnPosition(5, 9, "Gerard");
        std::cout << "PawnPosition : " << pawnPosition << std::endl;
        assert(BoardPosition(5, 9) == pawnPosition);
        assert("Gerard" == pawnPosition.playerName());
    }

    {
        WallPosition wallPosition(5, 9, WallPosition::Direction::horizontal);
        std::cout << "WallPosition : " << wallPosition << std::endl;
        assert(BoardPosition(5, 9) == wallPosition);
        assert(WallPosition::Direction::horizontal == wallPosition.direction());
    }

    {
        WallPosition wallPosition(6, 8, "v");
        std::cout << "WallPosition2 : " << wallPosition << std::endl;
        assert(BoardPosition(6, 8) == wallPosition);
        assert(WallPosition::Direction::vertical == wallPosition.direction());
    }
    std::cout << "test_BoardPosition() : END\n\n";
}

void test_Board()
{
    std::cout << "test_Board() : START\n\n";
    {
        Board board;

        Move michou = PawnPosition(5, 1, "Michou", Color::black); board.add(michou);
        michou = PawnPosition(5, 2, "Michou"); board.add(michou);
        Move gerard = PawnPosition(5, 9, "Gerard", Color::white); board.add(gerard);
        assert(board.pawns().size() == 2);
        assert(board.existsPawn({ 5, 2 }));
        assert(board.existsPawn({ 5, 9 }));

        Move wallPosition1 = WallPosition(1, 8, "h"); board.add(wallPosition1);
        Move wallPosition2 = WallPosition(1, 1, "h"); board.add(wallPosition2);
        Move wallPosition3 = WallPosition(4, 8, "v"); board.add(wallPosition3);
        Move wallPosition4 = WallPosition(5, 1, "v"); board.add(wallPosition4);
        Move wallPosition5 = WallPosition(5, 4, "h"); board.add(wallPosition5);
        Move wallPosition6 = WallPosition(4, 4, "v"); board.add(wallPosition6);
        assert(board.walls().size() == 6);
        assert(board.existsWall({ 1, 8, "h" }));
        assert(board.existsWall({ 1, 1, "h" }));
        assert(board.existsWall({ 4, 8, "v" }));
        assert(board.existsWall({ 5, 1, "v" }));
        assert(board.existsWall({ 5, 4, "h" }));
        assert(board.existsWall({ 4, 4, "v" }));

        std::cout << "Board : \n"
            << board << std::endl;
    }
    std::cout << "test_Board() : END\n\n";
}
