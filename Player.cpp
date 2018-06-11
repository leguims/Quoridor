#include "Player.h"


Player::Player(const PlayerName & name, const Color & color, const Position & startPosition):
    name_{ name }, color_{ color }, walls_{10}, startPosition_{startPosition}
{
}

const Move& Player::getNextMove(const unsigned int &round, const Board &board)
{
    return Move("e7");
}

