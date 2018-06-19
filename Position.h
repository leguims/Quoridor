#pragma once

#include "Common.h"
#include <ostream>
#include <tuple>
#include <sstream>

class Position
{
public:
    Position() : x_(0), y_(0) {}
    Position(const int x_value, const int y_value) : x_(x_value), y_(y_value) {}
    ~Position() = default;
    Position& operator=(const Position&) = default;

    const int x() const { return x_; }
    const int y() const { return y_; }
    const int x(const int x_value) { x_ = x_value; return x_; }
    const int y(const int y_value) { y_ = y_value; return y_; }
    Position distance(const Position& position) const;

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

    Position& operator+=(const Position& rhs)
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
    BoardPosition() : Position{ 1, 1 } {}
    BoardPosition(const int x_value, const int y_value) { x(x_value); y(y_value); }
    BoardPosition(const std::string& position) : Position{ x(position.substr(0, 1)), y(position.substr(1, 1)) } {}
    ~BoardPosition() = default;
    BoardPosition& operator=(const BoardPosition&) = default;

    const int x() const { return Position::x(); }
    const int y() const { return Position::y(); }
    const int x(const int x);
    const int y(const int y);
    const int x(const std::string & x);
    const int y(const std::string & y);

    void restore(const std::string&);

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

    BoardPosition& operator+=(const BoardPosition& rhs)
    {
        Position tmp = *this;
        tmp += (Position)rhs;
        if (validX(tmp.x()) && validY(tmp.y()))
        {
            x(tmp.x());
            y(tmp.y());
        }
        else
        {
            std::ostringstream oss;
            oss << "Position " << tmp << " is out of board.";
            throw std::out_of_range(oss.str());
        }
        return *this;
    }

    friend BoardPosition operator+(BoardPosition lhs, const BoardPosition& rhs)
    {
        lhs += rhs;
        return lhs;
    }

private:
    bool validX(const int x);
    bool validY(const int y);
};

inline bool operator> (const BoardPosition& lhs, const BoardPosition& rhs) { return rhs < lhs; }
inline bool operator<=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs > rhs); }
inline bool operator>=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs < rhs); }
inline bool operator!=(const BoardPosition& lhs, const BoardPosition& rhs) { return !(lhs == rhs); }

class PawnPosition : public BoardPosition
{
public:
    PawnPosition() = default;
    PawnPosition(const BoardPosition& position, const PlayerName& playerName) : BoardPosition{ position }, playerName_{ playerName } {}
    PawnPosition(const unsigned int x, const unsigned int y, const PlayerName& playerName) : BoardPosition{ x, y }, playerName_{ playerName } {}
    PawnPosition(const std::string & playerPosition, const PlayerName& playerName = "") : BoardPosition(playerPosition.substr(0, 2)), playerName_(playerName) {}
    ~PawnPosition() = default;
    PawnPosition& operator=(const PawnPosition&) = default;

    void playerName(const PlayerName& playerName) { playerName_ = playerName; }
    const PlayerName& playerName() const { return playerName_; }
    void restore(const std::string&);
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
};


class WallPosition : public BoardPosition
{
public:
    enum class Direction { horizontal, vertical };

    WallPosition() = default;
    WallPosition(const int x_value, const int y_value) { x(x_value); y(y_value); }


    WallPosition(const int x, const int y, const Direction& direction) : BoardPosition(x, y), direction_(direction) {}
    WallPosition(const int x, const int y, const std::string& text) : BoardPosition(x, y), direction_(direction(text)) {}
    WallPosition(const std::string & wallPosition) : BoardPosition(wallPosition.substr(0, 2)), direction_(direction(wallPosition.substr(2, 1))) {}
    WallPosition(const BoardPosition& a, const BoardPosition& b);
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

    WallPosition shiftedWallPosition(Position) const;

    void restore(const std::string&);

    friend std::ostream& operator<<(std::ostream& out, const WallPosition& position)
    {
        out << (BoardPosition)position;
        if (position.direction_ == WallPosition::Direction::horizontal)
            out << "h";
        if (position.direction_ == WallPosition::Direction::vertical)
            out << "v";
        return out;
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

class Move
{
public:
    enum class Type { pawn, wall, none };
    Move() : type_{ Type::none }, player_{}, wall_{} {}
    Move(const PawnPosition &player) : type_{ Type::pawn }, player_{ player }, wall_{} {}
    Move(const WallPosition &wall) : type_{ Type::wall }, player_{}, wall_{ wall } {}
    Move(const std::string &text) : type_{ type(text) }, player_{ type_ == Type::pawn ? PawnPosition(text) : PawnPosition() }, wall_{ type_ == Type::wall ? WallPosition(text) : WallPosition() } {}

    void restore(const std::string &move);
    const Type& type(const std::string &text);
    const Type& type() const { return type_; }
    void playerName(const PlayerName& playerName) { player_.playerName(playerName); }
    const PawnPosition& pawn() const { return player_; }
    const WallPosition& wall() const { return wall_; }

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        if (move.type_ == Type::pawn)
        {
            out << move.player_;
        }
        else if (move.type_ == Type::wall)
        {
            out << move.wall_;
        }
        return out;
    }

private:
    Type type_;
    PawnPosition player_;
    WallPosition wall_;
};
