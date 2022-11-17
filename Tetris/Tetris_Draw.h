#ifndef _TETRIS_DRAW_
#define _TETRIS_DRAW_

#include "Tetris_Definitions.h"
#include <vector>

class Tetris_Draw {
public:
    virtual void draw_scene(std::vector<std::vector<Field>>) = 0;       // draws current Tetris scene
    virtual void draw_layout() = 0;                                     // draw the skeleton: preview, game field, controls and game stats
    virtual void update_stats(Tetris_Stats_entry) = 0;                  // update current game stats
    virtual void update_preview(TetrominoKind) = 0;                     // update the preview field with nexct Tetromino
    virtual void draw_highscores(std::vector<Tetris_Stats_entry>) = 0;   
    virtual void draw_game_over() = 0;

};
#endif
