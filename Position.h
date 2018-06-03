#pragma once

//#include "Player.h"

class Position
{
public:
    Position();
    ~Position();
    unsigned int x;
    unsigned int y;
};

class PlayerPosition : Position
{
public:
    PlayerPosition() {};
    //PlayerPosition(const std::shared_ptr<Player> &player) : player_(player) {};

private:
    //std::shared_ptr<Player> player_;
};


class WallPosition : Position
{
public:
    enum class Direction { horizontal, vertical };
    WallPosition(Direction direction): direction_(direction) {};

private:
    Direction direction_;
};

