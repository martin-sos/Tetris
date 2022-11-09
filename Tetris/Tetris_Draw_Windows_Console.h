#ifndef _TETRIS_DRAW_WINDOWS_CONSOLE_
#define _TETRIS_DRAW_WINDOWS_CONSOLE_

#include "Tetris_Draw.h"
#include <Windows.h>
#include <string>

class Tetris_Draw_Windows_Console : public Tetris_Draw
{
public:
    Tetris_Draw_Windows_Console()
    {
        old_game_field = std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width));
        screen_buffer_handle = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleMode(screen_buffer_handle, ENABLE_PROCESSED_OUTPUT);
        SetConsoleActiveScreenBuffer(screen_buffer_handle);

        CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
        SetConsoleCursorInfo(screen_buffer_handle, &cursor);
        
        draw_frame();
    }

    void draw(std::vector<std::vector<Field>> game_field) override final
    {
        COORD con_coord = { 0, 0 };
        for (int i = field_height - 1; i >= 0; i--)
        {
            char buffer[1];
            for (int j = 0; j < field_width; j++)
            {
                Field f = game_field[i][j];
                if (f.mino != old_game_field[i][j].mino)
                {
                    buffer[0] = minoToChar(f.mino);
                    con_coord.X = j + 1;
                    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
                    WriteConsoleA(screen_buffer_handle, buffer, 1, NULL, NULL);
                }
            }
            con_coord.X = 0; con_coord.Y++;
        }
        old_game_field = game_field;
    }


    void draw_frame() override final
    {
        std::string field(field_width, ' ');
        COORD con_coord = { 0, 0 };
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
        for (int i = 0; i < field_height; i++)
        {
            WriteConsoleA(screen_buffer_handle, "+", 1, NULL, NULL);
            WriteConsoleA(screen_buffer_handle, field.c_str(), field_width, NULL, NULL);
            WriteConsoleA(screen_buffer_handle, "+", 1, NULL, NULL);

            con_coord.X = 0; con_coord.Y++;
            SetConsoleCursorPosition(screen_buffer_handle, con_coord);
        }
    }


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

};

#endif // !_TETRIS_DRAW_WINDOWS_CONSOLE_
