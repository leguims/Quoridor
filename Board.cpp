#include "Board.h"


Board::Board() : width_{ 0 }, heigth_{ 0 }
{
}

Board::~Board()
{
}

void Board::add(const PawnPosition & pawn)
{
	// Find player on board
	auto player = playersPosition_.find(pawn.playerName());
	if (player == playersPosition_.end())
	{
		// New pawn : Add it
		playersPosition_.emplace(pawn.playerName(), pawn);
	}
	else
	{
		// Existing one : replace it
		player->second = pawn;
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

PawnPosition & Board::position(const PlayerName & name)
{
	auto player = playersPosition_.find(name);
	if(player == playersPosition_.end())
		throw std::out_of_range("Player " + name + " unknown");
	
	return player->second;
}
