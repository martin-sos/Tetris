#ifndef _TETRIS_DRAW_
#define _TETRIS_DRAW_

#include "Tetris_Field.h"
#include "Tetris_Statistics.h"
#include <vector>

class Tetris_Draw {
public:
    virtual void draw_scene(std::vector<std::vector<Field>>) = 0;       // draw_scene current Tetris scene
    virtual void draw_layout() = 0;                                     // draw the skeleton: preview, game field and game stats
    virtual void update_stats(Tetris_Stats_entry) = 0;                  // update current game stats
    virtual void update_preview(TetrominoKind) = 0;                     // update the preview field with nexct Tetromino
    virtual void draw_highscores(std::vector<Tetris_Stats_entry>) = 0;   
};
#endif
