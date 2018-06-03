#pragma once

#include "Position.h"
#include "Player.h"

class Referee
{
public:
    Referee();
    //Referee(const std::shared_ptr<Board> & board);
    ~Referee();

    bool Win(const Player & player);
    bool ValidPutWall();
    bool ValidMovePawn();
    std::vector<WallPosition *> getValidWalls();
    std::vector<PlayerPosition *> getValidMoves();
    std::vector<Position *> getValidPlays();

private:
    //std::shared_ptr<Board> board_;

};

