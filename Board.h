#pragma once

#include "Position.h"

#include <vector>

class Board
{
public:
    Board();
    ~Board();

    void add(const PawnPosition & pawn);
    void add(const WallPosition & wall);
    void add(const Move & move);

    // To remove ?
    std::vector<PawnPosition>& playersPosition() { return playersPosition_; }
    std::vector<WallPosition>& wallsPosition() { return wallsPosition_; }

private:
    unsigned int width_;
    unsigned int heigth_;
    std::vector<PawnPosition> playersPosition_;
    std::vector<WallPosition> wallsPosition_;
};

