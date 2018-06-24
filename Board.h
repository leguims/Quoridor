#pragma once

#include "Position.h"
#include "a-star/source/AStar.hpp"

#include <vector>
#include <map>
#include <functional>

class Board
{
public:
    using handlerCB = std::function<void(void)>;

    Board() noexcept;
    Board(int width, int height) noexcept : width_{ width }, heigth_{ height } {}
    ~Board();

    void add(const Move & move);
    void registerHandler(const handlerCB &&);

    const PawnPosition& getPawn(const PlayerName& name) const;
    const std::vector<PawnPosition>& pawns() const { return pawnsPosition_; }
    const std::vector<WallPosition>& walls() const { return wallsPosition_; }
    bool existsWall(const WallPosition& wall) const;
    bool existsPawn(const Position& pawnPos) const;
    bool findPath(const WallPosition&, const PlayerName&, const std::vector<BoardPosition>&) const;

    const int width() const { return width_; }
    const int heigth() const { return heigth_; }
    void width(const int value) { width_ = value; }
    void heigth(const int value) { heigth_ = value; }
    // To remove ?
    //const std::vector<PawnPosition>& playersPosition() { return pawnsPosition_; }
    //std::vector<WallPosition>& wallsPosition() { return wallsPosition_; }

private:
    int width_;
    int heigth_;
    std::vector<PawnPosition> pawnsPosition_;
    std::vector<WallPosition> wallsPosition_;
    std::vector<handlerCB> handlers_;

    PawnPosition& getPawnNonConst(const PlayerName& name);
    void add(const PawnPosition & pawn);
    void add(const WallPosition & wall);
    void callHandlers();
    //void add(AStar::Generator &, const WallPosition &) const;
};


class AStarBoard
{
public:
    AStarBoard(const int, const int, const std::vector<WallPosition>&);
    ~AStarBoard() = default;

    bool findPath(const WallPosition&, const BoardPosition&, const std::vector<BoardPosition>&);


private:
    AStar::Generator generator_;
    void add(const WallPosition &);
    void remove(const WallPosition &);
    AStar::Vec2i position(const BoardPosition&);
    std::vector<AStar::Vec2i> position(const WallPosition&);
};
