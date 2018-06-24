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

public:
    Game() noexcept : board_{ std::make_shared<Board>() }, players_{}, moveList_{}, inGame{ false }, index_player_{ -1 } {};
    ~Game() = default;
    void chooseReferee();
    void choosePlayers();
    void launch();
    void move();
    //void save();
    Result getResult() const;
	const Referee& referee() const { return referee_; }

    friend std::ostream& operator<<(std::ostream& out, const Game& game)
    {
        // Save to file the game.

        auto now = std::chrono::system_clock::now(); 
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
#pragma warning(suppress : 4996) // to use std::localtime() deprecated
        out << "Date : " << std::put_time(std::localtime(&in_time_t), "%d/%m/%Y %H:%M:%S") << std::endl;

        out << "Rules : " << game.referee_ << std::endl;

        auto index = 1;
        for (const auto &player : *game.players_)
        {
            out << "Player " << index ++ << " : " << player << std::endl;
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
    std::shared_ptr<std::vector<Player>> players_;
    std::vector<std::pair<Move, Move>> moveList_;

    bool inGame;
    int index_player_;
};

