#include "Game.h"
#include <iostream>
#include <string>
#include <memory>

void Game::chooseReferee()
{
    // choose referee, means rules.
    // referee_.set(...)
    referee_.setBoard(board_);
}

void Game::choosePlayers()
{
    // Ask user to choose players : human, IA ?
    players_.emplace_back("Player 1", Player::Color::black, Position("e1"));
    players_.emplace_back("Player 2", Player::Color::red, Position("e9"));
}

void Game::launch()
{
    inGame = true;
}

void Game::move()
{
    // change player
    index_player_ = ++index_player_ % players_.size();
    if (index_player_ == 0)
        round_++;

    auto move = players_.at(index_player_).getNextMove(round_, *board_);
    move.playerName(players_.at(index_player_).name());
    if (referee_.ValidMove(move))
    {
        if (index_player_ == 0)
        {
            // Save player 1 (and "none" for player 2)
            moveList_.emplace_back(std::make_pair(move, Move()));
        }
        else if (index_player_ == 1)
        {
            // Save player 2
            moveList_.at(round_ - 1).second = move;
        }
        else
        {
            throw std::out_of_range("Unexpected player " + index_player_);
        }

        // Add move to the board
        board_->add(move);

        // Remove a wall to player
        if (move.type() == Move::Type::wall)
            players_.at(index_player_).removeWall();
    }
}

//void Game::save()
//{
//    // Save to file the game.
//    std::cout << "Game moves : " << std::endl;
//    auto index = 1;
//    for (const auto &move : moveList_)
//    {
//        std::cout << index << "." << move.first << "\t" << move.second << std::endl;
//    }
//    std::cout << "Game saved." << std::endl;
//}

Game::Result Game::getResult() const
{
    if (inGame)
        return Game::Result::inProgress;
    else if (referee_.Win(players_[0]))
        return Game::Result::win1;
    else if (referee_.Win(players_[1]))
        return Game::Result::win2;
    else
        return Game::Result::draw;
}
