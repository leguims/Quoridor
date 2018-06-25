// Quoridor.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>

#include "Game.h"

class IA_1 : public Player
{
public:
    IA_1(const PlayerName &n, const Color &c, const BoardPosition &sp) : Player(n, c, sp) {}
    Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
    {
        if (round == 1)
            std::cout << name() << " is playing his first move with 'IA_1'." << std::endl;

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
            "d6v", "d4v"
        };

        if ((2 * round) > move_list.size())
            return Move();
        else
            return startPosition() == BoardPosition("e1") ? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2 + 1]);
    }
};

struct IA_2 : public Player
{
    IA_2(const PlayerName &n, const Color &c, const BoardPosition &sp) : Player(n, c, sp) {}
    Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
    {
        if (round == 1)
            std::cout << name() << " is playing his first move with 'IA_2'." << std::endl;

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

        if ((2 * round) > move_list.size())
            return Move();
        else
            return startPosition() == BoardPosition("e1") ? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2 + 1]);
    }
};

struct IA_3 : public Player
{
    IA_3(const PlayerName &n, const Color &c, const BoardPosition &sp) : Player(n, c, sp) {}
    Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
    {
        if (round == 1)
            std::cout << name() << " is playing his first move with 'IA_3'." << std::endl;

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
            "e4", "d5",
            "d4", "e4",
            "e5", "d5",
            "d4", "e5",
            "e4", "d4",
        };

        if ((2 * round) > move_list.size())
            return Move();
        else
            return startPosition() == BoardPosition("e1") ? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2 + 1]);
    }
};

struct IA_4 : public Player
{
    IA_4(const PlayerName &n, const Color &c, const BoardPosition &sp) : Player(n, c, sp) {}
    Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
    {
        if (round == 1)
            std::cout << name() << " is playing his first move with 'IA_4'." << std::endl;

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
            "e9", "f1"
        };

        if ((2 * round) > move_list.size())
            return Move();
        else
            return startPosition() == BoardPosition("e1") ? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2 + 1]);
    }
};

void test_BoardPosition();
void test_Game();

int main()
{
    test_BoardPosition();
    test_Game();

    return 0;
}

void test_Game()
{
    auto test = [](std::string title, Player *p1, Player *p2) {
        std::cout << "test_Game(" << title << ") : START" << std::endl;
        Game game;

        game.chooseReferee();
        game.choosePlayers(p1, p2);
        game.launch();

        while (game.getResult() == Game::Result::inProgress)
            game.move();

        //game.save();
        std::cout << game;
        std::cout << "Game result : " << game.getResult() << std::endl;
        std::cout << "test_Game(" << title << ") : END\n\n";
    };

    {
        auto ia1 = new IA_1("*IA_1: Player 1*", Player::Color::black, BoardPosition("e1"));
        auto ia2 = new IA_1("*IA_1: Player 2*", Player::Color::white, BoardPosition("e9"));
        test("IA_1 versus IA_1", ia1, ia2);
        delete ia1, ia2;
    }

    {
        auto ia1 = new IA_2("*Player 1*", Player::Color::black, BoardPosition("e1"));
        auto ia2 = new IA_2("*Player 2*", Player::Color::white, BoardPosition("e9"));
        test("IA_2 versus IA_2", ia1, ia2);
        delete ia1, ia2;
    }

    {
        auto ia1 = new IA_3("Player 1", Player::Color::black, BoardPosition("e1"));
        auto ia2 = new IA_3("Player 2", Player::Color::white, BoardPosition("e9"));
        test("IA_3 versus IA_3", ia1, ia2);
        delete ia1, ia2;
    }

    {
        auto ia1 = new IA_4("*IA_4-Player 1*", Player::Color::black, BoardPosition("e1"));
        auto ia2 = new IA_4("*IA_4-Player 2*", Player::Color::white, BoardPosition("e9"));
        test("IA_4 versus IA_4", ia1, ia2);
        delete ia1, ia2;
    }
}

void test_BoardPosition()
{
    std::cout << "test_BoardPosition() : START" << std::endl;
    {
        BoardPosition position(5, 9);
        std::cout << "Position : " << position << std::endl;
    }

    {
        BoardPosition position("e1");
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
    std::cout << "test_BoardPosition() : END\n\n";
}
