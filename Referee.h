#pragma once

#include "Position.h"
#include "Player.h"

class Referee
{
public:
    Referee() = default;
    //Referee(const std::shared_ptr<Board> & board);
    ~Referee() = default;

    bool Win(const Player & player);
    bool ValidWall(const WallPosition &);
    bool ValidPawn(const PawnPosition &);
    bool ValidMove(const Move &);
    std::vector<WallPosition>& getValidWalls();
    std::vector<PawnPosition>& getValidPawns();
    std::vector<Move>& getValidMoves();

    friend std::ostream& operator<<(std::ostream& out, const Referee& referee)
    {
        out << "Classic";
        return out;
    }

private:
    //std::shared_ptr<Board> board_;

};

