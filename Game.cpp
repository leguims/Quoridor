#include "Game.h"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

constexpr int MAX_ROUND = 130;

Game::operator notation() const
{
    notation n;
    std::ostringstream oss;

    n.filename = filename();

    n.date = date();

    oss.str(""); oss << referee_;
    n.rules = oss.str();

    for (const auto &player : *players_)
    {
        n.players.push_back(player->name());
    }

    n.rounds = moveList_.size();

    oss.str(""); oss << getResult();
    n.result = oss.str();

    for (const auto &movePair : moveList_)
    {
        for (const auto &move : { movePair.first, movePair.second })
        {
            oss.str(""); oss << move;
            n.moves.push_back(oss.str());
        }
    }

    return n;
}

void Game::chooseReferee()
{
    // choose referee, means rules.
    // referee_.set(...)
    referee_.setBoard(board_);
}

void Game::choosePlayers(Player * const p1, Player * const p2)
{
    // Ask user to choose players : human, IA ?
    players_ = std::make_shared<std::vector<Player*>>();

    players_->push_back(p1);
    players_->push_back(p2);

    referee_.setPlayers(players_);
}

void Game::launch()
{
    referee_.launch();
    inGame_ = true;
}

void Game::move(const bool replay)
{
    if (!inGame_)
        return;

    // change player
    auto index_player = ++index_move_ % players_->size();
    auto round = 1 + index_move_ / players_->size();
    //std::cout << "Round = " << round << std::endl;
    if (!replay && (round >= MAX_ROUND))
    {
        inGame_ = false;
        return;
    }

    auto& current_player = players_->at(index_player);
    auto current_name = current_player->name();
    Move currentMove;
    // Ask move to player OR replaying  move list ?
    if (!replay)
    {
        // Ask move to player
        currentMove = current_player->getNextMove(round, *board_, referee_.getValidPawns(current_name), referee_.getValidWalls(current_name));
    }
    else
    {
        // Replaying  move list
        if (index_player == 0)
            currentMove = moveList_.at(round - 1).first;
        else
            currentMove = moveList_.at(round - 1).second;
    }
    currentMove.playerName(current_name);

    if (referee_.ValidMove(currentMove))
    {
        // Add move to the board
        board_->add(currentMove);

        // Remove a wall to player
        if (currentMove.type() == Move::Type::wall)
            current_player->removeWall();

        // Check if game is over
        if (referee_.Win(current_name))
            inGame_ = false;
    }
    else
    {
        inGame_ = false;
        currentMove.setIllegal();
        referee_.illegalMove(current_name);
        if (currentMove.type() == Move::Type::surrend)
            std::cout << "Round " << round << " : " << current_name << " does not play and surrenders, game is over." << std::endl;
        else
            std::cout << "Round " << round << " : " << current_name << " plays invalid move (" << currentMove << "), game is over." << std::endl;
    }

    // Add move to list
    if (!replay)
        add(currentMove);
}

void to_json(json& j, const notation& n) {
    j = json{ { "Date", n.date },
    { "Rules", n.rules },
    { "Players", n.players },
    { "Rounds", n.rounds },
    { "Result", n.result },
    { "moves", n.moves },
    };
}

void from_json(const json& j, notation& n) {
    n.date = j.at("Date").get<std::string>();
    n.rules = j.at("Rules").get<std::string>();
    n.players = j.at("Players").get<std::vector<std::string>>();
    n.rounds = j.at("Rounds").get<int>();
    n.result = j.at("Result").get<std::string>();
    n.moves = j.at("moves").get<std::vector<std::string>>();
}

std::string Game::filename()
{
    if (filename_.empty())
    {
        for (const auto &player : *players_)
        {
            filename_ += player->name() + "_";
        }
        filename_ += date();

        for (const auto& forbidden : { ':', '/' })
            std::replace(filename_.begin(), filename_.end(), forbidden, '-');

        for (const auto& forbidden : { ' ', '?', '*' })
            std::replace(filename_.begin(), filename_.end(), forbidden, '_');
    }

    return filename_;
}

std::string Game::save()
{
    notation n = *this;
    n.save(filename());

    return filename();
}

void Game::replay()
{
    if (!moveList_.empty())
    {
        showMoves(true);
        launch();

        while (getResult() == Result::inProgress)
            move(true);
    }
}

Game::Result Game::getResult() const
{
    if (inGame_)
        return Game::Result::inProgress;
    else if (referee_.Win(players_->at(0)->name()))
        return Game::Result::win1;
    else if (referee_.Win(players_->at(1)->name()))
        return Game::Result::win2;
    else
        return Game::Result::draw;
}

void Game::add(const Move &move)
{
    if (moveList_.empty() || moveList_.rbegin()->second.type() != Move::Type::none)
        moveList_.emplace_back(std::make_pair(move, Move()));
    else
        moveList_.rbegin()->second = move;
}

std::string Game::date() const
{
    std::ostringstream oss;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
#pragma warning(suppress : 4996) // to use std::localtime() deprecated
    oss << std::put_time(std::localtime(&in_time_t), "%d/%m/%Y %H:%M:%S");

    return oss.str();
}

notation::notation(const std::string& name)
{
    std::ifstream i(name + ".json");
    if (i.is_open())
    {
        json j;
        i >> j;

        *this = j;
    }

    filename = name;
}

void notation::save(const std::string& name)
{
    auto filename = name;

    std::ofstream o(filename + ".json");
    json j = *this;
    o << std::setw(4) << j << std::endl;
}

notation::operator Game*() const
{
    auto g = new Game();

    //g->filename = filename
    g->filename(filename);

    //g->??? = date;

    //g->??? = rules;
    g->chooseReferee();

    //g->players_ = players
    auto p1 = new Player(players.at(0), Color::black, BoardPosition("e1"));
    auto p2 = new Player(players.at(1), Color::white, BoardPosition("e9"));
    g->choosePlayers(p1, p2);

    // g->??? = result;

    // g->moveList_ = moves
    auto move_index = 0;
    for (const auto &move_txt : moves)
    {
        Move move(move_txt);
        if ((move.type() == Move::Type::pawn) || (move.type() == Move::Type::illegal_pawn))
            move.playerName(players.at(move_index % 2));
        g->add(move_txt);
        ++move_index;
    }

    return g;
}
