#pragma once

#include "Display.h"
#include "PDCurses/curses.h"
#include <map>

#define HAVE_WIDE

namespace Quoridor {
    class DisplayPDCurses : public Display
    {
    public:
        DisplayPDCurses();
        ~DisplayPDCurses();

        void Board(void);
        void pawn(short player_number, std::string color, std::string pawn_style);
        void pawn(PawnPosition old, PawnPosition new_, short player_number, std::string color = "", std::string pawn_style = "");
        void pawn(PawnPosition new_, short player_number, std::string color = "", std::string pawn_style = "");
        void wall(WallPosition new_);
        void refresh();

    private:
        enum class target
        {
            legend = 1, pawn1, pawn2, pawn_empty, wall_empty, wall_full
        };

        std::map<target, unsigned long long> color_;
        int y_max_ = 8 * 2 + 1 + 2 - 1;
        int x_max_ = 9 * 2 + 1 - 1;

#ifdef HAVE_WIDE
        std::map<target, wchar_t> pawn_style_;
        wchar_t pawnStyle(std::string pawn_style = "erase");
#else
        std::map<target, char> pawn_style_;
        char pawnStyle(std::string pawn_style = "erase");
#endif
    };
}
