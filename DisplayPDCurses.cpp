#include "DisplayPDCurses.h"

#include <algorithm>

namespace Quoridor {
    DisplayPDCurses::DisplayPDCurses()
    {
        initscr();                  /* Start curses mode */
                                    // COLS, LINES which are initialized to the screen sizes after initscr()
        raw();				        /* Line buffering disabled	*/
        keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
        noecho();			        /* Don't echo() while we do getch */

                                    //halfdelay(5);             /* Set timeout with getch() */
                                    // cbreak();                /* Don't wait EOF to give input */
                                    // noecho();                /* Don't print on screen keys pressed */
                                    // getmaxyx(stdscr, row, col);		/* find the boundaries of the screeen */
                                    // getyx(stdscr, y, x);		/* get the current curser position */
                                    // attron(A_BOLD);			/* cut bold on */
                                    // attroff(A_BOLD);			/* cut bold off */
                                    // box(local_win, 0 , 0); /* Draw a box around the window */
                                    // wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' '); /* Erase the box */
                                    // mvaddstr(y, x, string);

        if (has_colors() == FALSE)
        {
            endwin();
            // std::cout << "Your terminal does not support color\n";
            exit(1);
        }
        start_color();			/* Start color 			*/

        // Initialize known colors
        init_pair(static_cast<short>(target::legend), COLOR_WHITE, COLOR_BLACK);
        init_pair(static_cast<short>(target::pawn_empty), COLOR_WHITE, 52); // Brown
        init_pair(static_cast<short>(target::wall_empty), COLOR_WHITE, 234);
        init_pair(static_cast<short>(target::wall_full), COLOR_WHITE, 214);
        for (const auto& i : { target::legend, target::pawn_empty, target::wall_empty, target::wall_full })
        {
            color_[i] = COLOR_PAIR(i);
        }
    }

    DisplayPDCurses::~DisplayPDCurses()
    {
        endwin();                     /* End curses mode */
    }

    void DisplayPDCurses::Board(void)
    {
        auto line_legend = " a b c d e f g h i ";
        auto line_void = "                   ";

        // Adjust or define color
        // init_color(COLOR_RED, 700, 0, 0);
        /* param 1     : color name
        * param 2, 3, 4 : rgb content min = 0, max = 1000 */

        for (int y = 0; y <= y_max_; ++y)
        {
            // Print text
            if ((y == 0) || (y == y_max_))
            {
                // Add column's legend
                attron(color_[target::legend]);       // Print colors
                mvaddstr(y, 0, line_legend);
            }
            else
            {
                // Print line as empty wall
                attron(color_[target::wall_empty]);
                mvaddstr(y, 0, line_void);
                // Restore line's color legend 
                mvchgat(y, 0, 1, A_NORMAL, color_[target::legend], NULL);
                mvchgat(y, x_max_, 1, A_NORMAL, color_[target::legend], NULL);

                if ((y % 2) != 0)
                {
                    // Add line text legend
                    mvaddch(y, 0, '9' - ((y - 1) / 2));
                    mvaddch(y, x_max_, '9' - ((y - 1) / 2));
                    for (int x = 1; x <= (x_max_ - 1); ++x)
                    {
                        if ((x % 2) != 0)
                        {
                            // mvchgat(y, x, length, attribute, color_index, NULL);
                            /*
                            * First two parameters specify the position at which to start
                            * Third parameter number of characters to update. -1 means till
                            * end of line
                            * Forth parameter is the normal attribute you wanted to give
                            * to the charcter
                            * Fifth is the color index. It is the index given during init_pair()
                            * use 0 if you didn't want color
                            * Sixth one is always NULL
                            */
                            // Restore pawn color square
                            mvchgat(y, x, 1, A_NORMAL, static_cast<short>(target::pawn_empty), NULL);
                        }
                        else
                        {
                            // Restore wall color square
                            mvchgat(y, x, 1, A_NORMAL, static_cast<short>(target::wall_empty), NULL);
                        }
                    }
                }
            }
        }
        //mvaddstr(0, 0, " a b c d e f g h i ");
        //mvaddstr(1, 0, "9                 9");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(2, 0, "8                 8");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(3, 0, "7                 7");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(4, 0, "6                 6");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(5, 0, "5                 5");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(6, 0, "4                 4");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(7, 0, "3                 3");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(8, 0, "2                 2");
        //mvaddstr(2, 0, "                   ");
        //mvaddstr(9, 0, "1                 1");
        //mvaddstr(10, 0, " a b c d e f g h i ");
    }

    void DisplayPDCurses::pawn(short player_number, std::string color, std::string pawn_style)
    {
        auto target_ = (player_number == 1 ? target::pawn1 : target::pawn2);

        // Adjust or define color
        // init_color(COLOR_RED, 700, 0, 0);
        /* param 1     : color name
        * param 2, 3, 4 : rgb content min = 0, max = 1000 */

        // init_pair(index, foreground color, background color);
        if (!color.empty())
        {
            std::transform(color.begin(), color.end(), color.begin(), ::tolower);
            if ((color == "red") || (color == "r"))
                init_pair(static_cast<short>(target_), COLOR_RED, 52);
            else if ((color == "blue") || (color == "b"))
                init_pair(static_cast<short>(target_), COLOR_BLUE, 52);
            else if ((color == "green") || (color == "g"))
                init_pair(static_cast<short>(target_), COLOR_GREEN, 52);
            else if ((color == "white") || (color == "w"))
                init_pair(static_cast<short>(target_), COLOR_WHITE, 52);
            else if ((color == "black") || (color == "b"))
                init_pair(static_cast<short>(target_), COLOR_BLACK, 52);
            color_[target_] = COLOR_PAIR(target_);
        }

        if (!pawn_style.empty())
        {
            pawn_style_[target_] = pawnStyle(pawn_style);
        }
    }

    void DisplayPDCurses::pawn(PawnPosition old, PawnPosition new_, short player_number, std::string color, std::string pawn_style)
    {
        auto target_ = (player_number == 1 ? target::pawn1 : target::pawn2);

        if (!color.empty() || !pawn_style.empty())
            pawn(player_number, color, pawn_style);

        // Erase old
        auto erase_pawn = pawnStyle("erase");
        attron(color_[target::pawn_empty] | A_BOLD);
        mvaddch(19 - old.y() * 2, old.x() * 2 - 1, erase_pawn);
        // Write new
        attron(color_[target_] | A_BOLD);
        mvaddch(19 - new_.y() * 2, new_.x() * 2 - 1, pawn_style_[target_]);
    }

    void DisplayPDCurses::pawn(PawnPosition new_, short player_number, std::string color, std::string pawn_style)
    {
        auto old = PawnPosition();
        pawn(old, new_, player_number, color, pawn_style);
    }

    void DisplayPDCurses::wall(WallPosition new_)
    {
        // Adjust or define color
        // init_color(COLOR_RED, 700, 0, 0);
        /* param 1     : color name
        * param 2, 3, 4 : rgb content min = 0, max = 1000 */

        // init_pair(index, foreground color, background color);
        auto wfi = static_cast<short>(target::wall_full);

        if (new_.direction() == WallPosition::Direction::horizontal)
            mvchgat(19 - new_.y() * 2 - 1, new_.x() * 2 - 1, 3, A_NORMAL, wfi, NULL);
        else
        {
            mvchgat(19 - new_.y() * 2 - 2, new_.x() * 2, 1, A_NORMAL, wfi, NULL);
            mvchgat(19 - new_.y() * 2 - 1, new_.x() * 2, 1, A_NORMAL, wfi, NULL);
            mvchgat(19 - new_.y() * 2, new_.x() * 2, 1, A_NORMAL, wfi, NULL);
        }
    }

    void DisplayPDCurses::refresh()
    {
        ::refresh();
    }

#ifdef HAVE_WIDE
    wchar_t DisplayPDCurses::pawnStyle(std::string pawn_style)
    {
        std::transform(pawn_style.begin(), pawn_style.end(), pawn_style.begin(), ::tolower);

        if ((pawn_style == "circle") || (pawn_style == "smile"))
            return 0x263B;
        else if ((pawn_style == "cross") || (pawn_style == "spade"))
            return 0x2660;
        else if ((pawn_style == "hashtag") || (pawn_style == "club"))
            return 0x2663;
        else if ((pawn_style == "arobase") || (pawn_style == "heart"))
            return 0x2665;
        else if ((pawn_style == "star") || (pawn_style == "diamond"))
            return 0x2666;
        else if ((pawn_style == "dollar") || (pawn_style == "triangle"))
            return 0x25B2;
        else if (pawn_style == "erase")
            return 0x0020;
        return 0x263B;
    }
#else
    char DisplayPDCurses::pawnStyle(std::string pawn_style)
    {
        std::transform(pawn_style.begin(), pawn_style.end(), pawn_style.begin(), ::tolower);

        if ((pawn_style == "circle") || (pawn_style == "smile"))
            return 'O';
        else if ((pawn_style == "cross") || (pawn_style == "spade"))
            return 'X';
        else if ((pawn_style == "hashtag") || (pawn_style == "club"))
            return '#';
        else if ((pawn_style == "arobase") || (pawn_style == "heart"))
            return '@';
        else if ((pawn_style == "star") || (pawn_style == "diamond"))
            return '*';
        else if ((pawn_style == "dollar") || (pawn_style == "triangle"))
            return '$';
        else if (pawn_style == "erase")
            return ' ';
        return 'X';
    }
#endif
}