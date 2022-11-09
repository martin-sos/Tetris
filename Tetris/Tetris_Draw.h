#ifndef _TETRIS_DRAW_
#define _TETRIS_DRAW_

#include "Tetris_Field.h"
#include <vector>

class Tetris_Draw {
public:
    virtual void draw(std::vector<std::vector<Field>> game_field) = 0;
    virtual void draw_frame() = 0;
};
#endif
