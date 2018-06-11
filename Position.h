#pragma once

#include "Common.h"

class Position
{
public:
    Position() : x_(0), y_(0) {};
    Position(const unsigned int x_value, const unsigned int y_value) { x(x_value); y(y_value); };
    Position(const std::string& position) : x_{ x(position.substr(0, 1)) }, y_{ y(position.substr(1, 1)) } {}
    ~Position() = default;
    const unsigned int x() const { return x_; }
    const unsigned int y() const { return y_; }
    unsigned int x(const unsigned int x);
    unsigned int y(const unsigned int y);
    unsigned int x(const std::string & x);
    unsigned int y(const std::string & y);
    void restore(const std::string&);
    std::string& save() const;

private:
    unsigned int x_;
    unsigned int y_;
};

class PawnPosition : public Position
{
public:
    PawnPosition() = default;
    PawnPosition(const unsigned int x, const unsigned int y, const PlayerName& playerName) : Position(x, y), playerName_(playerName) {}
    PawnPosition(const std::string & playerPosition, const PlayerName& playerName="") : Position(playerPosition.substr(0, 2)), playerName_(playerName) {}
    PawnPosition& operator=(const PawnPosition&) = default;
    ~PawnPosition() = default;

    void playerName(const PlayerName& playerName) { playerName_ = playerName; }
    void restore(const std::string&);
    void restore(const std::string&, const PlayerName&);
    std::string& save() const;

private:
    PlayerName playerName_;
};


class WallPosition : public Position
{
public:
    enum class Direction { horizontal, vertical };

    WallPosition() = default;
    WallPosition(const unsigned int x, const unsigned int y, const Direction& direction) : Position(x, y), direction_(direction) {}
    WallPosition(const std::string & wallPosition) : Position(wallPosition.substr(0, 2)), direction_(direction(wallPosition.substr(2, 1))) {}
    WallPosition& operator=(const WallPosition&) = default;
    ~WallPosition() = default;

    void direction(const Direction& direction) { direction_ = direction; }
    Direction direction(const std::string&);

    void restore(const std::string&);
    std::string& save() const;

private:
    Direction direction_;

    std::string& saveDirection() const;
};

class Move
{
public:
    enum class Type { pawn, wall, none };
    Move() : type_{ Type::none }, player_{}, wall_{} {}
    Move(const PawnPosition &player) : type_{ Type::pawn }, player_{ player }, wall_{} {}
    Move(const WallPosition &wall) : type_{ Type::wall }, player_{}, wall_{ wall } {}
    Move(const std::string &text) : type_{ type(text) }, player_{ type_ == Type::pawn ? PawnPosition(text) : PawnPosition() }, wall_{ type_ == Type::wall ? WallPosition(text) : WallPosition() } {}

    std::string & save() const;
    void restore(const std::string &move);
    Type type(const std::string &text);

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        out << move.save();
        return out;
    }

private:
    Type type_;
    PawnPosition player_;
    WallPosition wall_;
};
