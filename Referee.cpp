#include "Referee.h"

void Referee::setBoard(const std::shared_ptr<Board>& board)
{
    board_ = board;
    board_->registerHandler([this](){
        // Action to do when board is changing
        validPawns_.clear();
        validwalls_.clear();
        validMoves_.clear();
    });
}

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
    auto d = pawn.distance(currentPawn);
    auto length = d.length();

    if (length == 1)
    {
        // (next to player pawns and no wall between 2 positions)
        // Pawn moves to next case

        // Verify the pawn moves to next square
        if ((d == Position(1, 0)) || (d == Position(-1, 0))
            || (d == Position(0, 1)) || (d == Position(0, -1)))
        {
            for (const auto& wall : findBlockerWalls(currentPawn, pawn))
            {
                if (board_->exists(wall))
                    return false;
            }
            return true;
        }
        else
        {
            // Pawn jumps over another pawn, but bounces on wall behind it.
            if ((d == Position(1, 1)))
            {
                // TO DO
                // TO DO
            }
            else if ((d == Position(1, -1)))
            {
                // TO DO
                // TO DO
            }
            else if ((d == Position(-1, -1)))
            {
                // TO DO
                // TO DO
            }
            else if ((d == Position(-1, 1)))
            {
                // TO DO
                // TO DO
            }

            // TO DO
            // TO DO
            // TO DO
            // TO DO
            // TO DO
            // TO DO
            // TO DO
        }
    }
    else if (length == 2)
    {
        // Pawn jumps over another pawn
        // Where have to be the other pawn
        auto opponent = ((Position)pawn + (Position)currentPawn) / 2;

        // Verify that opponent is in the right position
        for (const auto& p : board_->pawns())
        {
            if (((Position)p != (Position)currentPawn) && ((Position)p != opponent))
                return false;
        }

        // Verify that pawn moves 2 squares ahead
        if ((d == Position(2, 0)) || (d == Position(-2, 0))
            || (d == Position(0, 2)) || (d == Position(0, -2)))
        {
            for (const auto& wall : findBlockerWalls(currentPawn, pawn))
            {
                if (board_->exists(wall))
                    return false;
            }
            return true;
        }
    }

    return false;
}

bool Referee::ValidMove(const Move &move)const
{
    switch (move.type())
    {
    case Move::Type::pawn:
        return ValidPawn(move.pawn());
    case Move::Type::wall:
        return ValidWall(move.wall());
    case Move::Type::none:
        return false;
    }
    throw std::out_of_range("Unknown move to verify");
}

const std::vector<WallPosition>& Referee::getValidWalls()
{
    if (validwalls_.empty())
    {
        // For each theorical pawns, check validity
        // Then add it to pawn list

        // TO DO
        // TO DO
        // TO DO
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

    return validwalls_;
}

const std::vector<PawnPosition>& Referee::getValidPawns()
{
    if (validPawns_.empty())
    {
        // For each theorical pawns, check validity
        // Then add it to pawn list

        // TO DO
        // TO DO
        // TO DO
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

    return validPawns_;
}

const std::vector<Move>& Referee::getValidMoves()
{
    if (validMoves_.empty())
    {
        for (const auto & w : getValidWalls())
            validMoves_.emplace_back(w);

        for (const auto & p : getValidPawns())
            validMoves_.emplace_back(p);
    }
    return validMoves_;
}

std::vector<WallPosition> Referee::findBlockerWalls(const PawnPosition &current, const PawnPosition &next) const
{
    std::vector<WallPosition> list;

    auto d = next.distance(current);
    auto length = d.length();

    if ((length == 1)
        && ((d == Position(1, 0)) || (d == Position(-1, 0))
            || (d == Position(0, 1)) || (d == Position(0, -1))))
    {
        // Pawn moves to next case
        // Find each possible walls between the 2 pawns
        auto wall1 = WallPosition(next, current);
        auto wall2 = wall1;
        if (wall1.direction() == WallPosition::Direction::horizontal)
            wall2 += Position(-1, 0);
        else if (wall1.direction() == WallPosition::Direction::vertical)
            wall2 += Position(0, -1);

        list.emplace_back(wall1);
        list.emplace_back(wall2);
    }
    else if (length > 1)
    {
        // middle position
        auto middle = PawnPosition(((Position)current + (Position)next) / 2, "");

        for (const auto& wall : findBlockerWalls(current, middle))
            list.emplace_back(wall);

        for (const auto& wall : findBlockerWalls(middle, next))
            list.emplace_back(wall);
    }
    else
    {
        std::ostringstream oss;
        oss << "Move between " << current << " and " << next << " must be as a line.";
        throw std::out_of_range(oss.str());
    }

    return list;
}
