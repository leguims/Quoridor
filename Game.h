#pragma once

#include "Board.h"
#include "Player.h"
#include "Position.h"

#include <vector>
#include <string>
#include <map>

class Move;

class Game
{
public:
    enum class Result { win1, win2, draw, inProgress };
    friend std::ostream& operator<<(std::ostream& out, const Result& result)
    {
        std::map<Result, std::string> enumerator{
            { Result::win1, "Player 1 wins" },
            { Result::win2, "Player 2 wins" },
            { Result::draw, "Draw" },
            { Result::inProgress, "Game is in progress..." }
        };

        try {
            out << enumerator.at(result);
        }
        catch (const std::out_of_range &) {
            out << "Result???";
        }
        return out;
    }


public:
    Game() = default;
    ~Game() = default;
    void chooseReferee();
    void choosePlayers();
    void launch();
    Result getResult() const;

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
    Move(const std::string &text) : type_{ type(text) }, player_{ type_ == Type::pawn ? PlayerPosition(text) : PlayerPosition() }, wall_{ type_ == Type::wall ? WallPosition(text) : WallPosition() } {}

    std::string & save() const;
    void restore(const std::string &move);
    Type type(const std::string &text);


private:
    Type type_;
    PlayerPosition player_;
    WallPosition wall_;
};
