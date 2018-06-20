#pragma once

#include "Position.h"

#include <vector>
#include <map>
#include <functional>

class Board
{
public:
    using handlerCB = std::function<void(void)>;

    Board();
    ~Board();

    void add(const Move & move);
    void registerHandler(const handlerCB &&);

    const PawnPosition& getPawn(const PlayerName& name) const;
    const std::vector<PawnPosition>& pawns() const { return pawnsPosition_; }
    const std::vector<WallPosition>& walls() const { return wallsPosition_; }
    bool existsWall(const WallPosition& wall) const;
    bool existsPawn(const Position& pawnPos) const;


    // To remove ?
    //const std::vector<PawnPosition>& playersPosition() { return pawnsPosition_; }
    //std::vector<WallPosition>& wallsPosition() { return wallsPosition_; }

private:
    unsigned int width_;
    unsigned int heigth_;
    std::vector<PawnPosition> pawnsPosition_;
    std::vector<WallPosition> wallsPosition_;
    std::vector<handlerCB> handlers_;

    void add(const PawnPosition & pawn);
    void add(const WallPosition & wall);
    void callHandlers();
    PawnPosition& getPawn_(const PlayerName& name);
};

