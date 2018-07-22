#pragma once

#include "Position.h"

class Display
{
public:
    Display() = default;
    virtual ~Display() = default;
    virtual void Board(void) = 0;
    virtual void pawn(short player_number, std::string color, std::string pawn_style) = 0;
    virtual void pawn(PawnPosition old, PawnPosition new_, short player_number, std::string color = "", std::string pawn_style = "") = 0;
    virtual void pawn(PawnPosition new_, short player_number, std::string color = "", std::string pawn_style = "") = 0;
    virtual void wall(WallPosition new_) = 0;
    virtual void refresh() = 0;
};