#pragma once

#include "Position.h"

#include <vector>

class Board
{
public:
    Board();
    ~Board();

private:
    unsigned int width_;
    unsigned int heigth_;
    std::vector<PawnPosition> playersPosition_;
    std::vector<WallPosition> wallsPosition_;
};

