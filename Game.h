#pragma once

#include "Referee.h"
#include "Board.h"
#include "Player.h"
#include "Position.h"

#include <vector>
#include <string>
#include <map>

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
    Game() : board_{}, players_{}, moveList_{}, inGame{ false }, index_player{ 0 } {};
    ~Game() = default;
    void chooseReferee();
    void choosePlayers();
    void launch();
    void move();
    void save();
    Result getResult() const;

private:
    Referee referee_;
    Board board_;
    std::vector<Player> players_;
    std::vector<std::pair<Move, Move>> moveList_;

    bool inGame;
    unsigned short index_player;
};

