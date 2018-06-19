#pragma once

#include "Position.h"

#include <vector>
#include <map>

class Board
{
public:
    Board();
    ~Board();

    void add(const Move & move);

    const PawnPosition& position(const PlayerName& name);
    const std::vector<PawnPosition>& pawns() { return pawnsPosition_; }
    const std::vector<WallPosition>& walls() { return wallsPosition_; }
    bool exists(const WallPosition& wall);


    // To remove ?
    //const std::vector<PawnPosition>& playersPosition() { return pawnsPosition_; }
    //std::vector<WallPosition>& wallsPosition() { return wallsPosition_; }

private:
    unsigned int width_;
    unsigned int heigth_;
    std::vector<PawnPosition> pawnsPosition_;
    std::vector<WallPosition> wallsPosition_;

    void add(const PawnPosition & pawn);
    void add(const WallPosition & wall);
};

