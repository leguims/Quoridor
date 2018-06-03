#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Position.h"
#include "Board.h"

class Player
{
public:
    enum class Color { red, blue, white, black };

public:
    Player();
    //Player(const std::shared_ptr<Board> & board);
    ~Player();
    std::vector<Position *> getNextMove(const Board &board);

private:
    std::string name_;
    Color color_;
    unsigned int walls_;
    Position startPosition_;
    //std::shared_ptr<Board> board_;

};
