#pragma once

#include "Position.h"
#include "a-star/source/AStar.hpp"
#include "Display.h"

#include <vector>
#include <map>
#include <functional>

class Board
{
public:
    using handlerCB = std::function<void(void)>;

    Board() noexcept;
    Board(int width, int height) noexcept : width_{ width }, heigth_{ height }, showMoves_{ false }, handler_{nullptr} {}
    ~Board();

    void add(const Move & move);
    void registerHandler(const handlerCB &&);
    void display();

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
    void showMoves(const bool show) { showMoves_ = show; }

    friend std::ostream& operator<<(std::ostream& out, const Board& board)
    {
        std::vector<std::string> board_str = {
            //00000000001111111111222222222233333333334
            //01234567890123456789012345678901234567890
             "   a   b   c   d   e   f   g   h   i   ",
             " +---+---+---+---+---+---+---+---+---+ ",
             "9|   |   |   |   |   |   |   |   |   |9",
             " +---+---+---+---+---+---+---+---+---+ ",
             "8|   |   |   |   |   |   |   |   |   |8",
             " +---+---+---+---+---+---+---+---+---+ ",
             "7|   |   |   |   |   |   |   |   |   |7",
             " +---+---+---+---+---+---+---+---+---+ ",
             "6|   |   |   |   |   |   |   |   |   |6",
             " +---+---+---+---+---+---+---+---+---+ ",
             "5|   |   |   |   |   |   |   |   |   |5",
             " +---+---+---+---+---+---+---+---+---+ ",
             "4|   |   |   |   |   |   |   |   |   |4",
             " +---+---+---+---+---+---+---+---+---+ ",
             "3|   |   |   |   |   |   |   |   |   |3",
             " +---+---+---+---+---+---+---+---+---+ ",
             "2|   |   |   |   |   |   |   |   |   |2",
             " +---+---+---+---+---+---+---+---+---+ ",
             "1|   |   |   |   |   |   |   |   |   |1",
             " +---+---+---+---+---+---+---+---+---+ ",
             "   a   b   c   d   e   f   g   h   i   "
        };

        for (const auto& p : board.pawns())
        {
            std::ostringstream oss;
            oss << p.color();
            board_str[20 - 2 * p.y()][-1 + 4 * p.x()] = oss.str().c_str()[0]; // oss => string => char* => char
        }

        for (const auto& w : board.walls())
        {
            if (w.direction() == WallPosition::Direction::vertical)
            {
                board_str[20 - 2 * w.y() - 0][4 * w.x() + 1] = '#';
                board_str[20 - 2 * w.y() - 1][4 * w.x() + 1] = '#';
                board_str[20 - 2 * w.y() - 2][4 * w.x() + 1] = '#';
            }
            else
            {
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() - 1] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 0] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 1] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 2] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 3] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 4] = '#';
                board_str[20 - 2 * w.y() - 1][-1 + 4 * w.x() + 5] = '#';
            }
        }

        for (const auto& l : board_str)
            out << l << "\n";

        return out;
    }

private:
    int width_;
    int heigth_;
    std::vector<PawnPosition> pawnsPosition_;
    std::vector<WallPosition> wallsPosition_;
    handlerCB handler_;
    bool showMoves_;
    Quoridor::Display::Display *p_display_;

    PawnPosition& getPawnNonConst(const PlayerName& name);
    void add(PawnPosition pawn);
    void add(const WallPosition & wall);
    void callHandler();
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
