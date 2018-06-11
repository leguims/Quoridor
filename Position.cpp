#include "Position.h"


unsigned int Position::x(const unsigned int x)
{
    if ((1 <= x) && (x <= 9))
        x_ = x;
    else
        throw std::out_of_range("X should be [1;9] not " + x);
    return x_;
}

unsigned int Position::y(const unsigned int y)
{
    if ((1 <= y) && (y <= 9))
        y_ = y;
    else
        throw std::out_of_range("Y should be [1;9] not " + y);
    return y_;
}

unsigned int Position::x(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("X should have size to 1 not " + text.size());
    return x(text[0] - 'a' + 1);
}

unsigned int Position::y(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("Y should have size to 1 not " + text.size());
    return y(std::stoul(text.substr(1, 1)));
}

void Position::restore(const std::string &text)
{
    x(text.substr(0, 1));
    y(text.substr(1, 1));
}

std::string & Position::save() const
{
    std::string text;
    text += std::to_string('a' + x_ - 1);
    text += std::to_string(y_);
    return text;
}

WallPosition::Direction WallPosition::direction(const std::string & text)
{
    if (text.size() != 1)
        throw std::out_of_range("Direction should have size to 1 not " + text.size());

    if ((text.compare("h") == 0) || (text.compare("H") == 0))
        direction_ = Direction::horizontal;
    else if ((text.compare("v") == 0) || (text.compare("V") == 0))
        direction_ = Direction::vertical;
    else
        throw std::out_of_range("Direction should be ['h', 'v', 'H', 'V'] not " + text);
    return direction_;
}

void WallPosition::restore(const std::string &text)
{
    Position::restore(text);
    direction(text.substr(2, 1));
}

std::string & WallPosition::save() const
{
    auto text = Position::save();
    text += saveDirection();
    return text;
}

std::string & WallPosition::saveDirection() const
{
    std::string text;
    if (direction_ == Direction::horizontal)
        text = "h";
    if (direction_ == Direction::vertical)
        text = "v";
    return text;
}

void PawnPosition::restore(const std::string &text)
{
    Position::restore(text);
}

void PawnPosition::restore(const std::string &text, const PlayerName &playerNameText)
{
    PawnPosition::restore(text.substr(0, 2));
    playerName(playerNameText);
}

std::string & PawnPosition::save() const
{
    auto text = Position::save();
    text += playerName_;
    return text;
}
