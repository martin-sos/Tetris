#include "Tetris_Draw_Windows_Console.h"

#if (defined (_WIN32) || defined (_WIN64))

constexpr COORD Tetris_Draw_Windows_Console::coord_preview_label;
constexpr COORD Tetris_Draw_Windows_Console::coord_preview_tetromino;
constexpr COORD Tetris_Draw_Windows_Console::coord_game_frame;
constexpr COORD Tetris_Draw_Windows_Console::coord_game_field;
constexpr COORD Tetris_Draw_Windows_Console::coord_controls;
constexpr COORD Tetris_Draw_Windows_Console::coord_stats_lines;
constexpr COORD Tetris_Draw_Windows_Console::coord_stats_level;
constexpr COORD Tetris_Draw_Windows_Console::coord_stats_score;
constexpr COORD Tetris_Draw_Windows_Console::coord_stats_highscore;

static inline constexpr char minoToChar(const TetrominoKind mino)
{
    switch (mino)
    {
    case TetrominoKind::I:
    case TetrominoKind::J:
    case TetrominoKind::L:
    case TetrominoKind::O:
    case TetrominoKind::S:
    case TetrominoKind::T:
    case TetrominoKind::Z:
    case TetrominoKind::Ghost:  return '#';
    case TetrominoKind::Pause:  return '-';
    case TetrominoKind::none:   return ' ';
    default:                    return '?';
    }
}

static inline constexpr WORD minoToColor(const TetrominoKind mino)
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
    case TetrominoKind::Ghost:  return (WORD)COLOR::Gray;
    case TetrominoKind::Pause:  return (WORD)COLOR::Gray;
    case TetrominoKind::none:   return (WORD)COLOR::Black;
    default:                    return (WORD)COLOR::White;
    }
}

static inline void draw(HANDLE scr_buf, COORD coord, WORD color, const char *c_str, DWORD length)
{
    SetConsoleTextAttribute(scr_buf, color);
    SetConsoleCursorPosition(scr_buf, coord);
    WriteConsoleA(scr_buf, c_str, length, NULL, NULL);
}

void Tetris_Draw_Windows_Console::draw_scene(std::vector<std::vector<Field>> game_field)
{
    COORD con_coord = coord_game_field;
    for (int i = field_height - 1; i >= 0; i--)
    {
        for (int j = 0; j < field_width; j++)
        {
            Field f = game_field[i][j];
            if (f.mino != old_game_field[i][j].mino ) // draw only what has changed 
            {
                WORD color = minoToColor(f.mino);
                con_coord.X = coord_game_field.X + j;
                char buffer = minoToChar(f.mino);
                draw(screen_buffer_handle, con_coord, color, &buffer, 1);
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
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::White, label.c_str(), (DWORD)label.length());

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
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::White, controls.c_str(), (DWORD)controls.length());
    
    con_coord.Y++;
    controls = "s     - start game  | esc  - pause | q - quit  ";
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, controls.c_str(), (DWORD)controls.length());

    con_coord.Y++;
    controls = "left  - shift left  | up   - rotate clockwise";
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, controls.c_str(), (DWORD)controls.length());
    
    con_coord.Y++;
    controls = "right - shift right | down - rotate counter-clockwise";
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, controls.c_str(), (DWORD)controls.length());

    con_coord.Y++;
    controls = "space - hard drop   | g    - ghost on/off";
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, controls.c_str(), (DWORD)controls.length());
}

void Tetris_Draw_Windows_Console::update_stats(Tetris_Stats_entry stats)
{
    
    std::string number = std::to_string(stats.lines);
    std::string text = "LINES: " + number;
    std::string spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(screen_buffer_handle, coord_stats_lines, (WORD)COLOR::White, text.c_str(), (DWORD)text.length());
    
    number = std::to_string(stats.level);
    text = "LEVEL: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(screen_buffer_handle, coord_stats_level, (WORD)COLOR::White, text.c_str(), (DWORD)text.length());

    number = std::to_string(stats.score);
    text = "SCORE: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(screen_buffer_handle, coord_stats_score, (WORD)COLOR::White, text.c_str(), (DWORD)text.length());
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
    COORD con_coord = coord_preview_tetromino;
    draw(screen_buffer_handle, con_coord, color, t[0].c_str(), (DWORD)t[0].length());

    con_coord.Y++;
    draw(screen_buffer_handle, con_coord, color, t[1].c_str(), (DWORD)t[1].length());
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
    COORD con_coord = coord_stats_highscore;
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Yellow, highscores.c_str(), (DWORD)highscores.length());

    // print row names 
    con_coord.Y += 2;
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, title_row.c_str(), (DWORD)title_row.length());

    // print dash
    con_coord.Y++;
    draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, title_dash.c_str(), (DWORD)title_dash.length());

    // print highscores
    for (int i = 0; i < stats.size(); i++)
    { 
        std::string row_entry = "";
        std::string fill;
        
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
        draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, row_entry.c_str(), (DWORD)row_entry.length());
    }
}

void Tetris_Draw_Windows_Console::draw_game_over()
{
    static const std::string game_over_screen[field_height] = {
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

    static const std::string fill(field_width, 'X');

    COORD con_coord = coord_game_field; 
    for (int i = 0; i < field_height; i++)
    {
        draw(screen_buffer_handle, con_coord, (WORD)COLOR::Gray, fill.c_str(), field_width);
        Sleep(20);
        con_coord.Y++;
    }

    con_coord = coord_game_field;
    for (int i = 0; i < field_height; i++)
    {
        draw(screen_buffer_handle, con_coord, (WORD)COLOR::Yellow, game_over_screen[i].c_str(), (DWORD)game_over_screen[i].length());
        Sleep(20);
        con_coord.Y++;
    }

    Sleep(1000);
}

#endif // (defined (_WIN32) || defined (_WIN64))
