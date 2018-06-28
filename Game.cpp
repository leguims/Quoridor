#include "Game.h"
#include <iostream>
#include <string>
#include <memory>

constexpr int MAX_ROUND = 130;

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
    else if (referee_.Win((*players_)[0]->name()))
        return Game::Result::win1;
    else if (referee_.Win((*players_)[1]->name()))
        return Game::Result::win2;
    else
        return Game::Result::draw;
}
