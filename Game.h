#pragma once

#include "Player.h"

#include <vector>
#include <string>

class Game
{
public:
    enum class Result {win1, win2, draw, inProgress};

public:
    Game();
    ~Game();
    void ChooseReferee();
    void ChoosePlayers();
    Result getResult();

private:
    std::vector<Player> players_;
    std::shared_ptr<Board> board_;
    std::vector<Position *> moveList_;


};

