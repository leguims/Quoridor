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
    try
    {
        // Find player on board
        auto& player = getPawn_(pawn.playerName());

        // Existing one : replace it
        player = pawn;
    }
    catch (const std::out_of_range &)
    {
        // New pawn : Add it
        pawnsPosition_.emplace_back(pawn);
    }
}

void Board::add(const WallPosition & wall)
{
    wallsPosition_.emplace_back(wall);
}

void Board::callHandlers()
{
    for (const auto& h : handlers_)
        h();
}

void Board::add(const Move & move)
{
    switch (move.type())
    {
    case Move::Type::pawn:
        add(move.pawn());
        callHandlers();
        break;
    case Move::Type::wall:
        add(move.wall());
        callHandlers();
        break;
    case Move::Type::none:
        break;
    default:
        throw std::out_of_range("Unknown move to add to the board");
    }
}

void Board::registerHandler(const handlerCB &&handler)
{
    handlers_.push_back(std::move(handler));
}

PawnPosition & Board::getPawn_(const PlayerName & name)
{
    // Find player by name in vector
    auto player = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(),
        [name](const PawnPosition &p)->bool { return (p.playerName() == name); });

    if (player == pawnsPosition_.end())
        throw std::out_of_range("Player " + name + " unknown");

    return *player;
}

const PawnPosition & Board::getPawn(const PlayerName & name) const
{
    // Find player by name in vector
    auto player = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(),
        [name](const PawnPosition &p)->bool { return (p.playerName() == name); });

    if (player == pawnsPosition_.end())
        throw std::out_of_range("Player " + name + " unknown");

    return *player;
}

bool Board::existsWall(const WallPosition & wall) const
{
    // Find player by name in vector
    auto wallFound = std::find(wallsPosition_.begin(), wallsPosition_.end(), wall);

    return (wallFound != wallsPosition_.end());
}

bool Board::existsPawn(const Position & pawnPos) const
{
    // Find player by its getPawn in vector
    auto pawnFound = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(),
        [pawnPos](const PawnPosition &p)->bool { return ((Position)p == pawnPos); });

    return (pawnFound != pawnsPosition_.end());
}

