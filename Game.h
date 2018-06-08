#pragma once

#include "Board.h"
#include "Player.h"
#include "Position.h"

#include <vector>
#include <string>

class Move;

class Game
{
public:
    enum class Result { win1, win2, draw, inProgress };

public:
    Game() = default;
    ~Game() = default;
    void ChooseReferee();
    void ChoosePlayers();
    Result getResult();

private:
    Board board_;
    std::vector<Player> players_;
    std::vector<std::pair<Move, Move>> moveList_;
};

class Move
{
public:
    enum class Type { pawn, wall, none };
    Move() = delete;
    Move(const PlayerPosition &player) : type_{ Type::pawn }, player_{ player }, wall_{} {}
    Move(const WallPosition &wall) : type_{ Type::wall }, player_{}, wall_{ wall } {}
    Move(const std::string &text) : type_{ type(text) }, player_{ type_==Type::pawn ? PlayerPosition(text) : PlayerPosition() }, wall_{ type_ == Type::wall ? WallPosition(text) : WallPosition() } {}

    std::string & save() const;
    void restore(const std::string &move);
    Type type(const std::string &text);


private:
    Type type_;
    PlayerPosition player_;
    WallPosition wall_;
};
