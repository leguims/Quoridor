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
    void setPlayers(const std::shared_ptr<std::vector<Player>> & players);
    void launch();

    bool Win(const PlayerName&) const;
    void illegalMove(const PlayerName&);
    bool ValidWall(const WallPosition&) const;
    bool ValidPawn(const PawnPosition&) const;
    bool ValidMove(const Move&) const;
    const std::vector<WallPosition>& getValidWalls(const PlayerName&);
    const std::vector<PawnPosition>& getValidPawns(const PlayerName&);
    //const std::vector<Move>& getValidMoves(const PlayerName & player);

    friend std::ostream& operator<<(std::ostream& out, const Referee& referee)
    {
        out << "Classic";
        return out;
    }

private:
    std::shared_ptr<Board> board_;
    std::shared_ptr<std::vector<Player>> players_;
    std::vector<PawnPosition> validPawns_;
    std::vector<WallPosition> validwalls_;
    std::vector<Move> validMoves_;
    PlayerName illegalMove_;

    //bool ValidWallRange(const WallPosition &) const;
    //bool ValidWallAvailable(const WallPosition &) const;
    //bool ValidWallDoNotCloseTheArrival(const WallPosition &) const;

    //bool ValidPawnRange(const PawnPosition &) const;
    //bool ValidPawnFree(const PawnPosition &) const;
    //bool ValidPawnLegalMove(const PawnPosition &) const;

	//const std::vector<std::string> arrival_e1 = { "a9", "b9", "c9", "d9", "e9", "f9", "g9", "h9", "i9" };
	//const std::vector<std::string> arrival_e9 = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1" };

    std::vector<WallPosition> findBlockerWalls(const BoardPosition&, const BoardPosition&) const;
    bool noBlockerWalls(const BoardPosition&, const BoardPosition&) const;
    Player& find(const PlayerName&) const;
};

