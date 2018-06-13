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
    Player(const PlayerName &name, const Color &color, const Position &startPosition);
    Player& operator=(const Player&) = default;
    ~Player() = default;
    Move getNextMove(const unsigned int &round, const Board &board) const ;
    const PlayerName& name() const { return name_; }
    const Position& startPosition() const { return startPosition_; }
    void removeWall() { --walls_; }

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
    Position startPosition_;

};

