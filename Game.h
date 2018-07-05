#pragma once

#include "Referee.h"
#include "Board.h"
#include "Player.h"
#include "Position.h"

#include <vector>
#include <string>
#include <map>
#include <ostream>

// for date
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time

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

    struct notation{
        std::string date;
        std::string rules;
        std::vector<std::string> players;
        int rounds;
        std::string result;
        std::vector<std::string> moves;
        operator Game() const;
    };
    operator notation() const;


public:
    Game() noexcept : board_{ std::make_shared<Board>() }, players_{}, moveList_{}, inGame{ false }, index_player_{ -1 } {};
    ~Game() = default;
    void chooseReferee();
    void choosePlayers(Player * const p1, Player * const p2);
    void launch();
    void move();
    std::string save();
    Result getResult() const;
    const Referee& referee() const { return referee_; }
    void add(const Move &);

    friend std::ostream& operator<<(std::ostream& out, const Game& game)
    {
        // Save to file the game.

        out << "Date : " << game.date() << std::endl;
        //out << "Filename : " << game.filename() << std::endl;

        out << "Rules : " << game.referee_ << std::endl;

        auto index = 1;
        for (const auto &player : *game.players_)
        {
            out << "Player " << index++ << " : " << *player << std::endl;
        }

        out << "Result : " << game.getResult() << std::endl;

        // out << "Notation : Glendenning/Alternative" << std::endl;

        out << "Game moves : " << std::endl;
        index = 1;
        for (const auto &move : game.moveList_)
        {
            out << index++ << "." << move.first << "\t" << move.second << std::endl;
        }

        return out;
    }

private:
    Referee referee_;
    std::shared_ptr<Board> board_;
    std::shared_ptr<std::vector<Player*>> players_;
    std::vector<std::pair<Move, Move>> moveList_;

    bool inGame;
    int index_player_;
    std::string date() const;
    std::string filename() const;
};
