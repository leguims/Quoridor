#pragma once

#include "Position.h"
#include "Player.h"

#include <memory>

class Referee
{
public:
    Referee() = default;
    ~Referee() = default;
    void setBoard(const std::shared_ptr<Board>& board);
    void setPlayers(const std::shared_ptr<std::vector<Player*>> & players);
    void launch();

    bool Win(const PlayerName&) const;
    void illegalMove(const PlayerName&);
    bool ValidWall(const WallPosition&) const;
    bool ValidPawn(const PawnPosition&) const;
    bool ValidMove(const Move&) const;
    const std::vector<WallPosition>& getValidWalls(const PlayerName&);
    const std::vector<PawnPosition>& getValidPawns(const PlayerName&);

    friend std::ostream& operator<<(std::ostream& out, const Referee& referee)
    {
        out << "Classic";
        return out;
    }

private:
    std::shared_ptr<Board> board_;
    std::shared_ptr<std::vector<Player*>> players_;
    std::vector<PawnPosition> validPawns_;
    std::vector<WallPosition> validwalls_;
    std::vector<Move> validMoves_;
    PlayerName illegalMove_;

    std::vector<WallPosition> findBlockerWalls(const BoardPosition&, const BoardPosition&) const;
    bool noBlockerWalls(const BoardPosition&, const BoardPosition&) const;
    Player& find(const PlayerName&) const;
};

