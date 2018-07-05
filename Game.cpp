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
    inGame = true;
}

void Game::move()
{
    if (!inGame)
        return;

    // change player
    index_player_ = ++index_player_ % players_->size();
    auto round = (index_player_ == 0 ? moveList_.size() + 1 : moveList_.size());
    //std::cout << "Round = " << round << std::endl;
    if (round >= MAX_ROUND)
    {
        inGame = false;
        return;
    }

    auto& current_player = players_->at(index_player_);
    auto current_name = current_player->name();
    auto move = current_player->getNextMove(round, *board_, referee_.getValidPawns(current_name), referee_.getValidWalls(current_name));
    move.playerName(current_name);

    if (referee_.ValidMove(move))
    {
        // Add move to the board
        board_->add(move);

        // Remove a wall to player
        if (move.type() == Move::Type::wall)
            current_player->removeWall();

        // Check if game is over
        if (referee_.Win(current_name))
            inGame = false;
    }
    else
    {
        inGame = false;
        move.setIllegal();
        referee_.illegalMove(current_name);
        if (move.type() == Move::Type::surrend)
            std::cout << "Round " << round << " : " << current_name << " does not play and surrenders, game is over." << std::endl;
        else
            std::cout << "Round " << round << " : " << current_name << " plays invalid move (" << move << "), game is over." << std::endl;
    }

    // Add move to list
    if (index_player_ == 0)
    {
        // Save player 1 (and "none" for player 2)
        moveList_.emplace_back(std::make_pair(move, Move()));
    }
    else if (index_player_ == 1)
    {
        // Save player 2
        moveList_.at(round - 1).second = move;
    }
    else
    {
        throw std::out_of_range("Unexpected player " + index_player_);
    }
}

void to_json(json& j, const Game::notation& g) {
    j = json{ { "Date", g.date },
    { "Rules", g.rules },
    { "Players", g.players },
    { "Rounds", g.rounds },
    { "Result", g.result },
    { "moves", g.moves },
    };
}

void from_json(const json& j, Game::notation& g) {
    g.date = j.at("Date").get<std::string>();
    g.rules = j.at("Rules").get<std::string>();
    g.players = j.at("Players").get<std::vector<std::string>>();
    g.result = j.at("Rounds").get<int>();
    g.result = j.at("Result").get<std::string>();
    g.moves = j.at("moves").get<std::vector<std::string>>();
}

std::string Game::save()
{
    auto _filename = filename() + ".json";

    std::ofstream o(_filename);
    json j = *this;
    o << std::setw(4) << j << std::endl;

    return _filename;
}

Game::Result Game::getResult() const
{
    if (inGame)
        return Game::Result::inProgress;
    else if (referee_.Win((*players_)[0]->name()))
        return Game::Result::win1;
    else if (referee_.Win((*players_)[1]->name()))
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

std::string Game::filename() const
{
    std::string _filename;

    for (const auto &player : *players_)
    {
        _filename += player->name() + "_";
    }
    _filename += date();

    for (const auto& forbidden : { ':', '/' })
        std::replace(_filename.begin(), _filename.end(), forbidden, '-');

    for (const auto& forbidden : { ' ', '?', '*' })
        std::replace(_filename.begin(), _filename.end(), forbidden, '_');

    return _filename;
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

Game::notation::operator Game() const
{
    Game g;

    //g.??? = date;

    //g.??? = rules;

    //g.players_ = players
    auto p1 = new Player(players.at(0), Color::black, BoardPosition("e1"));
    auto p2 = new Player(players.at(1), Color::white, BoardPosition("e9"));
    g.choosePlayers(p1, p2);

    // g.??? = result;

    // g.moveList_ = moves
    for (const auto &move : moves)
    {
        g.add(move);
    }

    return g;
}
