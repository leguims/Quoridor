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
    Player();
    ~Player();
    std::vector<Position *> getNextMove(const Board &board);

private:
    PlayerName name_;
    Color color_;
    unsigned int walls_;
    Position startPosition_;

};

