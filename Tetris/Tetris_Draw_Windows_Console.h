#ifndef _TETRIS_DRAW_WINDOWS_CONSOLE_
#define _TETRIS_DRAW_WINDOWS_CONSOLE_

#include "Tetris_Draw.h"
#include <Windows.h>
#include <string>

enum class COLOR { Black = 0, Blue = 1, Green = 2, Red = 4, Gray = 8, Cyan = 11, Orange = 12, Magenta = 13, Yellow = 14, White = 15 };

class Tetris_Draw_Windows_Console : public Tetris_Draw
{
public:
    Tetris_Draw_Windows_Console()
        :screen_buffer_handle(CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
        old_game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width)))
    {
        // define location of preview, game field, current stats and overall highscores 
        coord_preview_label = { 1, 6 };
        coord_preview_tetromino = { 3, 8 };
        coord_game_field = { 10, 0 };
        coord_stats_lines =     { coord_game_field.X + field_width + 8, 6 };
        coord_stats_level =     { coord_stats_lines.X, coord_stats_lines.Y + 2 };
        coord_stats_score =     { coord_stats_lines.X, coord_stats_level.Y + 2 };
        coord_stats_highscore = { coord_stats_lines.X + 15, coord_stats_lines.Y };

        // setup console: set buffer, disable cursor
        SetConsoleMode(screen_buffer_handle, ENABLE_PROCESSED_OUTPUT);
        SetConsoleActiveScreenBuffer(screen_buffer_handle);
        CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
        SetConsoleCursorInfo(screen_buffer_handle, &cursor);

        draw_layout();
    }

    void draw_scene(std::vector<std::vector<Field>>) override final;
    void draw_layout() override final;
    void update_stats(Tetris_Stats_entry) override final;
    void update_preview(TetrominoKind) override final;
    void draw_highscores(std::vector<Tetris_Stats_entry>) override final;

private:
    HANDLE screen_buffer_handle;
    std::vector<std::vector<Field>> old_game_field;     // store the last given game field, in the next call of draw(), redraw only updated data fields

    COORD coord_preview_label;
    COORD coord_preview_tetromino;
    COORD coord_game_field;
    COORD coord_stats_lines;
    COORD coord_stats_level;
    COORD coord_stats_score;
    COORD coord_stats_highscore;

    char minoToChar(TetrominoKind mino)
    {
        switch (mino)
        {
        case TetrominoKind::I:      return '#';
        case TetrominoKind::J:      return '#';
        case TetrominoKind::L:      return '#';
        case TetrominoKind::O:      return '#';
        case TetrominoKind::S:      return '#';
        case TetrominoKind::T:      return '#';
        case TetrominoKind::Z:      return '#';
        case TetrominoKind::none:   return ' ';
        default:                    return '?';
        }
    }

    WORD minoToColor(TetrominoKind mino)
    {
        switch (mino)
        {
        case TetrominoKind::I:      return (WORD)COLOR::Cyan;
        case TetrominoKind::J:      return (WORD)COLOR::Blue;
        case TetrominoKind::L:      return (WORD)COLOR::Orange;
        case TetrominoKind::O:      return (WORD)COLOR::Yellow;
        case TetrominoKind::S:      return (WORD)COLOR::Green;
        case TetrominoKind::T:      return (WORD)COLOR::Magenta;
        case TetrominoKind::Z:      return (WORD)COLOR::Red;
        case TetrominoKind::none:   return (WORD)COLOR::Black;
        default:                    return (WORD)COLOR::White;
        }
    }
};


#endif // !_TETRIS_DRAW_WINDOWS_CONSOLE_
