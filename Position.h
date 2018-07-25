#pragma once

#include "Common.h"
#include <ostream>
#include <tuple>
#include <sstream>
#include <cmath>

class Position
{
public:
    Position() noexcept : x_(0), y_(0) {}
    Position(const int x_value, const int y_value) : x_(x_value), y_(y_value) {}
    Position(const std::string& position) : Position(x(position.substr(0, 1)), y(position.substr(1, 1))) {}
    ~Position() = default;
    virtual Position& operator=(const Position&) = default;

    virtual const int x() const { return x_; }
    virtual const int y() const { return y_; }
    virtual const int x(const int x_value) { x_ = x_value; return x_; }
    virtual const int y(const int y_value) { y_ = y_value; return y_; }
    virtual const int x(const std::string&);
    virtual const int y(const std::string&);

    Position distance(const Position& position) const;
    int length() const { return std::lround(std::hypot(x_, y_)); }

    friend std::ostream& operator<<(std::ostream& out, const Position& position)
    {
        out << "(" << position.x_ << ", " << position.y_ << ")";
        return out;
    }

    friend bool operator<(const Position& lhs, const Position& rhs)
    {
        return std::tie(lhs.x_, lhs.y_) < std::tie(rhs.x_, rhs.y_); // keep the same order
    }

    friend bool operator==(const Position& lhs, const Position& rhs) {
        return std::tie(lhs.x_, lhs.y_) == std::tie(rhs.x_, rhs.y_);
    }

    virtual Position& operator+=(const Position& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    friend Position operator+(Position lhs, const Position& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    virtual Position& operator/=(const int d)
    {
        x_ /= d;
        y_ /= d;
        return *this;
    }

    friend Position operator/(Position lhs, const int rhs)
    {
        lhs /= rhs;
        return lhs;
    }

private:
    int x_;
    int y_;
};

inline bool operator> (const Position& lhs, const Position& rhs) { return rhs < lhs; }
inline bool operator<=(const Position& lhs, const Position& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Position& lhs, const Position& rhs) { return !(lhs < rhs); }
inline bool operator!=(const Position& lhs, const Position& rhs) { return !(lhs == rhs); }

class BoardPosition : public Position
{
public:
    BoardPosition() noexcept : Position{ 1, 1 } {}
    BoardPosition(const int x_value, const int y_value) { x(x_value); y(y_value); }
    BoardPosition(const std::string& position) : Position{ position.substr(0, 2) } { x(x()); y(y()); }
    BoardPosition(const Position& p) { x(p.x()); y(p.y()); }
    ~BoardPosition() = default;
    virtual BoardPosition& operator=(const BoardPosition&) = default;

    const int x() const override { return Position::x(); }
    const int y() const override { return Position::y(); }
    const int xMin() const { return 1; }
    const int yMin() const { return 1; }
    const int xMax() const { return 9; }
    const int yMax() const { return 9; }
    const int x(const int x) override;
    const int y(const int y) override;
    const int x(const std::string&) override;
    const int y(const std::string&) override;
    void position(const int x_value, const int y_value) { x(x_value); y(y_value); }
    Position position() const { return (Position)*this; }

    virtual void restore(const std::string&);

    friend std::ostream& operator<<(std::ostream& out, const BoardPosition& position)
    {
        out << char('a' + position.x() - 1);
        out << position.y();
        return out;
    }

    friend bool operator<(const BoardPosition& lhs, const BoardPosition& rhs)
    {
        return (Position)lhs < (Position)rhs;
    }

    friend bool operator==(const BoardPosition& lhs, const BoardPosition& rhs) {
        return (Position)lhs == (Position)rhs;
    }

    BoardPosition& operator+=(const Position& rhs)
    {
        x(x() + rhs.x());
        y(y() + rhs.y());
        return *this;
    }

    friend BoardPosition operator+(BoardPosition lhs, const Position& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    BoardPosition& operator/=(const int d)
    {
        x(x() / d);
        y(y() / d);
        return *this;
    }

    friend BoardPosition operator/(BoardPosition lhs, const int rhs)
    {
        lhs /= rhs;
        return lhs;
    }

private:
    virtual bool validX(const int x);
    virtual bool validY(const int y);
};

inline bool operator> (const BoardPosition& lhs, const BoardPosition& rhs) { return rhs < lhs; }
inline bool operator<=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs > rhs); }
inline bool operator>=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs < rhs); }
inline bool operator!=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs == rhs); }

class PawnPosition : public BoardPosition
{
public:
    PawnPosition() = default;
    PawnPosition(const BoardPosition& position, const PlayerName& playerName, const Color& color = Color::none, const PawnStyle& style = PawnStyle::none) : BoardPosition{ position }, playerName_{ playerName }, color_{ color }, pawn_style_{ style } {}
    PawnPosition(const unsigned int x, const unsigned int y, const PlayerName& playerName, const Color& color = Color::none, const PawnStyle& style = PawnStyle::none) : BoardPosition{ x, y }, playerName_{ playerName }, color_{ color }, pawn_style_{ style } {}
    PawnPosition(const std::string & playerPosition, const PlayerName& playerName = "", const Color& color = Color::none, const PawnStyle& style = PawnStyle::none) : BoardPosition(playerPosition.substr(0, 2)), playerName_(playerName), color_{ color }, pawn_style_{ style } {}
    ~PawnPosition() = default;
    PawnPosition& operator=(const PawnPosition&);

    void playerName(const PlayerName& playerName) { playerName_ = playerName; }
    const PlayerName& playerName() const { return playerName_; }
    void color(const Color& color) { color_ = color; }
    const Color& color() const { return color_; }
    void pawnStyle(PawnStyle style) { pawn_style_ = style; }
    const PawnStyle& pawnStyle() const { return pawn_style_; }
    void restore(const std::string&) override;
    void restore(const std::string&, const PlayerName&);

    friend std::ostream& operator<<(std::ostream& out, const PawnPosition& position)
    {
        out << (BoardPosition)position;
        if (!position.playerName_.empty())
            out << " (" << position.playerName_ << ") ";
        return out;
    }

private:
    PlayerName playerName_;
    Color color_;
    PawnStyle pawn_style_;
};


class WallPosition : public BoardPosition
{
public:
    enum class Direction { horizontal, vertical };

    WallPosition() noexcept : direction_{ Direction::horizontal } {};
    WallPosition(const int x_value, const int y_value) : direction_{ Direction::horizontal } { x(x_value); y(y_value); }


    WallPosition(const int x, const int y, const Direction& direction) : BoardPosition(x, y), direction_(direction) {}
    WallPosition(const int x, const int y, const std::string& text) : BoardPosition(x, y), direction_(direction(text)) {}
    WallPosition(const std::string & wallPosition) : BoardPosition(wallPosition.substr(0, 2)), direction_(direction(wallPosition.substr(2, 1))) {}
    WallPosition(const BoardPosition&, const BoardPosition&, const bool);
    ~WallPosition() = default;
    WallPosition& operator=(const WallPosition&) = default;

    const int x() const { return BoardPosition::x(); }
    const int y() const { return BoardPosition::y(); }
    const int x(const int x);
    const int y(const int y);
    const int x(const std::string & x);
    const int y(const std::string & y);

    void direction(const Direction& direction) { direction_ = direction; }
    const Direction direction(const std::string&);
    const Direction direction() const { return direction_; }
    const Direction oppositeDirection() const { return direction_ == Direction::horizontal ? Direction::vertical : Direction::horizontal; }

    WallPosition shiftedWallPosition(Position) const;

    void restore(const std::string&) override;

    friend std::ostream& operator<<(std::ostream& out, const WallPosition& position)
    {
        out << (BoardPosition)position;
        if (position.direction_ == WallPosition::Direction::horizontal)
            out << "h";
        if (position.direction_ == WallPosition::Direction::vertical)
            out << "v";
        return out;
    }

    friend bool operator==(const WallPosition& lhs, const WallPosition& rhs) {
        return ((Position)lhs == (Position)rhs) && (lhs.direction_ == rhs.direction_);
    }

    WallPosition& operator+=(WallPosition rhs)
    {
        (Position)rhs += (Position)*this;
        if (validX(rhs.x()) && validY(rhs.y()))
        {
            x(rhs.x());
            y(rhs.y());
        }
        else
        {
            std::ostringstream oss;
            oss << "Position " << rhs << " is out of board.";
            throw std::out_of_range(oss.str());
        }
        return *this;
    }

    friend WallPosition operator+(WallPosition lhs, const WallPosition& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    WallPosition& operator+=(Position rhs)
    {
        rhs += (Position)*this;
        if (validX(rhs.x()) && validY(rhs.y()))
        {
            x(rhs.x());
            y(rhs.y());
        }
        else
        {
            std::ostringstream oss;
            oss << "Position " << rhs << " is out of board.";
            throw std::out_of_range(oss.str());
        }
        return *this;
    }

    friend WallPosition operator+(WallPosition lhs, const Position& rhs)
    {
        lhs += rhs;
        return lhs;
    }

private:
    Direction direction_;
    bool validX(const int x);
    bool validY(const int y);
};

inline bool operator!=(const WallPosition& lhs, const WallPosition& rhs) { return !(lhs == rhs); }

class Move
{
public:
    enum class Type { pawn, wall, illegal_pawn, illegal_wall, surrend, none };
    friend std::ostream& operator<<(std::ostream& out, const Type& type)
    {
        switch (type)
        {
        case Move::Type::pawn:
        case Move::Type::wall:
            break;
        case Move::Type::illegal_pawn:
        case Move::Type::illegal_wall:
            out << "(illegal)";
            break;
        case Move::Type::surrend:
            out << "(resigns)";
            break;
        }
        return out;
    }

    Move() noexcept : type_{ Type::none }, player_{}, wall_{} {}
    Move(const PawnPosition &player) : type_{ Type::pawn }, player_{ player }, wall_{} {}
    Move(const WallPosition &wall) : type_{ Type::wall }, player_{}, wall_{ wall } {}
    Move(const std::string &text) : 
        type_{ type(text) }, 
        player_{ type_ == Type::pawn || type_ == Type::illegal_pawn ? PawnPosition(text) : PawnPosition() }, 
        wall_{ type_ == Type::wall || type_ == Type::illegal_wall ? WallPosition(text) : WallPosition() } 
    {}

    void restore(const std::string &move);
    const Type& type(const std::string &text);
    void setIllegal();
    const Type& type() const { return type_; }
    void playerName(const PlayerName& playerName) { player_.playerName(playerName); }
    const PawnPosition& pawn() const { return player_; }
    const WallPosition& wall() const { return wall_; }

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        switch (move.type())
        {
        case Move::Type::pawn:
        case Move::Type::illegal_pawn:
            out << (BoardPosition)move.player_ << move.type_; // casted to mask player name
            break;
        case Move::Type::wall:
        case Move::Type::illegal_wall:
            out << move.wall_ << move.type_;
            break;
        case Move::Type::surrend:
            out << move.type_;
            break;
        }
        return out;
    }

    friend bool operator==(const Move& lhs, const Move& rhs) {
        auto pawnType = ((lhs.type_ == Move::Type::pawn) || (lhs.type_ == Move::Type::illegal_pawn));
        auto pawnEqual = (lhs.player_ == rhs.player_);
        auto wallType = ((lhs.type_ == Move::Type::wall) || (lhs.type_ == Move::Type::illegal_wall));
        auto wallEqual = (lhs.wall_ == rhs.wall_);

        return (lhs.type_ == rhs.type_ && (
            (pawnType && pawnEqual)
            || (wallType && pawnEqual)
            || (lhs.type_ == Move::Type::surrend)
            || (lhs.type_ == Move::Type::none)
            ));
    }

private:
    Type type_;
    PawnPosition player_;
    WallPosition wall_;
};
