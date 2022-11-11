#ifndef _TETRIS_DEFINITIONS_
#define _TETRIS_DEFINITIONS_

#include <string>

enum class TetrominoKind { none = 0, I, J, L, O, S, T, Z, Ghost, Pause };
enum class MoveTetromino { Left, Right, Down };
enum class RotateTetromino { Clockwise, CounterClockwise };

static constexpr const int field_width = 10;        // width of the Tetris game field
static constexpr const int field_height = 20;       // heigth of the Tetris game field

static constexpr const int level_up = 10;           // number of lines needed for a level up

static constexpr const int VK_G = 0x047;            // code for key G

static constexpr const int maxMinos = 4;            // number of minos a Tetromino is composed of

/* the Tetris game filed is composed of Field elements */
struct Field {
    TetrominoKind mino;     // describes the kind of the stone
    bool occupied;          // distinguishes if a field is already occupied or not 
};


/* each Tetris round is */
struct Tetris_Stats_entry
{
    std::string name;       // player name
    int lines;              // number of destroyed lines
    int level;              // level :=  lines / level_up
    int score;              // score := lines * 10
};

#endif