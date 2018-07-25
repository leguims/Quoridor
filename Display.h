#pragma once

#include "Position.h"

namespace Quoridor {
    namespace Display {
        class Display
        {
        public:
            Display() = default;
            virtual ~Display() = default;
            virtual void Board(void) = 0;
            virtual void pawn(short player_number, Color color, PawnStyle pawn_style) = 0;
            virtual void pawn(PawnPosition old, PawnPosition new_, short player_number, Color color = Color::none, PawnStyle pawn_style = PawnStyle::none) = 0;
            virtual void pawn(PawnPosition new_, short player_number, Color color = Color::none, PawnStyle pawn_style = PawnStyle::none) = 0;
            virtual void wall(WallPosition new_) = 0;
            virtual void refresh() = 0;
        };
    }
}