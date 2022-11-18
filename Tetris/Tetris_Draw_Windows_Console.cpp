#include "Tetris_Draw_Windows_Console.h"

#if (defined (_WIN32) || defined (_WIN64))

void Tetris_Draw_Windows_Console::draw_scene(std::vector<std::vector<Field>> game_field)
{
    COORD con_coord = coord_game_field;
    for (int i = field_height - 1; i >= 0; i--)
    {
        char buffer;
        for (int j = 0; j < field_width; j++)
        {
            Field f = game_field[i][j];
            if (f.mino != old_game_field[i][j].mino ) // draw only what has changed 
            {
                WORD color = minoToColor(f.mino);
                SetConsoleTextAttribute(screen_buffer_handle, color);
                
                con_coord.X = coord_game_field.X + j;
                SetConsoleCursorPosition(screen_buffer_handle, con_coord);
                
                buffer = minoToChar(f.mino);
                WriteConsoleA(screen_buffer_handle, &buffer, 1, NULL, NULL);
            }
        }
        con_coord.Y++;
    }
    old_game_field = game_field;    // saving current game field to detect changes in the next scene
}

void Tetris_Draw_Windows_Console::draw_layout()
{
    /* 1. draw preview label */
    std::string label("PREVIEW");
    COORD con_coord = coord_preview_label;

    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    WriteConsoleA(screen_buffer_handle, label.c_str(), (DWORD)label.length(), NULL, NULL);

    /* 2. draw stats */
    update_stats({ "", 0, 1, 0 });

    /* 3. draw game field */
    std::string field(field_width, ' ');
    con_coord = coord_game_frame;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    for (int i = 0; i < field_height; i++)
    {
        WriteConsoleA(screen_buffer_handle, "|", 1, NULL, NULL);
        WriteConsoleA(screen_buffer_handle, field.c_str(), field_width, NULL, NULL);
        WriteConsoleA(screen_buffer_handle, "|", 1, NULL, NULL);

        con_coord.Y++;
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    }
    
    field = std::string(field_width + 2, '\'');
    WriteConsoleA(screen_buffer_handle, field.c_str(), (DWORD)field.length(), NULL, NULL);

    /* 4. draw controls */    
    con_coord = coord_controls;
    std::string controls = "CONTROLS";
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    
    WriteConsoleA(screen_buffer_handle, controls.c_str(), (DWORD)controls.length(), NULL, NULL);

    
    con_coord.Y++;
    controls = "s     - start game  | esc  - pause | q - quit  ";
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Gray);
    WriteConsoleA(screen_buffer_handle, controls.c_str(), (DWORD)controls.length(), NULL, NULL);

    con_coord.Y++;
    controls = "left  - shift left  | up   - rotate clockwise";
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Gray);
    WriteConsoleA(screen_buffer_handle, controls.c_str(), (DWORD)controls.length(), NULL, NULL);
    
    con_coord.Y++;
    controls = "right - shift right | down - rotate counter-clockwise";
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, controls.c_str(), (DWORD)controls.length(), NULL, NULL);

    con_coord.Y++;
    controls = "space - hard drop   | g    - ghost on/off";
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Gray);
    WriteConsoleA(screen_buffer_handle, controls.c_str(), (DWORD)controls.length(), NULL, NULL);
}

void Tetris_Draw_Windows_Console::update_stats(Tetris_Stats_entry stats)
{
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::White);
    
    std::string number = std::to_string(stats.lines);
    std::string text = "LINES: " + number;
    std::string spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_lines);
    WriteConsoleA(screen_buffer_handle, text.c_str(), (DWORD)text.length(), NULL, NULL);
    
    number = std::to_string(stats.level);
    text = "LEVEL: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_level);
    WriteConsoleA(screen_buffer_handle, text.c_str(), (DWORD)text.length(), NULL, NULL);

    number = std::to_string(stats.score);
    text = "SCORE: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    SetConsoleCursorPosition(screen_buffer_handle, coord_stats_score);
    WriteConsoleA(screen_buffer_handle, text.c_str(), (DWORD)text.length(), NULL, NULL);
}

void Tetris_Draw_Windows_Console::update_preview(TetrominoKind kind)
{
    std::string t[2] ;
   
    /* draw new preview */
    switch (kind)
    {
    case TetrominoKind::I: t[0] = "####"; t[1] = "    "; break;
    case TetrominoKind::J: t[0] = "#   "; t[1] = "### "; break;
    case TetrominoKind::L: t[0] = "   #"; t[1] = " ###"; break;
    case TetrominoKind::O: t[0] = " ## "; t[1] = " ## "; break;
    case TetrominoKind::S: t[0] = "  ##"; t[1] = " ## "; break;
    case TetrominoKind::T: t[0] = " #  "; t[1] = "### "; break;
    case TetrominoKind::Z: t[0] = "##  "; t[1] = " ## "; break;
    case TetrominoKind::Pause: t[0] = "----"; t[1] = "----"; break;
    case TetrominoKind::none: t[0] = "    "; t[1] = "    "; break;
    }

    WORD color = minoToColor(kind);
    SetConsoleTextAttribute(screen_buffer_handle, color);

    COORD con_coord = coord_preview_tetromino;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[0].c_str(), (DWORD)t[0].length(), NULL, NULL);

    con_coord.Y++;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, t[1].c_str(), (DWORD)t[1].length(), NULL, NULL);
}

void Tetris_Draw_Windows_Console::draw_highscores(std::vector<Tetris_Stats_entry> stats)
{
    /* draws highscore like:
     
                      *** H I G H S C O R E S ***

          Rank    |   Name   |   Lines  |   Level  | Score
        ------------------------------------------------------
        ..1.......|..Player 1|..83......|..5.......|..830.....
        ..2.......|..Player 1|..70......|..6.......|..700.....
        ..3.......|..Player 1|..66......|..5.......|..660.....
        ..4.......|..Player 1|..56......|..5.......|..560.....
        ..5.......|..Player 1|..54......|..2.......|..540.....
        ..6.......|..gegeege.|..50......|..5.......|..500.....
        ..7.......|..Player 1|..45......|..4.......|..450.....
        ..8.......|..Player 1|..42......|..3.......|..420.....
        ..9.......|..Player 1|..40......|..5.......|..400.....
        ..10......|..Player 1|..35......|..3.......|..350.....
    */

    int col_width = 9;
    std::string highscores = "             *** H I G H S C O R E S ***";
    std::string rank  = "  Rank    |";
    std::string name  = "  Name    |";
    std::string lines = "  Lines   |";
    std::string level = "  Level   |";
    std::string score = "  Score   ";
    std::string title_row = rank + name + lines + level + score;
    std::string title_dash(title_row.length(), '-');

    // print title
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Yellow);
    COORD con_coord = coord_stats_highscore;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, highscores.c_str(), (DWORD)highscores.length(), NULL, NULL);

    // print row names 
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Gray);
    con_coord.Y += 2;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, title_row.c_str(), (DWORD)title_row.length(), NULL, NULL);

    // print dash
    con_coord.Y++;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    WriteConsoleA(screen_buffer_handle, title_dash.c_str(), (DWORD)title_dash.length(), NULL, NULL);

   
    // print highscores
    for (int i = 0; i < stats.size(); i++)
    { 
        std::string row_entry = "";
        std::string fill = "";
        
        rank = std::to_string(i + 1);
        fill = std::string(col_width - rank.length() - 1, '.');
        row_entry += ".." + rank + fill + "|";

        name = stats[i].name.substr(0, col_width - 1);
        fill = std::string(col_width - name.length() - 1, '.');
        row_entry += ".." + name + fill + "|";

        lines = std::to_string(stats[i].lines); 
        fill = std::string(col_width - lines.length() - 1, '.');
        row_entry += ".." + lines + fill + "|";

        level = std::to_string(stats[i].level);
        fill = std::string(col_width - level.length() - 1, '.');
        row_entry += ".." + level + fill + "|";

        score = std::to_string(stats[i].score);
        fill = std::string(col_width - score.length() - 1, '.');
        row_entry += ".." + score + fill;

        con_coord.Y++;
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
        WriteConsoleA(screen_buffer_handle, row_entry.c_str(), (DWORD)row_entry.length(), NULL, NULL);
    }

}

void Tetris_Draw_Windows_Console::draw_game_over()
{
    std::string fill(field_width, 'X');

    COORD con_coord = coord_game_field;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Gray);
    
    for (int i = 0; i < field_height; i++)
    {
        WriteConsoleA(screen_buffer_handle, fill.c_str(), field_width, NULL, NULL);
        Sleep(40);
        con_coord.Y++;
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    }

    std::string game_over_screen[field_height] = {
        {"          "},
        {"          "},
        {"          "},
        {"          "},
        {"          "},
        {"   GAME   "},
        {"    OVER  "},
        {"          "},
        {"          "},
        {"          "},
        {"          "},
        {" Please   "},
        {"   try    "},
        {"    again "},
        {"          "},
        {"          "},
        {"          "},
        {"          "},
        {"          "},
        {"          "}
    };

    con_coord = coord_game_field;
    SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    SetConsoleTextAttribute(screen_buffer_handle, (WORD)COLOR::Yellow);

    for (int i = 0; i < field_height; i++)
    {
        WriteConsoleA(screen_buffer_handle, game_over_screen[i].c_str(), (DWORD)game_over_screen[i].length(), NULL, NULL);
        Sleep(40);
        con_coord.Y++;
        SetConsoleCursorPosition(screen_buffer_handle, con_coord);
    }

    Sleep(1000);
}

#endif // (defined (_WIN32) || defined (_WIN64))
