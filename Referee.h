#pragma once

#include "Position.h"
#include "Player.h"

#include <memory>

class Referee
{
public:
    Referee() = default;
    //Referee(const std::shared_ptr<Board> & board);
    void setBoard(const std::shared_ptr<Board> & board) { board_ = board; }
    ~Referee() = default;

    bool Win(const Player & player) const;
    bool ValidWall(const WallPosition &) const;
    bool ValidPawn(const PawnPosition &) const;
    bool ValidMove(const Move &) const;
    std::vector<WallPosition>& getValidWalls() const;
    std::vector<PawnPosition>& getValidPawns() const;
    std::vector<Move>& getValidMoves() const;

    friend std::ostream& operator<<(std::ostream& out, const Referee& referee)
    {
        out << "Classic";
        return out;
    }

private:
    std::shared_ptr<Board> board_;

};

