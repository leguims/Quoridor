#include "Position.h"


const int BoardPosition::x(const int x)
{
    if (validX(x))
        return Position::x(x);
    else
        throw std::out_of_range("X-Board should be [1;9] not " + x);
}

const int BoardPosition::y(const int y)
{
    if (validY(y))
        return Position::y(y);
    else
        throw std::out_of_range("Y-Board should be [1;9] not " + y);
}

const int BoardPosition::x(const std::string& text)
{
    return x(Position::x(text));
}

const int BoardPosition::y(const std::string& text)
{
    return y(Position::y(text));
}

bool BoardPosition::validX(const int x)
{
    return ((1 <= x) && (x <= 9));
}

bool BoardPosition::validY(const int y)
{
    return ((1 <= y) && (y <= 9));
}

void BoardPosition::restore(const std::string &text)
{
    x(text.substr(0, 1));
    y(text.substr(1, 1));
}

WallPosition::WallPosition(const BoardPosition & a, const BoardPosition & b): direction_(Direction::vertical)
{
    auto d = a.distance(b);
    if (d == Position(1, 0))
    {
        try
        {
            // b is the wall position
            x(b.x());
            y(b.y());
            direction(Direction::vertical);
        }
        catch (const std::out_of_range &)
        {
            // b is illegal wall position => take b + (0, -1)
            x(b.x());
            y(b.y() - 1);
            direction(Direction::vertical);
            throw;
        }
    }
    else if (d == Position(-1, 0))
    {
        try
        {
            // a is the wall position
            x(a.x());
            y(a.y());
            direction(Direction::vertical);
        }
        catch (const std::out_of_range &)
        {
            // a is illegal wall position => take b + (0, -1)
            x(a.x());
            y(a.y()-1);
            direction(Direction::vertical);
            throw;
        }
    }
    else if (d == Position(0, 1))
    {
        try
        {
            // b is the wall position
            x(b.x());
            y(b.y());
            direction(Direction::horizontal);
        }
        catch (const std::out_of_range &)
        {
            // b is illegal wall position => take b + (0, -1)
            x(b.x());
            y(b.y() - 1);
            direction(Direction::horizontal);
            throw;
        }
    }
    else if (d == Position(0, -1))
    {
        try
        {
            // a is the wall position
            x(a.x());
            y(a.y());
            direction(Direction::horizontal);
        }
        catch (const std::out_of_range &)
        {
            // a is illegal wall position => take b + (0, -1)
            x(a.x());
            y(a.y() - 1);
            direction(Direction::horizontal);
            throw;
        }
    }
    else
    {
        std::ostringstream oss;
        oss << "Cannot create a wall position with board positions : " << a << " and " << b;
        throw std::out_of_range(oss.str());
    }
}

const int WallPosition::x(const int x)
{
    if (validX(x))
        return BoardPosition::x(x);
    else
        throw std::out_of_range("X-Wall should be [1;8] not " + x);
}

const int WallPosition::y(const int y)
{
    if (validY(y))
        return BoardPosition::y(y);
    else
        throw std::out_of_range("Y-Wall should be [1;8] not " + y);
}

const int WallPosition::x(const std::string & text)
{
    if (text.size() != 1)
        throw std::out_of_range("X should have size to 1 not " + text.size());
    return x(text[0] - 'a' + 1);
}

const int WallPosition::y(const std::string & text)
{
    if (text.size() != 1)
        throw std::out_of_range("Y should have size to 1 not " + text.size());
    return y(std::stoul(text.substr(0, 1)));
}

const WallPosition::Direction WallPosition::direction(const std::string & text)
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

WallPosition WallPosition::shiftedWallPosition(Position position) const
{
    WallPosition new_ = *this;
    new_ += position;
    return new_;
}

void WallPosition::restore(const std::string &text)
{
    BoardPosition::restore(text);
    direction(text.substr(2, 1));
}

bool WallPosition::validX(const int x)
{
    return ((1 <= x) && (x <= 8));
}

bool WallPosition::validY(const int y)
{
    return ((1 <= y) && (y <= 8));
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

    switch (type_)
    {
    case Move::Type::pawn:
        player_ = PawnPosition(text);
        break;
    case Move::Type::wall:
        wall_ = WallPosition(text);
        break;
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

void Move::setIllegal()
{
    switch (type_)
    {
    case Move::Type::pawn:
        type_ = Move::Type::illegal_pawn;
        break;
    case Move::Type::wall:
        type_ = Move::Type::illegal_wall;
        break;
    case Move::Type::none:
        type_ = Move::Type::surrend;
        break;
    }
}

const int Position::x(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("X should have size to 1 not " + text.size());
    return Position::x(text[0] - 'a' + 1);
}

const int Position::y(const std::string& text)
{
    if (text.size() != 1)
        throw std::out_of_range("Y should have size to 1 not " + text.size());
    return Position::y(static_cast<int>(std::stoul(text.substr(0, 1))));
}

Position Position::distance(const Position & position) const
{
    return Position(x_ - position.x_, y_ - position.y_);
}
