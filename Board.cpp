#include "Board.h"

#include <algorithm>
#include <iostream>

Board::Board() noexcept : width_{ 0 }, heigth_{ 0 }
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
        auto& player = getPawnNonConst(pawn.playerName());

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

PawnPosition& Board::getPawnNonConst(const PlayerName& name)
{
    // Find player by name in vector
    auto player = std::find_if(pawnsPosition_.begin(), pawnsPosition_.end(),
        [name](const PawnPosition &p)->bool { return (p.playerName() == name); });

    if (player == pawnsPosition_.end())
        throw std::out_of_range("Player " + name + " unknown");

    return *player;
}

const PawnPosition & Board::getPawn(const PlayerName& name) const
{
    return (const_cast<Board*>(this))->getPawnNonConst(name);
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

bool Board::findPath(const WallPosition& wall, const PlayerName& name, const std::vector<BoardPosition>& arrival) const
{
    AStarBoard generator{ width_, heigth_, wallsPosition_ };

    auto current = static_cast<BoardPosition>(getPawn(name));
    return generator.findPath(wall, current, arrival);
}

AStarBoard::AStarBoard(const int width, const int heigth, const std::vector<WallPosition>& walls) : generator_{}
{
    // World = [ 0 ; WorldSize-1 ] ===> width = WorldSize
    generator_.setWorldSize({ 2 * width - 1, 2 * heigth - 1 });

    generator_.setHeuristic(AStar::Heuristic::manhattan); // x3 Faster
    //generator_.setHeuristic(AStar::Heuristic::euclidean);
    //generator_.setHeuristic(AStar::Heuristic::octagonal);

    // generator.setDiagonalMovement(false); // false by default

    // Generate board
    for (const auto& w : walls)
        add(w);
}

bool AStarBoard::findPath(const WallPosition& wall, const BoardPosition& current, const std::vector<BoardPosition>& arrivals)
{
    int cpt(0); bool log = false;

    //Add new wall to check
    add(wall);

    for (const auto & arrival : arrivals)
    {
        auto source = position(current);
        auto target = position(arrival);
        auto path = generator_.findPath(source, target);
        // A path exists to win and ended on arrival position
        if (!path.empty() && (*path.begin() == target) && (*path.rbegin() == source))
        {
            if (log)
            {
                std::cout << cpt++ << " - findPath( (" << source.x << " " << source.y << "), "
                    << "(" << target.x << " " << target.y << ") :\n";
                for (auto& coordinate : path) {
                    std::cout << "(" << coordinate.x << " " << coordinate.y << "), ";
                }
                std::cout << " *END*\n";
            }
            //Remove new wall checked
            remove(wall);
            return true;
        }
    }

    //Remove new wall checked
    remove(wall);
    // No path to win
    return false;
}

void AStarBoard::add(const WallPosition &wall)
{
    for (const auto v : position(wall))
    {
        try
        {
            generator_.addCollision(v);
        }
        catch (const std::out_of_range &) {}
    }
}

void AStarBoard::remove(const WallPosition &wall)
{
    for (const auto v : position(wall))
    {
        try
        {
            generator_.removeCollision(v);
        }
        catch (const std::out_of_range &) {}
    }
}

AStar::Vec2i AStarBoard::position(const BoardPosition &bp)
{
    return AStar::Vec2i({ 2 * bp.x() - 2, 2 * bp.y() - 2 });
}

std::vector<AStar::Vec2i> AStarBoard::position(const WallPosition &wp)
{
    std::vector<AStar::Vec2i> list;
    if (wp.direction() == WallPosition::Direction::horizontal)
    {
        list.push_back({ 2 * wp.x() - 2 + 0, 2 * wp.y() - 1 });
        list.push_back({ 2 * wp.x() - 2 + 1, 2 * wp.y() - 1 });
        list.push_back({ 2 * wp.x() - 2 + 2, 2 * wp.y() - 1 });
        list.push_back({ 2 * wp.x() - 2 + 3, 2 * wp.y() - 1 });
    }
    else
    {
        list.push_back({ 2 * wp.x() - 1, 2 * wp.y() - 2 + 0 });
        list.push_back({ 2 * wp.x() - 1, 2 * wp.y() - 2 + 1 });
        list.push_back({ 2 * wp.x() - 1, 2 * wp.y() - 2 + 2 });
        list.push_back({ 2 * wp.x() - 1, 2 * wp.y() - 2 + 3 });
    }
    return list;
}

