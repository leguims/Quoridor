#include "Board.h"
#include <algorithm>

Board::Board() : width_{ 0 }, heigth_{ 0 }
{
}

Board::~Board()
{
}

void Board::add(const PawnPosition & pawn)
{
	// Find player on board
    auto name = pawn.playerName();
    auto player = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(), 
        [name] (const PawnPosition &p)->bool { return (p.playerName() == name); });

    if (player == pawnsPosition_.end())
	{
		// New pawn : Add it
        pawnsPosition_.emplace_back(pawn);
	}
	else
	{
		// Existing one : replace it
		*player = pawn;
	}
}

void Board::add(const WallPosition & wall)
{
	wallsPosition_.emplace_back(wall);
}

void Board::add(const Move & move)
{
	if (move.type() == Move::Type::pawn)
		add(move.pawn());
	else if (move.type() == Move::Type::wall)
		add(move.wall());
	else if (move.type() == Move::Type::none)
		return;
	else
		throw std::out_of_range("Unknown move to add to the board");
}

const PawnPosition & Board::position(const PlayerName & name)
{
    // Find player by name in vector
    auto player = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(),
        [name](const PawnPosition &p)->bool { return (p.playerName() == name); });

	if(player == pawnsPosition_.end())
		throw std::out_of_range("Player " + name + " unknown");
	
	return *player;
}

bool Board::exists(const WallPosition & wall)
{
    // Find player by name in vector
    auto wallFound = std::find(wallsPosition_.begin(), wallsPosition_.end(), wall);

    return (wallFound != wallsPosition_.end());
}

