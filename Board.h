#pragma once

#include "Player.h"
#include "Position.h"

#include <vector>
#include <memory>

class Board
{
public:
    Board();
    ~Board();

private:
    unsigned int width_;
    unsigned int heigth_;
    std::vector<PlayerPosition> playersPosition_;
    std::vector<Position> wallsPosition_;
};

