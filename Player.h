#pragma once

#include "Common.h"
#include "Position.h"
#include "Board.h"

#include <vector>
#include <ostream>

class Player
{
public:
    enum class Color { red, blue, white, black };

public:
    Player() = default;
    Player(const PlayerName &name, const Color &color, const BoardPosition &startPosition);
    Player& operator=(const Player&) = default;
    virtual ~Player() = default;
    const PlayerName& name() const { return name_; }
    const BoardPosition& startPosition() const { return startPosition_; }
    const std::vector<BoardPosition>& arrivalPosition() const { return arrivalPosition_; }

    virtual Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const;
    bool haveWall() { return (walls_ > 0); }
    void removeWall() { --walls_; }
    void addArrival(const BoardPosition& arrival) { arrivalPosition_.emplace_back(arrival); }

    friend std::ostream& operator<<(std::ostream& out, const Player& player)
    {
        out << player.name_
            << " (remains " << player.walls_
            << " wall" << (player.walls_ > 1 ? "s" : "") << ")";
        return out;
    }

private:
    PlayerName name_;
    Color color_;
    unsigned int walls_;
    BoardPosition startPosition_;
    std::vector<BoardPosition> arrivalPosition_;
};

class IA_linear : public Player
{
public:
    IA_linear(const PlayerName &n, const Color &c, const BoardPosition &sp, std::vector<std::string> &moves) : Player(n, c, sp), move_list_{ moves } {}
    Move getNextMove(const unsigned int round, const Board &board, const std::vector<PawnPosition>& pawns, const std::vector<WallPosition>& walls) const;
private:
    std::vector<std::string> move_list_;
};
