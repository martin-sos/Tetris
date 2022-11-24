#ifndef _TETRIS_DRAW_LINUX_CONSOLE_H_
#define _TETRIS_DRAW_LINUX_CONSOLE_H_

#if (defined __unix__)

#include <ncurses.h>
#include <signal.h>
#include <iostream>
#include "Tetris_Draw.h"
#include "Tetris.h"

struct COORD {
    int X;
    int Y;
};

enum class COLOR { Black = 0, Blue = 1, Green = 2, Red = 4, Gray = 8, Cyan = 11, Orange = 12, Magenta = 13, Yellow = 14, White = 15 };

class Tetris_Draw_Linux_Console : public Tetris_Draw
{
public:
    Tetris_Draw_Linux_Console()
        :win(initscr())              // initialize the screen
    {
        /* setup curses */
        (void) cbreak();        // disable line buffering and take one character at a time
        (void) noecho();        // do not echo input back to screen
        curs_set(0);            // disbale the cursor
        clearok(win, TRUE);     // clear and repaint the screen from scratch on call to wrefresh
        keypad (win, TRUE);     // enable function keys like ESC, down, up, left, right

        if(has_colors()) // TODO: make a try block here
        {
            start_color();
            init_pair((short)COLOR::Black,   (short)COLOR::Black,   COLOR_BLACK);
            init_pair((short)COLOR::Blue,    (short)COLOR::Blue,    COLOR_BLACK);
            init_pair((short)COLOR::Green,   (short)COLOR::Green,   COLOR_BLACK);
            init_pair((short)COLOR::Red,     (short)COLOR::Red,     COLOR_BLACK);
            init_pair((short)COLOR::Gray,    (short)COLOR::Gray,    COLOR_BLACK);
            init_pair((short)COLOR::Cyan,    (short)COLOR::Cyan,    COLOR_BLACK);
            init_pair((short)COLOR::Orange,  (short)COLOR::Orange,  COLOR_BLACK);
            init_pair((short)COLOR::Magenta, (short)COLOR::Magenta, COLOR_BLACK);
            init_pair((short)COLOR::Yellow,  (short)COLOR::Yellow,  COLOR_BLACK);
            init_pair((short)COLOR::White,   (short)COLOR::White,   COLOR_BLACK);
        }
        else
        {
            std::cout << "Terminal does not support colors...exiting" << std::endl;
            exit(1);
        }
    }

    ~Tetris_Draw_Linux_Console()
    {
        endwin();       // exit the curses environment
    }

    void draw_scene(std::vector<std::vector<Field>>) override final;
    void draw_layout() override final;
    void update_stats(Tetris_Stats_entry) override final;
    void update_preview(TetrominoKind) override final;
    void draw_highscores(std::vector<Tetris_Stats_entry>) override final;
    void draw_game_over() override final;

    void detectKeyboardInputLinux(Tetris* T);

private:
    std::vector<std::vector<Field>> old_game_field;     // store the last given game field, in the next call of draw(), redraw only updated data fields
    WINDOW *win;
    static constexpr COORD coord_preview_label      = { 5, 8 };
    static constexpr COORD coord_preview_tetromino  = { coord_preview_label.X + 2, coord_preview_label.Y + 2 };
    static constexpr COORD coord_game_frame         = { coord_preview_label.X + 13, coord_preview_label.Y - 5 };
    static constexpr COORD coord_game_field         = { coord_game_frame.X + 1, coord_game_frame.Y };
    static constexpr COORD coord_controls           = { coord_preview_label.X, coord_game_frame.Y + field_height + 1 };
    static constexpr COORD coord_stats_lines        = { coord_game_field.X + field_width + 8, coord_preview_label.Y };
    static constexpr COORD coord_stats_level        = { coord_stats_lines.X, coord_stats_lines.Y + 2 };
    static constexpr COORD coord_stats_score        = { coord_stats_lines.X, coord_stats_level.Y + 2 };
    static constexpr COORD coord_stats_highscore    = { coord_stats_lines.X + 15, coord_stats_lines.Y };
};
#endif // (defined __unix__)
#endif // _TETRIS_DRAW_LINUX_CONSOLE_H_
