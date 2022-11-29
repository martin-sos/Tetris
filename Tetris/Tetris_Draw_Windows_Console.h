#ifndef _TETRIS_DRAW_WINDOWS_CONSOLE_
#define _TETRIS_DRAW_WINDOWS_CONSOLE_

#if (defined (_WIN32) || defined (_WIN64))

#include "Tetris_Draw.h"
#include <windows.h>
#include <string>

enum class COLOR { Black = 0, Blue = 1, Green = 2, Red = 4, Gray = 8, Cyan = 11, Orange = 12, Magenta = 13, Yellow = 14, White = 15 };

class Tetris_Draw_Windows_Console : public Tetris_Draw
{
public:
    Tetris_Draw_Windows_Console()
        :screen_buffer_handle(CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
        old_game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width)))
    {
        // setup console: set buffer, disable cursor
        SetConsoleMode(screen_buffer_handle, ENABLE_PROCESSED_OUTPUT);
        SetConsoleActiveScreenBuffer(screen_buffer_handle);
        CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
        SetConsoleCursorInfo(screen_buffer_handle, &cursor);
    }

    Tetris_Draw_Windows_Console(const Tetris_Draw_Windows_Console&) = delete;
    Tetris_Draw_Windows_Console(Tetris_Draw_Windows_Console&&) = delete;
    Tetris_Draw_Windows_Console& operator=(const Tetris_Draw_Windows_Console&) = delete;
    Tetris_Draw_Windows_Console& operator=(Tetris_Draw_Windows_Console&&) = delete;

    ~Tetris_Draw_Windows_Console()
    {
        CloseHandle(screen_buffer_handle);
    }

    void draw_scene(std::vector<std::vector<Field>>) override final;
    void draw_layout() override final;
    void update_stats(Tetris_Stats_entry) override final;
    void update_preview(TetrominoKind) override final;
    void draw_highscores(std::vector<Tetris_Stats_entry>) override final;
    void draw_game_over() override final;

private:
    HANDLE screen_buffer_handle;
    std::vector<std::vector<Field>> old_game_field;     // store the last given game field, in the next call of draw(), redraw only updated data fields

    static constexpr COORD coord_preview_label      = { 5, 8 };
    static constexpr COORD coord_preview_tetromino  = { coord_preview_label.X + 2, coord_preview_label.Y + 2 };
    static constexpr COORD coord_game_frame         = { coord_preview_label.X + 13, coord_preview_label.Y - 5 };
    static constexpr COORD coord_game_field         = { coord_game_frame.X + 1, coord_game_frame.Y };
    static constexpr COORD coord_controls           = { coord_preview_label.X, coord_game_frame.Y + field_height + 1 };
    static constexpr COORD coord_stats_lines        = { coord_game_field.X + field_width + 8, coord_preview_label.Y };
    static constexpr COORD coord_stats_level        = { coord_stats_lines.X, coord_stats_lines.Y + 2 };
    static constexpr COORD coord_stats_score        = { coord_stats_lines.X, coord_stats_level.Y + 2 };
    static constexpr COORD coord_stats_highscore    = { coord_stats_lines.X + 15, coord_stats_lines.Y };

 /* with the coordinates above draw_layout creates a display as below



                  |          |
                  |          |
                  |          |
                  |          |
                  |          |
     PREVIEW      |    #     |       LINES: 0                    *** H I G H S C O R E S ***
                  |   ###    |
       ####       |          |       LEVEL: 1         Rank    |  Name    |  Lines   |  Level   |  Score
                  |          |                      ------------------------------------------------------
                  |          |       SCORE: 0       ..1.......|..Player 1|..142.....|..15......|..1420....
                  |          |                      ..2.......|..Player 1|..132.....|..14......|..1320....
                  |          |                      ..3.......|..Player 1|..106.....|..11......|..1060....
                  |          |                      ..4.......|..Player 1|..93......|..10......|..930.....
                  |          |                      ..5.......|..Player 1|..91......|..10......|..910.....
                  |          |                      ..6.......|..Player 1|..78......|..4.......|..780.....
                  |          |                      ..7.......|..Player 1|..59......|..6.......|..590.....
                  |          |                      ..8.......|..Player 1|..58......|..6.......|..580.....
                  |          |                      ..9.......|..Player 1|..53......|..6.......|..530.....
                  |          |                      ..10......|..Player 1|..53......|..6.......|..530.....
                  |          |
                  ''''''''''''
     CONTROLS
     s     - start game  | esc  - pause | q - quit
     left  - shift left  | up   - rotate clockwise
     right - shift right | down - rotate counter-clockwise
     space - hard drop   | g    - ghost on/off
 */


};


#endif // (defined (_WIN32) || defined (_WIN64))
#endif // !_TETRIS_DRAW_WINDOWS_CONSOLE_
