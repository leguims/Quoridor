#include "Referee.h"

bool Referee::Win(const Player & player) const
{
    // TO DO
    // TO DO
    // TO DO
    //if(player.startPosition() == Position("e1"))
    return true;
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

bool Referee::ValidMove(const Move &)const
{
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
