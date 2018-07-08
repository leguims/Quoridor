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

class Game;

struct notation {
    std::string filename;
    std::string date;
    std::string rules;
    std::vector<std::string> players;
    int rounds;
    std::string result;
    std::vector<std::string> moves;

    notation() = default;
    notation(const std::string&);
    void save(const std::string&);
    operator Game*() const;
};

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

    operator notation() const;

public:
    Game() noexcept : board_{ std::make_shared<Board>() }, players_{}, moveList_{}, inGame_{ false }, index_move_{ -1 } {};
    ~Game() = default;
    void chooseReferee();
    void choosePlayers(Player * const p1, Player * const p2);
    void launch();
    void move(const bool replay = false);
    std::string filename() const { return filename_; }
    std::string filename();
    void filename(const std::string& f) { filename_ = f; }
    std::string save();
    void replay();
    Result getResult() const;
    const Referee& referee() const { return referee_; }
    void add(const Move &);
    void showMoves(const bool show) { board_->showMoves(show); }
    const std::vector<std::pair<Move, Move>>& moves() const { return moveList_; }

    friend std::ostream& operator<<(std::ostream& out, const Game& game)
    {
        // Save to file the game.

        out << "Date : " << game.date() << std::endl;
        out << "Filename : " << game.filename() << std::endl;

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
    std::string filename_;

    bool inGame_;
    int index_move_;
    std::string date() const;
};
