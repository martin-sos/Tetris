#ifndef _TETRIS_DRAW_
#define _TETRIS_DRAW_

#include "Tetris_Field.h"
#include "Tetris_Statistics.h"
#include <vector>

class Tetris_Draw {
public:
    virtual void draw(std::vector<std::vector<Field>>) = 0;
    virtual void draw_frame() = 0;
    virtual void update_stats(Tetris_Stats_entry) = 0;
    virtual void update_preview(TetrominoKind) = 0;
};
#endif
