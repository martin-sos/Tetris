#include "Tetris_Draw_Windows_Console.h"

void Tetris_Draw_Windows_Console::draw(std::vector<std::vector<Field>> game_field)
{
    COORD con_coord = { 0, 0 };
    for (int i = field_height - 1; i >= 0; i--)
    {
        char buffer[1];
        for (int j = 0; j < field_width; j++)
        {
            Field f = game_field[i][j];
            if (f.mino != old_game_field[i][j].mino) // draw only what has changed 
            {
                buffer[0] = minoToChar(f.mino);
                WORD color = minoToColor(f.mino);
                con_coord.X = j + 1;
                SetConsoleTextAttribute(screen_buffer_handle, color);
                SetConsoleCursorPosition(screen_buffer_handle, con_coord);
                WriteConsoleA(screen_buffer_handle, buffer, 1, NULL, NULL);
            }
        }
        con_coord.X = 0; con_coord.Y++;
    }
    old_game_field = game_field;    // saving current game field to detect changes in the next call
}

void Tetris_Draw_Windows_Console::draw_frame()
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