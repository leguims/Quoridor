#include "Referee.h"

bool Referee::Win(const Player & player) const
{
    if (player.startPosition() == BoardPosition("e1"))
    {
        auto pl = board_->position(player.name());
        const auto& arrival_e1 = { "a9", "b9", "c9", "d9", "e9", "f9", "g9", "h9", "i9", };
        for (const auto &arrival : arrival_e1)
        {
            if (pl == std::string(arrival))
                return true;
        }
    }
    else if (player.startPosition() == BoardPosition("e9"))
    {
        auto pl = board_->position(player.name());
        const auto& arrival_e9 = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1" };
        for (const auto &arrival : arrival_e9)
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
    // Check range of pawns
    // Check if wall is already in pawns or crosses the pawns
    // Check if wall close the path to arrival

    // TO DO
    // TO DO
    // TO DO
    return true;
}

bool Referee::ValidPawn(const PawnPosition &pawn) const
{
    auto name = pawn.playerName();
    auto currentPawn = board_->position(name);

    // Check range of pawns ==> Already done in 'PawnPosition'

    // Check if a pawn is already in this position
    auto pawns = board_->pawns();
    for (const auto & p : pawns)
    {
        if (p == pawn)
            return false;
    }

    // Check if pawns is a legal move
    // (next to player pawns and no wall between 2 positions)
    auto d = pawn.distance(currentPawn);
    auto wall1 = WallPosition(pawn, currentPawn);
    auto wall2 = wall1;
    if (wall1.direction() == WallPosition::Direction::horizontal)
        wall2 += Position(-1, 0);
    else if (wall1.direction() == WallPosition::Direction::vertical)
        wall2 += Position(0, -1);

    if (((d == Position(1, 0)) || (d == Position(-1, 0)) || (d == Position(0, 1)) || (d == Position(0, -1)))
        && (!board_->exists(wall1)) && (!board_->exists(wall2)))
        return true;
    // Check if pawns is a legal move (next to player pawns or need jump over another pawn)
    else
    {
        // TO DO
        // TO DO
        // TO DO
        // TO DO
        // TO DO
        // TO DO
        // TO DO
        // TO DO
        // TO DO

    }

    return false;
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

    // For each theorical pawns, check validity
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

    // For each theorical pawns, check validity
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

    // For each theorical pawns, check validity
    // Then add it to move list

    // TO DO
    // TO DO
    // TO DO

    return *list;
}
