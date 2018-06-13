#include "Referee.h"

bool Referee::Win(const Player & player) const
{
    //if(player.startPosition() == Position("e1"))
    return true;
}

bool Referee::ValidWall(const WallPosition &) const
{
    return true;
}

bool Referee::ValidPawn(const PawnPosition &) const
{
    return true;
}

bool Referee::ValidMove(const Move &)const
{
    return true;
}
