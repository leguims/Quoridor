// Quoridor.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <thread>
#include <chrono>

// #define NDEBUG
#include <cassert>

#include "Game.h"

void test_BoardPosition();
void test_Board();
void test_Replay();
void test_Game_IA_linear();
void test_Game_IA_random();


#include "PDCurses/curses.h"

static int next_j(int j)
{
    if (j == 0)
        j = 4;
    else
        --j;

    if (has_colors())
    {
        int z = rand() % 3;
        chtype color = COLOR_PAIR(z);

        if (z)
            color |= A_BOLD;

        attrset(color);
    }

    return j;
}

void main_pdcurses_rain()
{
    int x, y, j, r, c, seed;
    static int xpos[5], ypos[5];

    initscr();                    /* Start curses mode */
    seed = (int)time((time_t *)0);
    srand(seed);

    if (has_colors())
    {
        short bg = COLOR_BLACK;

        start_color();
        init_pair(1, COLOR_BLUE, bg);
        init_pair(2, COLOR_CYAN, bg);
    }

    nl();
    noecho();
    curs_set(0);
    timeout(0);
    keypad(stdscr, TRUE);

    r = LINES - 4;
    c = COLS - 4;

    for (j = 5; --j >= 0;)
    {
        xpos[j] = rand() % c + 2;
        ypos[j] = rand() % r + 2;
    }

    for (j = 0;;)
    {
        x = rand() % c + 2;
        y = rand() % r + 2;

        mvaddch(y, x, '.');

        mvaddch(ypos[j], xpos[j], 'o');

        j = next_j(j);
        mvaddch(ypos[j], xpos[j], 'O');

        j = next_j(j);
        mvaddch(ypos[j] - 1, xpos[j], '-');
        mvaddstr(ypos[j], xpos[j] - 1, "|.|");
        mvaddch(ypos[j] + 1, xpos[j], '-');

        j = next_j(j);
        mvaddch(ypos[j] - 2, xpos[j], '-');
        mvaddstr(ypos[j] - 1, xpos[j] - 1, "/ \\");
        mvaddstr(ypos[j], xpos[j] - 2, "| O |");
        mvaddstr(ypos[j] + 1, xpos[j] - 1, "\\ /");
        mvaddch(ypos[j] + 2, xpos[j], '-');

        j = next_j(j);
        mvaddch(ypos[j] - 2, xpos[j], ' ');
        mvaddstr(ypos[j] - 1, xpos[j] - 1, "   ");
        mvaddstr(ypos[j], xpos[j] - 2, "     ");
        mvaddstr(ypos[j] + 1, xpos[j] - 1, "   ");
        mvaddch(ypos[j] + 2, xpos[j], ' ');

        xpos[j] = x;
        ypos[j] = y;

        switch (getch())
        {
        case 'q':
        case 'Q':
            curs_set(1);
            endwin();
            return;
        case 's':
            nodelay(stdscr, FALSE);
            break;
        case ' ':
            nodelay(stdscr, TRUE);
#ifdef KEY_RESIZE
            break;
        case KEY_RESIZE:
# ifdef PDCURSES
            resize_term(0, 0);
            erase();
# endif
            r = LINES - 4;
            c = COLS - 4;
#endif
        }
        napms(50);
    }
}

void main_pdcurses_helloworld()
{
    initscr();                    /* Start curses mode */
    printw("Hello World !!!");    /* Print Hello World */
    refresh();                    /* Print it on to the real screen */
    getch();                      /* Wait for user input */
    endwin();                     /* End curses mode */
}

void main_pdcurses_board()
{
    initscr();                  /* Start curses mode */
    raw();				        /* Line buffering disabled	*/
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
    noecho();			        /* Don't echo() while we do getch */
    //halfdelay(5);             /* Set timeout with getch() */
    // cbreak();                /* Don't wait EOF to give input */
    // noecho();                /* Don't print on screen keys pressed */

    printw("Hello World !!!");    /* Print Hello World */
    refresh();                    /* Print it on to the real screen */

    getch();                      /* Wait for user input */
    endwin();                     /* End curses mode */
}

void main_pdcurses()
{
    // Source : http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
    // Source : https://pdcurses.sourceforge.io/doc/PDCurses.txt
    // Source : https://www.projectpluto.com/win32a.htm

    //main_pdcurses_rain();
    //main_pdcurses_helloworld();
    main_pdcurses_board();
}


int main()
{
    main_pdcurses();

    test_BoardPosition();
    test_Board();
    test_Replay();
    test_Game_IA_linear();

    auto start = std::chrono::system_clock::now();
    auto end = start;
    std::cout << "Loop on Random IA ..." << std::endl;
    do
    {
        test_Game_IA_random();
        end = std::chrono::system_clock::now();
    } while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 300); //60s
    std::cout << "End of loop on Random IA." << std::endl;

    {
        using namespace std::chrono_literals;
        std::cout << "End in ";
        for (auto i = 5; i > 0; --i)
        {
            std::cout << i << " ";
            std::this_thread::sleep_for(1s);
        }
    }

    return 0;
}

void test_Game_IA_random()
{
    auto test = [](std::string title, Player *p1, Player *p2, bool showMoves) {
        std::cout << "test_Game(" << title << ") : START" << std::endl;
        Game game;

        game.chooseReferee();
        game.choosePlayers(p1, p2);
        game.showMoves(showMoves);
        game.launch();
        assert(game.getResult() == Game::Result::inProgress);

        while (game.getResult() == Game::Result::inProgress)
            game.move();

        std::cout << game;
        std::cout << "Game result : " << game.getResult() << std::endl;
        std::cout << "Save game to file : " << game.save() << std::endl;
        std::cout << "test_Game(" << title << ") : END\n\n";
    };

    {
        auto ia1 = new IA_random_pawn("IA_random_pawn.1", Color::black, BoardPosition("e1"));
        auto ia2 = new IA_random_pawn("IA_random_pawn.2", Color::white, BoardPosition("e9"));
        test("Test Random pawn 1", ia1, ia2, false);
        delete ia1, ia2;
    }

    {
        auto ia1 = new IA_random_wall_pawn("IA_random_wall_pawn.1", Color::black, BoardPosition("e1"));
        auto ia2 = new IA_random_wall_pawn("IA_random_wall_pawn.2", Color::white, BoardPosition("e9"));
        test("Test Random wall/pawn 1", ia1, ia2, false);
        delete ia1, ia2;
    }
}

void test_Game_IA_linear()
{
        auto test = [](std::string title, Player *p1, Player *p2, Game::Result result,
            std::pair<Move, Move>& last_move, bool showMoves = false) {
        std::cout << "test_Game(" << title << ") : START" << std::endl;
        Game game;

        game.chooseReferee();
        game.choosePlayers(p1, p2);
        game.showMoves(showMoves);
        game.launch();
        assert(game.getResult() == Game::Result::inProgress);

        while (game.getResult() == Game::Result::inProgress)
            game.move();

        assert(last_move == *game.moves().rbegin());
        assert(game.getResult() == result);
        std::cout << game;
        std::cout << "Game result : " << game.getResult() << std::endl;
        std::cout << "test_Game(" << title << ") : END\n\n";
    };

    {
        // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
        //   Only legal moves.
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "g6v",
            "e5", "e4",
            "e6", "d4",
            "c3h", "a3h",
            "e7", "e7h",
            "d6v", "d4v" // Game not over, player 1 resigns
        };
        std::pair<Move, Move> lastMove{ "(resigns)", "" };

        auto ia1 = new IA_linear("*IA_1: Player 1*", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("*IA_1: Player 2*", Color::white, BoardPosition("e9"), move_list);
        test("Test 1", ia1, ia2, Game::Result::win2, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "g6v",
            "e5", "e4",
            "e6", "d4",
            "c3h", "a3h",
            "e7", "e7h",
            "d6v", "d4v",
            "e8", "d3", // e8 illegal because of "e7h" !
            "e9", "d2"
        };
        std::pair<Move, Move> lastMove{ "e8(illegal)", "" };

        auto ia1 = new IA_linear("*Player 1*", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("*Player 2*", Color::white, BoardPosition("e9"), move_list);
        test("Test 2", ia1, ia2, Game::Result::win2, lastMove);
        delete ia1, ia2;
    }

    {
        // Meet on center and create closed square around player and then jump each other.
        // Wall "c4v" is ILLEGAL MOVE !
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "d3h", "e5",
            "e4v", "d5h",
            "c4v", "d4", // Illegal c4v, pawn is in jail !
            "d5", "e5",
        };
        std::pair<Move, Move> lastMove{ "c4v(illegal)", "" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 3", ia1, ia2, Game::Result::win2, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e3h", "e5",
            "g6v", "d4",
            "e5", "d3",
            "e6", "d2",
            "e7", "d1", // Player 2 wins, end of game.
            "e8", "e1",
        };
        std::pair<Move, Move> lastMove{ "e7", "d1" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 4", ia1, ia2, Game::Result::win2, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "b5v", "b6v",  // Illegal b6v, wall cannot use b6v twice !
            "e3", "e7",
        };
        std::pair<Move, Move> lastMove{ "b5v", "b6v(illegal)" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 5", ia1, ia2, Game::Result::win1, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "f1", "h5v",
            "e3h", "b8v",
            "d4v", "d9",
            "a4h", "c9",
            "g1", "b9",  // Illegal b9, cannot jump over the wall b8v !
            "f1", "c9",
        };
        std::pair<Move, Move> lastMove{ "g1", "b9(illegal)" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test 6", ia1, ia2, Game::Result::win1, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e5", "e5", // Illegal move of player 2, e5 is not empty.
            "g6v", "d4",
        };
        std::pair<Move, Move> lastMove{ "e5", "e5(illegal)" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 1", ia1, ia2, Game::Result::win1, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "e8",
            "e3", "e7",
            "e4", "e6",
            "e5", "e5v",
            "c6h", "d6h",  // Illegal d6h, wall cannot use d6h twice !
            "f6", "e5",
            "f7", "e4",
        };
        std::pair<Move, Move> lastMove{ "c6h", "d6h(illegal)" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 2", ia1, ia2, Game::Result::win1, lastMove);
        delete ia1, ia2;
    }

    {
        std::vector<std::string> move_list{
            "e2", "i1", // Illegal move, i1 is too far from e9.
            "e3", "e7",
        };
        std::pair<Move, Move> lastMove{ "e2", "i1(illegal)" };

        auto ia1 = new IA_linear("Player 1", Color::black, BoardPosition("e1"), move_list);
        auto ia2 = new IA_linear("Player 2", Color::white, BoardPosition("e9"), move_list);
        test("Test Cyprien 3", ia1, ia2, Game::Result::win1, lastMove);
        delete ia1, ia2;
    }
}

void test_BoardPosition()
{
    std::cout << "test_BoardPosition() : START" << std::endl;
    {
        BoardPosition position(5, 9);
        std::cout << "Position : " << position << std::endl;
        assert(BoardPosition(5, 9) == position);
    }

    {
        BoardPosition position("e1");
        std::cout << "Position : " << position << std::endl;
        assert(BoardPosition(5, 1) == position);
    }

    {
        PawnPosition pawnPosition(5, 9, "Gerard");
        std::cout << "PawnPosition : " << pawnPosition << std::endl;
        assert(BoardPosition(5, 9) == pawnPosition);
        assert("Gerard" == pawnPosition.playerName());
    }

    {
        WallPosition wallPosition(5, 9, WallPosition::Direction::horizontal);
        std::cout << "WallPosition : " << wallPosition << std::endl;
        assert(BoardPosition(5, 9) == wallPosition);
        assert(WallPosition::Direction::horizontal == wallPosition.direction());
    }

    {
        WallPosition wallPosition(6, 8, "v");
        std::cout << "WallPosition2 : " << wallPosition << std::endl;
        assert(BoardPosition(6, 8) == wallPosition);
        assert(WallPosition::Direction::vertical == wallPosition.direction());
    }

    {
        Move illegal1("e1");
        illegal1.setIllegal();
        std::cout << "PawnPosition illegal : " << illegal1 << std::endl;
        std::ostringstream oss;
        oss << illegal1;
        Move illegal2(oss.str());
        std::cout << "Move illegal2 : " << illegal1 << std::endl;
        assert(illegal1 == illegal2);
    }

    {
        Move illegal1("i9");
        illegal1.setIllegal();
        std::cout << "PawnPosition illegal : " << illegal1 << std::endl;
        std::ostringstream oss;
        oss << illegal1;
        Move illegal2(oss.str());
        std::cout << "Move illegal2 : " << illegal1 << std::endl;
        assert(illegal1 == illegal2);
    }

    {
        Move illegal1("e1h");
        illegal1.setIllegal();
        std::cout << "WallPosition illegal : " << illegal1 << std::endl;
        std::ostringstream oss;
        oss << illegal1;
        Move illegal2(oss.str());
        std::cout << "WallPosition illegal2 : " << illegal1 << std::endl;
        assert(illegal1 == illegal2);
    }

    {
        Move illegal1("e1v");
        illegal1.setIllegal();
        std::cout << "WallPosition illegal : " << illegal1 << std::endl;
        std::ostringstream oss;
        oss << illegal1;
        Move illegal2(oss.str());
        std::cout << "WallPosition illegal2 : " << illegal1 << std::endl;
        assert(illegal1 == illegal2);
    }
    std::cout << "test_BoardPosition() : END\n\n";
}

void test_Board()
{
    std::cout << "test_Board() : START\n\n";
    {
        Board board;

        Move michou = PawnPosition(5, 1, "Michou", Color::black); board.add(michou);
        michou = PawnPosition(5, 2, "Michou"); board.add(michou);
        Move gerard = PawnPosition(5, 9, "Gerard", Color::white); board.add(gerard);
        assert(board.pawns().size() == 2);
        assert(board.existsPawn({ 5, 2 }));
        assert(board.existsPawn({ 5, 9 }));

        Move wallPosition1 = WallPosition(1, 8, "h"); board.add(wallPosition1);
        Move wallPosition2 = WallPosition(1, 1, "h"); board.add(wallPosition2);
        Move wallPosition3 = WallPosition(4, 8, "v"); board.add(wallPosition3);
        Move wallPosition4 = WallPosition(5, 1, "v"); board.add(wallPosition4);
        Move wallPosition5 = WallPosition(5, 4, "h"); board.add(wallPosition5);
        Move wallPosition6 = WallPosition(4, 4, "v"); board.add(wallPosition6);
        assert(board.walls().size() == 6);
        assert(board.existsWall({ 1, 8, "h" }));
        assert(board.existsWall({ 1, 1, "h" }));
        assert(board.existsWall({ 4, 8, "v" }));
        assert(board.existsWall({ 5, 1, "v" }));
        assert(board.existsWall({ 5, 4, "h" }));
        assert(board.existsWall({ 4, 4, "v" }));

        std::cout << "Board : \n"
            << board << std::endl;
    }
    std::cout << "test_Board() : END\n\n";
}

void test_Replay()
{
    std::cout << "test_Replay() : START\n\n";
    {
        auto test = [](std::string title, Player *p1, Player *p2, Game::Result result,
            std::pair<Move, Move>& last_move, bool showMoves = false)->std::string {
            std::cout << "test_Game(" << title << ") : START" << std::endl;
            Game game;

            game.chooseReferee();
            game.choosePlayers(p1, p2);
            game.showMoves(showMoves);
            game.launch();
            assert(game.getResult() == Game::Result::inProgress);

            while (game.getResult() == Game::Result::inProgress)
                game.move();

            assert(last_move == *game.moves().rbegin());
            assert(game.getResult() == result);
            std::cout << game;
            std::cout << "Game result : " << game.getResult() << std::endl;
            std::cout << "Save game to file : " << game.save() << std::endl;
            std::cout << "test_Game(" << title << ") : END\n\n";
            return game.filename();
        };

        {
            std::cout << "Play a game" << std::endl;
            // https://quoridorstrats.files.wordpress.com/2014/09/game-full-with-notation1.png
            //   Only legal moves.
            std::vector<std::string> move_list{
                "e2", "e8",
                "e3", "e7",
                "e4", "e6",
                "e3h", "g6v",
                "e5", "e4",
                "e6", "d4",
                "c3h", "a3h",
                "e7", "e7h",
                "d6v", "d4v" // Game not over, player 1 resigns
            };
            std::pair<Move, Move> lastMove{ "(resigns)", "" };

            auto ia1 = new IA_linear("*IA_1: Player 1*", Color::black, BoardPosition("e1"), move_list);
            auto ia2 = new IA_linear("*IA_1: Player 2*", Color::white, BoardPosition("e9"), move_list);
            auto filename = test("Test REPLAY 1", ia1, ia2, Game::Result::win2, lastMove);
            delete ia1, ia2;

            std::cout << "Replay : START" << std::endl;
            notation replayFile(filename);
            Game *replay(replayFile);
            replay->replay();
            std::cout << *replay;
            std::cout << "Replay : END" << std::endl;
            delete replay;
        }
    }
    std::cout << "test_Replay() : END\n\n";
}
