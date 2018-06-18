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

	PawnPosition& position(const PlayerName& name);

    // To remove ?
    //std::vector<PawnPosition>& playersPosition() { return playersPosition_; }
    std::vector<WallPosition>& wallsPosition() { return wallsPosition_; }

private:
    unsigned int width_;
    unsigned int heigth_;
    std::map<std::string, PawnPosition> playersPosition_;
    std::vector<WallPosition> wallsPosition_;

	void add(const PawnPosition & pawn);
	void add(const WallPosition & wall);
};

