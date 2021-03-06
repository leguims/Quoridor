#include "Referee.h"

#include <algorithm>

void Referee::setBoard(const std::shared_ptr<Board> & board)
{
    if (!board_)
    {
        board_ = board;
        board_->width(9);
        board_->heigth(9);

        board_->registerHandler([this]() {
            // Action to do when board is changing
            validPawns_.clear();
            validwalls_.clear();
            validMoves_.clear();
        });
    }
}

void Referee::setPlayers(const std::shared_ptr<std::vector<Player*>> & players)
{
    players_ = players;

    for (auto& p : *players_)
    {
        if (p->startPosition() == BoardPosition("e1"))
        {
            const auto& arrival_e1 = { "a9", "b9", "c9", "d9", "e9", "f9", "g9", "h9", "i9", };
            for (const auto &arrival : arrival_e1)
            {
                p->addArrival(std::string(arrival));
            }
        }
        else if (p->startPosition() == BoardPosition("e9"))
        {
            const auto& arrival_e9 = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1" };
            for (const auto &arrival : arrival_e9)
            {
                p->addArrival(std::string(arrival));
            }
        }
    }
}

void Referee::launch()
{
    // Initialize start position for players
    for (const auto& p : *players_)
    {
        Move move = PawnPosition(p->startPosition(), p->name(), p->color());
        board_->add(move);
    }
    illegalMove_.clear(); // No illegal move

    // Action to do when board is changing
    validPawns_.clear();
    validwalls_.clear();
    validMoves_.clear();
}

bool Referee::Win(const PlayerName & name) const
{
    if (!illegalMove_.empty() && (illegalMove_ != name))
        return true;

    auto& player = find(name);
    auto currentPosition = board_->getPawn(name);
    for (const auto& arrival : player.arrivalPosition())
    {
        if (currentPosition == arrival)
            return true;
    }

    return false;
}

void Referee::illegalMove(const PlayerName &name)
{
    illegalMove_ = name;
}

bool Referee::ValidWall(const WallPosition &wall) const
{
    // Check if player have at least 1 wall ==> done by getValidWalls()

    // Check range of walls ==> Already done in 'WallPosition'

    // Check if wall is already on board or crosses another wall
    auto crossed = wall;
    crossed.direction(wall.oppositeDirection());
    std::vector<WallPosition> badWalls;
    badWalls.push_back(wall);
    badWalls.push_back(crossed);
    if (wall.direction() == WallPosition::Direction::horizontal)
    {
        try { badWalls.push_back(wall + Position{ -1,0 }); }
        catch (std::out_of_range) {}

        try { badWalls.push_back(wall + Position{ 1,0 }); }
        catch (std::out_of_range) {}
    }
    else
    {
        try { badWalls.push_back(wall + Position{ 0,-1 }); }
        catch (std::out_of_range) {}

        try { badWalls.push_back(wall + Position{ 0,1 }); }
        catch (std::out_of_range) {}
    }
    for (const auto& w : badWalls)
    {
        if (board_->existsWall(w))
            return false;
    }

    // Check if wall close the path to arrival
    for (const auto& p : *players_)
    {
        if (!board_->findPath(wall, p->name(), p->arrivalPosition()))
            return false;
    }

    return true;
}

bool Referee::ValidPawn(const PawnPosition &pawn) const
{
    auto name = pawn.playerName();
    auto currentPawn = board_->getPawn(name);

    // Check range of pawns ==> Already done in 'PawnPosition'


    // Check if a pawn is already in this position
    if (board_->existsPawn(pawn))
        return false;


    // Check if pawns is a legal move
    auto d = currentPawn.distance(pawn);
    auto length = d.length();
    if (length == 1)
    {
        // (next to player pawns and no wall between 2 positions)
        // Verify the pawn moves to next square
        if ((d == Position(1, 0)) || (d == Position(-1, 0))
            || (d == Position(0, 1)) || (d == Position(0, -1)))
        {
            // Verify no wall block the move
            if (noBlockerWalls(currentPawn, pawn))
                return true;
        }
        else
        {
            // Pawn jumps over another pawn, but bounces on wall behind it.
            std::vector<std::tuple<Position, Position, WallPosition>> tests;
            //std::vector<     distance    , opponent,     wall    >> tests;
            //{
            //    distance,
            //    opponent,
            //    wall,
            //},
            try {
                tests.emplace_back(
                    Position(1, 1),
                    (Position)currentPawn + Position(0, -1),
                    WallPosition(currentPawn.x(), currentPawn.y() - 2, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, 1),
                    (Position)currentPawn + Position(0, -1),
                    WallPosition(currentPawn.x() - 1, currentPawn.y() - 2, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, 1),
                    (Position)currentPawn + Position(-1, 0),
                    WallPosition(currentPawn.x() - 2, currentPawn.y(), WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, 1),
                    (Position)currentPawn + Position(-1, 0),
                    WallPosition(currentPawn.x() - 2, currentPawn.y() - 1, WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, -1),
                    (Position)currentPawn + Position(-1, 0),
                    WallPosition(currentPawn.x() - 2, currentPawn.y(), WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, -1),
                    (Position)currentPawn + Position(-1, 0),
                    WallPosition(currentPawn.x() - 2, currentPawn.y() - 1, WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, -1),
                    (Position)currentPawn + Position(0, 1),
                    WallPosition(currentPawn.x(), currentPawn.y() + 1, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(1, -1),
                    (Position)currentPawn + Position(0, 1),
                    WallPosition(currentPawn.x() - 1, currentPawn.y() + 1, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, -1),
                    (Position)currentPawn + Position(0, 1),
                    WallPosition(currentPawn.x(), currentPawn.y() + 1, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, -1),
                    (Position)currentPawn + Position(0, 1),
                    WallPosition(currentPawn.x() - 1, currentPawn.y() + 1, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, -1),
                    (Position)currentPawn + Position(1, 0),
                    WallPosition(currentPawn.x() + 1, currentPawn.y(), WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, -1),
                    (Position)currentPawn + Position(1, 0),
                    WallPosition(currentPawn.x() + 1, currentPawn.y() - 1, WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, 1),
                    (Position)currentPawn + Position(1, 0),
                    WallPosition(currentPawn.x() + 1, currentPawn.y(), WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, 1),
                    (Position)currentPawn + Position(1, 0),
                    WallPosition(currentPawn.x() + 1, currentPawn.y() - 1, WallPosition::Direction::vertical)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, 1),
                    (Position)currentPawn + Position(0, -1),
                    WallPosition(currentPawn.x(), currentPawn.y() - 2, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 
            try {
                tests.emplace_back(
                    Position(-1, 1),
                    (Position)currentPawn + Position(0, -1),
                    WallPosition(currentPawn.x() - 1, currentPawn.y() - 2, WallPosition::Direction::horizontal)
                );
            }
            catch (const std::out_of_range &) {} // one element is out of board, ignore the test 

            Position distance;
            Position opponent;
            WallPosition wall;
            for (const auto & t : tests)
            {
                std::tie(distance, opponent, wall) = t;
                if (d == distance)
                {
                    // Opponent at the right place + 1 Wall behind opponent + NO wall between opponent and destination
                    if (board_->existsPawn(opponent)
                        && board_->existsWall(wall)
                        && noBlockerWalls(opponent, pawn))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if (length == 2)
    {
        // Pawn jumps over another pawn
        // Define where have to be the other pawn
        auto pawnJumped = ((Position)pawn + (Position)currentPawn) / 2;

        // Verify that pawnJumped exists on board
        if (!board_->existsPawn(pawnJumped))
            return false;

        // Verify that pawn moves 2 squares ahead
        if ((d == Position(2, 0)) || (d == Position(-2, 0))
            || (d == Position(0, 2)) || (d == Position(0, -2)))
        {
            // Verify that no wall can block the jump
            if (noBlockerWalls(currentPawn, pawn))
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
    case Move::Type::illegal_pawn:
    case Move::Type::illegal_wall:
    case Move::Type::surrend:
    case Move::Type::none:
        return false;
    }
    throw std::out_of_range("Unknown move to verify");
}

const std::vector<WallPosition>& Referee::getValidWalls(const PlayerName& name)
{
    auto& player = find(name);
    if (validwalls_.empty() && player.haveWall())
    {
        WallPosition wall;
        // For each theorical walls, check validity
        for (const auto& x : { 1,2,3,4,5,6,7,8 })
        {
            wall.x(x);
            for (const auto& y : { 1,2,3,4,5,6,7,8 })
            {
                wall.y(y);
                for (const auto& d : { WallPosition::Direction::horizontal,WallPosition::Direction::vertical })
                {
                    wall.direction(d);
                    // Then add it to wall list
                    if (ValidWall(wall))
                        validwalls_.push_back(wall);
                }
            }
        }
    }

    return validwalls_;
}

const std::vector<PawnPosition>& Referee::getValidPawns(const PlayerName & player)
{
    if (validPawns_.empty())
    {
        PawnPosition pawn{ 1, 1, player };
        // For each theorical pawns, check validity
        for (const auto& x : { 1,2,3,4,5,6,7,8,9 })
        {
            pawn.x(x);
            for (const auto& y : { 1,2,3,4,5,6,7,8,9 })
            {
                pawn.y(y);
                // Then add it to pawn list
                if (ValidPawn(pawn))
                    validPawns_.push_back(pawn);
            }
        }
    }

    return validPawns_;
}

std::vector<WallPosition> Referee::findBlockerWalls(const BoardPosition &current, const BoardPosition &next) const
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
        for (const auto primary : { true, false })
        {
            try
            {
                auto wall = WallPosition(next, current, primary);
                list.emplace_back(wall);
            }
            catch (const std::out_of_range &) {} // Wall is out of board : ignore it
        }
    }
    else if (length > 1)
    {
        // middle pawn position
        auto middle = PawnPosition(((Position)current + (Position)next) / 2, "");

        for (const auto& wall : findBlockerWalls(current, middle))
            list.emplace_back(wall);

        for (const auto& wall : findBlockerWalls(middle, next))
            list.emplace_back(wall);
    }
    else
    {
        std::ostringstream oss;
        oss << "Move between " << current << " and " << next << " must be vertical/horizontal line.";
        throw std::out_of_range(oss.str());
    }

    return list;
}

bool Referee::noBlockerWalls(const BoardPosition &p1, const BoardPosition &p2) const
{
    for (const auto& wall : findBlockerWalls(p1, p2))
    {
        if (board_->existsWall(wall))
            return false;
    }
    return true;
}

Player& Referee::find(const PlayerName& name) const
{
    // Find player by name in vector
    auto player = std::find_if(players_->begin(), players_->end(),
        [name](const Player *p)->bool { return (p->name() == name); });

    if (player == players_->end())
        throw std::out_of_range("Player " + name + " unknown");

    return *(*player); // *(*iter)
}
