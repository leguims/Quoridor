#include "Referee.h"

bool Referee::Win(const Player & player) const
{
	if (player.startPosition() == Position("e1"))
	{
		auto pl = board_->position(player.name());
		for (const auto &arrival : { "a9", "b9", "c9", "d9", "e9", "f9", "g9", "h9", "i9", })
		{
			if (pl == std::string(arrival))
				return true;
		}
	}
	else if (player.startPosition() == Position("e9"))
	{
		auto pl = board_->position(player.name());
		for (const auto &arrival : { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", })
		{
			if (pl == std::string(arrival))
				return true;
		}
	}
    return false;
}

bool Referee::ValidWall(const WallPosition &) const
{
    // Check if player have at least 1 wall
    // Check range of position
    // Check if wall is already in position or crosses the position
    // Check if wall close the path to arrival

    // TO DO
    // TO DO
    // TO DO
    return true;
}

bool Referee::ValidPawn(const PawnPosition &) const
{
    // Check range of position
    // Check if a pawn is already in position
    // Check if position is a legal move (next to player position or need jump over another pawn)

    // TO DO
    // TO DO
    // TO DO
    return true;
}

bool Referee::ValidMove(const Move &move)const
{
	if (move.type() == Move::Type::pawn)
		ValidPawn(move.pawn());
	else if (move.type() == Move::Type::wall)
		ValidWall(move.wall());
	else if (move.type() == Move::Type::none)
		return false;
	else
		throw std::out_of_range("Unknown move to verify");
	// Check type of move
    // Check wall move
    // Check pawn move

    // TO DO
    // TO DO
    // TO DO
    return true;
}

std::vector<WallPosition>& Referee::getValidWalls() const
{
    // Allocate empty wall list
    std::vector<WallPosition> *list = new std::vector<WallPosition>;

    // For each theorical position, check validity
    // Then add it to wall list

    // TO DO
    // TO DO
    // TO DO

    return *list;
}

std::vector<PawnPosition>& Referee::getValidPawns() const
{
    // Allocate empty pawn list
    std::vector<PawnPosition> *list = new std::vector<PawnPosition>;

    // For each theorical position, check validity
    // Then add it to pawn list

    // TO DO
    // TO DO
    // TO DO

    return *list;
}

std::vector<Move>& Referee::getValidMoves() const
{
    // Allocate empty move list
    std::vector<Move> *list = new std::vector<Move>;

    // For each theorical position, check validity
    // Then add it to move list

    // TO DO
    // TO DO
    // TO DO

    return *list;
}
