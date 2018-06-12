#include "Player.h"

#include <vector>
#include <string>

Player::Player(const PlayerName & name, const Color & color, const Position & startPosition):
    name_{ name }, color_{ color }, walls_{10}, startPosition_{startPosition}
{
}

const Move Player::getNextMove(const unsigned int &round, const Board &board)
{
    std::vector<std::string> move_list{ "e8", "e4h", "e7", "b3v", "e6", "e6", "e5", "e4" };
    return Move(move_list[round-1]);
}

