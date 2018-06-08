#pragma once

#include "Common.h"
#include "Position.h"
#include "Board.h"

#include <vector>

class Player
{
public:
    enum class Color { red, blue, white, black };

public:
    Player() = default;
    Player(const PlayerName &name, const Color &color, const Position &startPosition);
    Player& operator=(const Player&) = default;
    ~Player() = default;
    std::vector<Position *> getNextMove(const unsigned int &round, const Board &board);

private:
    PlayerName name_;
    Color color_;
    unsigned int walls_;
    Position startPosition_;

};

