#ifndef _TETRIS_FIELD_
#define _TETRIS_FIELD_

#include "Tetromino.h"

static constexpr const int field_width = 10;
static constexpr const int field_height = 20;

struct Field {
    TetrominoKind mino;
    bool occupied;
};

#endif