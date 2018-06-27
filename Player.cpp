#include "Player.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

Player::Player(const PlayerName & name, const Color & color, const BoardPosition & startPosition) :
    name_{ name }, color_{ color }, walls_{ 10 }, startPosition_{ startPosition }, arrivalPosition_{}
{
}

Move Player::getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
{
    // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
    // Only legal moves.
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

Move IA_linear::getNextMove(const unsigned int round, const Board & board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
{
    if (round == 1)
        std::cout << name() << " is playing his first move with 'IA_linear'." << std::endl;

    if ((2 * round) > move_list_.size())
        return Move();
    else
        return startPosition() == BoardPosition("e1") ? Move(move_list_[(round - 1) * 2]) : Move(move_list_[(round - 1) * 2 + 1]);
}

Move IA_random_pawn::getNextMove(const unsigned int round, const Board & board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> dis(1, 5); // 5 moves max for pawn

    auto random = dis(gen);
    while (random > pawns.size())
        random = dis(gen);
    //std::cout << random << " ";
    return pawns[random - 1];
}

Move IA_random_wall_pawn::getNextMove(const unsigned int round, const Board & board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> dis_5(1, 5); // 5 moves max for pawn
    std::uniform_int_distribution<int> dis_128(1, 128); // 5 moves max for pawn
    std::bernoulli_distribution dis_bool(0.5); // Bool 50% true

    if (dis_bool(gen) || walls.empty())
    {
        auto random = dis_5(gen);
        while (random > pawns.size())
            random = dis_5(gen);
        //std::cout << random << " ";
        return pawns[random - 1];
    }
    else
    {
        auto random = dis_128(gen);
        while (random > walls.size())
            random = dis_128(gen);
        //std::cout << random << " ";
        return walls[random - 1];
    }
}
