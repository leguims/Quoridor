#include "Player.h"

#include <vector>
#include <string>

Player::Player(const PlayerName & name, const Color & color, const BoardPosition & startPosition):
    name_{ name }, color_{ color }, walls_{ 10 }, startPosition_{ startPosition }, arrivalPosition_{}
{
}

Move Player::getNextMove(const unsigned int &round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const
{
    // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
    // Only legal moves.
    //std::vector<std::string> move_list{
    //    "e2", "e8",
    //    "e3", "e7",
    //    "e4", "e6",
    //    "e3h", "g6v",
    //    "e5", "e4",
    //    "e6", "d4",
    //    "c3h", "a3h",
    //    "e7", "e7h",
    //    "d6v", "d4v"
    //};

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

    // Meet on center and create closed square around player and then jump each other.
    // Wall "c4v" is ILLEGAL MOVE !
    //std::vector<std::string> move_list{
    //    "e2", "e8",
    //    "e3", "e7",
    //    "e4", "e6",
    //    "d3h", "e5",
    //    "e4v", "d5h",
    //    "c4v", "d4",
    //    "d5", "e5",
    //    "e4", "d5",
    //    "d4", "e4",
    //    "e5", "d5",
    //    "d4", "e5",
    //    "e4", "d4",
    //};

    //std::vector<std::string> move_list{
    //    "e2", "e8",
    //    "e3", "e7",
    //    "e4", "e6",
    //    "e3h", "e5",
    //    "g6v", "d4",
    //    "e5", "d3",
    //    "e6", "d2",
    //    "e7", "d1",
    //    "e8", "e1",
    //    "e9", "f1"
    //};

    if ( (2*round) > move_list.size())
        return Move();
    else
        return startPosition_==BoardPosition("e1")? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2+1]) ;
}

