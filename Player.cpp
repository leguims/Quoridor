#include "Player.h"

#include <vector>
#include <string>

Player::Player(const PlayerName & name, const Color & color, const Position & startPosition):
    name_{ name }, color_{ color }, walls_{10}, startPosition_{startPosition}
{
}

Move Player::getNextMove(const unsigned int &round, const Board &board) const
{
    // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
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
    if ( (2*round) > move_list.size())
        return Move();
    else
        return startPosition_==Position("e1")? Move(move_list[(round - 1) * 2]) : Move(move_list[(round - 1) * 2+1]) ;
}

