#ifndef _TETRIS_DRAW_WINDOWS_CONSOLE_
#define _TETRIS_DRAW_WINDOWS_CONSOLE_

#include "Tetris_Draw.h"
#include <Windows.h>
#include <string>

enum class COLOR { Black = 0, Blue = 1, Green = 2, Cyan = 11, Red = 4, Magenta = 13, Orange = 12, Yellow = 14, White = 15 };

class Tetris_Draw_Windows_Console : public Tetris_Draw
{
public:
    Tetris_Draw_Windows_Console()
        :screen_buffer_handle(CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
        old_game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width)))
    {
        SetConsoleMode(screen_buffer_handle, ENABLE_PROCESSED_OUTPUT);
        SetConsoleActiveScreenBuffer(screen_buffer_handle);

        CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
        SetConsoleCursorInfo(screen_buffer_handle, &cursor);    // disable cursor

        draw_frame();
    }

    void draw(std::vector<std::vector<Field>>) override final;
    void draw_frame() override final;
    void update_stats(Tetris_Stats_entry) override final;
    void update_preview(TetrominoKind) override final;

private:
    HANDLE screen_buffer_handle;
    std::vector<std::vector<Field>> old_game_field;     // store the last given game field, in the next call of draw(), redraw only updated data fields

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
