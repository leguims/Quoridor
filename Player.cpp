#include "Player.h"


Player::Player(const PlayerName & name, const Color & color, const Position & startPosition):
    name_{ name }, color_{ color }, walls_{10}, startPosition_{startPosition}
{
}

std::vector<Position *> Player::getNextMove(const unsigned int &round, const Board & board)
{
    return std::vector<Position *>();
}

