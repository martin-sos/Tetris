#include "Tetris_Draw_Linux_Console.h"
#include <boost/thread.hpp>
#define LINUX
#if ((defined LINUX) || defined (__linux__))


constexpr COORD Tetris_Draw_Linux_Console::coord_preview_label;
constexpr COORD Tetris_Draw_Linux_Console::coord_preview_tetromino;
constexpr COORD Tetris_Draw_Linux_Console::coord_game_frame;
constexpr COORD Tetris_Draw_Linux_Console::coord_game_field;
constexpr COORD Tetris_Draw_Linux_Console::coord_controls;
constexpr COORD Tetris_Draw_Linux_Console::coord_stats_lines;
constexpr COORD Tetris_Draw_Linux_Console::coord_stats_level;
constexpr COORD Tetris_Draw_Linux_Console::coord_stats_score;
constexpr COORD Tetris_Draw_Linux_Console::coord_stats_highscore;

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

static inline constexpr short minoToColor(const TetrominoKind mino)
{
    switch (mino)
    {
    case TetrominoKind::I:      return (short)COLOR::Cyan;
    case TetrominoKind::J:      return (short)COLOR::Blue;
    case TetrominoKind::L:      return (short)COLOR::Orange;
    case TetrominoKind::O:      return (short)COLOR::Yellow;
    case TetrominoKind::S:      return (short)COLOR::Green;
    case TetrominoKind::T:      return (short)COLOR::Magenta;
    case TetrominoKind::Z:      return (short)COLOR::Red;
    case TetrominoKind::Ghost:  return (short)COLOR::Gray;
    case TetrominoKind::Pause:  return (short)COLOR::Gray;
    case TetrominoKind::none:   return (short)COLOR::Black;
    default:                    return (short)COLOR::White;
    }
}

static inline void draw(WINDOW* win, COORD coord, short color, const char *c_str, int n)
{
    wmove(win, coord.Y, coord.X);
    wattron(win, COLOR_PAIR(color));
    waddnstr(win, c_str, n);
    wattroff(win, COLOR_PAIR(color));
    wrefresh(win);
}

void Tetris_Draw_Linux_Console::draw_scene(std::vector<std::vector<Field>> game_field)
{
    COORD con_coord = coord_game_field;

    for (int i = field_height - 1; i >= 0; i--)
    {
        for (int j = 0; j < field_width; j++)
        {
            Field f = game_field[i][j];
            if (f.mino != old_game_field[i][j].mino ) // draw only what has changed
            {
                short color = minoToColor(f.mino);
                con_coord.X = coord_game_field.X + j;
                char buffer = minoToChar(f.mino);
                draw(win, con_coord, color, &buffer, 1);
            }
        }
        con_coord.Y++;
    }
    old_game_field = game_field;    // saving current game field to detect changes in the next scene
}

void Tetris_Draw_Linux_Console::draw_layout()
{
    /* 1. draw preview label */
    std::string label("PREVIEW");
    COORD con_coord = coord_preview_label;
    draw(win, con_coord, (short)COLOR::White, label.c_str(), (int)label.length());

    /* 2. draw stats */
    update_stats({ "", 0, 1, 0 });

    /* 3. draw game field */
    std::string field(field_width, ' ');
    con_coord = coord_game_frame;
    wmove(win, con_coord.Y, con_coord.X);
    wattron(win, COLOR_PAIR((short)COLOR::White));
    for (int i = 0; i < field_height; i++)
    {
        waddch(win, '|');
        waddnstr(win, field.c_str(), field_width);  // TODO: this could be a move
        waddch(win, '|');
        con_coord.Y++;
        wmove(win, con_coord.Y, con_coord.X);
    }
    wattroff(win, COLOR_PAIR((short)COLOR::White));

    field = std::string(field_width + 2, '\'');
    draw(win, con_coord, (short)COLOR::White, field.c_str(), (int)field.length());

    /* 4. draw controls */
    con_coord = coord_controls;
    std::string controls = "CONTROLS";
    draw(win, con_coord, (short)COLOR::White, controls.c_str(), (int)controls.length());

    con_coord.Y++;
    controls = "s     - start game  | esc  - pause | q - quit  ";
    draw(win, con_coord, (short)COLOR::Gray, controls.c_str(), (int)controls.length());

    con_coord.Y++;
    controls = "left  - shift left  | up   - rotate clockwise";
    draw(win, con_coord, (short)COLOR::Gray, controls.c_str(), (int)controls.length());

    con_coord.Y++;
    controls = "right - shift right | down - rotate counter-clockwise";
    draw(win, con_coord, (short)COLOR::Gray, controls.c_str(), (int)controls.length());

    con_coord.Y++;
    controls = "space - hard drop   | g    - ghost on/off";
    draw(win, con_coord, (short)COLOR::Gray, controls.c_str(), (int)controls.length());
}

void Tetris_Draw_Linux_Console::update_stats(Tetris_Stats_entry stats)
{
    std::string number = std::to_string(stats.lines);
    std::string text = "LINES: " + number;
    std::string spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(win, coord_stats_lines, (short)COLOR::White, text.c_str(), (int)text.length());

    number = std::to_string(stats.level);
    text = "LEVEL: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(win, coord_stats_level, (short)COLOR::White, text.c_str(), (int)text.length());

    number = std::to_string(stats.score);
    text = "SCORE: " + number;
    spaces = std::string(coord_stats_highscore.X - coord_stats_lines.X - text.length(), ' ');
    text += spaces;
    draw(win, coord_stats_score, (short)COLOR::White, text.c_str(), (int)text.length());
}

void Tetris_Draw_Linux_Console::update_preview(TetrominoKind kind)
{
    std::string t[2];

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

    short color = minoToColor(kind);
    COORD con_coord = coord_preview_tetromino;
    draw(win, con_coord, color, t[0].c_str(), (int)t[0].length());

    con_coord.Y++;
    draw(win, con_coord, color, t[1].c_str(), (int)t[1].length());
}

void Tetris_Draw_Linux_Console::draw_highscores(std::vector<Tetris_Stats_entry> stats)
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
    draw(win, con_coord, (short)COLOR::Yellow, highscores.c_str(), (int)highscores.length());

    // print row names
    con_coord.Y += 2;
    draw(win, con_coord, (short)COLOR::Gray, title_row.c_str(), (int)title_row.length());

    // print dash
    con_coord.Y++;
    draw(win, con_coord, (short)COLOR::Gray, title_dash.c_str(), (int)title_dash.length());

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
        draw(win, con_coord, (short)COLOR::Gray, row_entry.c_str(), (int)row_entry.length());
    }
}

void Tetris_Draw_Linux_Console::draw_game_over()
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
        draw(win, con_coord, (short)COLOR::Gray, fill.c_str(), field_width);
        usleep(20*1000);    // 20ms
        con_coord.Y++;
    }

    con_coord = coord_game_field;
    for (int i = 0; i < field_height; i++)
    {
        draw(win, con_coord, (short)COLOR::Yellow, game_over_screen[i].c_str(), (int)game_over_screen[i].length());
        usleep(20*1000);    // 20ms
        con_coord.Y++;
    }

    usleep(1000*1000);       // 1000ms
}

void Tetris_Draw_Linux_Console::detectKeyboardInputLinux(Tetris* T)
{
    int ch;
    while(ch = getch())
    {
        //printw("key %d", ch );
        switch(ch)
        {
        case 115:        T->key_s();      break;
        case 113:        T->key_q();      break;
        case 103:        T->key_g();      break;
        case 32:         T->key_space();  break;
        case 27:         T->key_escape(); break;
        case KEY_UP:     T->key_up();     break;
        case KEY_DOWN:   T->key_down();   break;
        case KEY_LEFT:   T->key_left();   break;
        case KEY_RIGHT:  T->key_right();  break;
        default: /*ignore other keys */    break;
        }

        boost::this_thread::sleep_for(boost::chrono::milliseconds(thread_sleep_time_in_ms));
    }
}

#endif  // ((defined LINUX) || defined (__linux__))
