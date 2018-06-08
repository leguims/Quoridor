#include "Game.h"
#include <string>

void Game::chooseReferee()
{
}

void Game::choosePlayers()
{
}

void Game::launch()
{
}

Game::Result Game::getResult() const
{
    return Game::Result::inProgress;
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
        player_ = PlayerPosition(text);
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
