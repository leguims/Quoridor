#include "Game.h"
#include <iostream>
#include <string>

void Game::chooseReferee()
{
    // choose referee, means rules.
    // referee_.set(...)
}

void Game::choosePlayers()
{
    // Ask user to choose players : human, IA ?
    players_.emplace_back("Player 1", Player::Color::black, Position("e9"));
    players_.emplace_back("Player 2", Player::Color::red, Position("e1"));
}

void Game::launch()
{
    inGame = true;
}

void Game::move()
{
    // change player
    auto round = moveList_.size();
    index_player = ++index_player % players_.size();
    auto move = players_.at(index_player).getNextMove(round, board_);
    if (referee_.ValidMove(move))
    {
        if (index_player == 0)
        {
            // Save player 1 (and "none" for player 2)
            moveList_.emplace_back( std::make_pair(move, Move()) );
        }
        else
        {
            // Save player 2
            moveList_.at(round).second = move;
        }
    }
}

void Game::save()
{
    // Save to file the game.
    std::cout << "Game moves : " << std::endl;
    auto index = 1;
    for (const auto &move : moveList_)
    {
        std::cout << index << "." << move.first << "\t" << move.second << std::endl;
    }
    std::cout << "Game saved." << std::endl;
}

Game::Result Game::getResult() const
{
    if (inGame)
        return Game::Result::inProgress;
    else
        return Game::Result::draw;
}

std::string & Move::save() const
{
    std::string text;
    if (type_ == Type::pawn)
    {
        text += player_.save();
    }
    else if (type_ == Type::wall)
    {
        text += wall_.save();
    }
    return text;
}

void Move::restore(const std::string & text)
{
    type_ = type(text);

    if (type_ == Type::pawn)
    {
        player_ = PawnPosition(text);
    }
    else if (type_ == Type::wall)
    {
        wall_ = WallPosition(text);
    }
}

Move::Type Move::type(const std::string & text)
{
    if (text.size() == 2)
        type_ = Type::pawn;
    else if (text.size() == 3)
        type_ = Type::wall;
    else
        type_ = Type::none;
    return type_;
}
