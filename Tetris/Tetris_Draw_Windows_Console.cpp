#include "Tetris_Draw_Windows_Console.h"


void Tetris_Draw_Windows_Console::draw(std::vector<std::vector<Field>> game_field)
{
    COORD con_coord = coord_game_field;
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
                con_coord.X = coord_game_field.X + j + 1;
                SetConsoleTextAttribute(screen_buffer_handle, color);
                SetConsoleCursorPosition(screen_buffer_handle, con_coord);
                WriteConsoleA(screen_buffer_handle, buffer, 1, NULL, NULL);
            }
        }
        con_coord.Y++;
    }
    old_game_field = game_field;    // saving current game field to detect changes in the next call
}

void Tetris_Draw_Windows_Console::draw_frame()
{
    /* 1. draw preview label */
    std::string label("PREVIEW");
    COORD con_coord = { 1, 6 };

    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    WriteConsoleA(screen_buffer_handle, label.c_str(), (DWORD)label.length(), NULL, NULL);

    /* 2. draw stats */
    update_stats({ "", 0, 1, 0 });

    /* 3. draw game field */
    std::string field(field_width, ' ');
    con_coord = coord_game_field;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    for (int i = 0; i < field_height; i++)
    {
        WriteConsoleA(screen_buffer_handle, "+", 1, NULL, NULL);
        WriteConsoleA(screen_buffer_handle, field.c_str(), field_width, NULL, NULL);
        WriteConsoleA(screen_buffer_handle, "+", 1, NULL, NULL);

        con_coord.Y++;
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    }


}

void Tetris_Draw_Windows_Console::update_stats(Tetris_Stats_entry stats)
{
    std::string lines = "LINES: " + std::to_string(stats.lines);
    std::string level = "LEVEL: " + std::to_string(stats.level);
    std::string score = "SCORE: " + std::to_string(stats.score);

    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    
    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_lines);
    WriteConsoleA(screen_buffer_handle, lines.c_str(), (DWORD)lines.length(), NULL, NULL);
    
    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_level);
    WriteConsoleA(screen_buffer_handle, level.c_str(), (DWORD)level.length(), NULL, NULL);

    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_score);
    WriteConsoleA(screen_buffer_handle, score.c_str(), (DWORD)score.length(), NULL, NULL);

}

void Tetris_Draw_Windows_Console::update_preview(TetrominoKind kind)
{
    /* reset current preview */
    std::string t[2] = { {"    "},{"    "} };
    
    WORD color = minoToColor(TetrominoKind::none);
    SetConsoleTextAttribute(screen_buffer_handle, color);

    COORD con_coord = coord_preview_tetromino;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[0].c_str(), (DWORD)t[0].length(), NULL, NULL);
    
    con_coord.Y++;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[0].c_str(), (DWORD)t[0].length(), NULL, NULL);

    /* draw new preview */
    switch (kind)
    {
    case TetrominoKind::I: t[0] = "####"; t[1] = "";   break;
    case TetrominoKind::J: t[0] = "#"; t[1] = "###";   break;
    case TetrominoKind::L: t[0] = "  #"; t[1] = "###"; break;
    case TetrominoKind::O: t[0] = "##"; t[1] = "##";   break;
    case TetrominoKind::S: t[0] = " ##"; t[1] = "##";  break;
    case TetrominoKind::T: t[0] = " #"; t[1] = "###";  break;
    case TetrominoKind::Z: t[0] = "##"; t[1] = " ##";  break;
    }

    color = minoToColor(kind);
    SetConsoleTextAttribute(screen_buffer_handle, color);

    con_coord = coord_preview_tetromino;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[0].c_str(), (DWORD)t[0].length(), NULL, NULL);

    con_coord.Y++;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[1].c_str(), (DWORD)t[1].length(), NULL, NULL);
}

