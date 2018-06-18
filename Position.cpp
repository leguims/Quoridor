#include "Position.h"


const int BoardPosition::x(const int x)
{
    if ((1 <= x) && (x <= 9))
        return Position::x(x);
    else
        throw std::out_of_range("X should be [1;9] not " + x);
}

const int BoardPosition::y(const int y)
{
    if ((1 <= y) && (y <= 9))
        return Position::y(y);
    else
        throw std::out_of_range("Y should be [1;9] not " + y);
}

const int BoardPosition::x(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("X should have size to 1 not " + text.size());
    return x(text[0] - 'a' + 1);
}

const int BoardPosition::y(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("Y should have size to 1 not " + text.size());
    return y(std::stoul(text.substr(0, 1)));
}

void BoardPosition::restore(const std::string &text)
{
    x(text.substr(0, 1));
    y(text.substr(1, 1));
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
    BoardPosition::restore(text);
    direction(text.substr(2, 1));
}

void PawnPosition::restore(const std::string &text)
{
    BoardPosition::restore(text);
}

void PawnPosition::restore(const std::string &text, const PlayerName &playerNameText)
{
    PawnPosition::restore(text.substr(0, 2));
    playerName(playerNameText);
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

const Move::Type& Move::type(const std::string & text)
{
    if (text.size() == 2)
        type_ = Type::pawn;
    else if (text.size() == 3)
        type_ = Type::wall;
    else
        type_ = Type::none;
    return type_;
}
