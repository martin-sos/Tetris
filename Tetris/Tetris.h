#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <vector>
#include "Tetris_Draw.h"
#include "Tetromino.h"
#include "Tetris_Definitions.h"
#include "Tetris_Statistics.h"

class Tetris {
public:
    Tetris(Tetris_Draw* s)
        :game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width, { TetrominoKind::none, false }))),
        isActive(false), isPaused(false), ghosting(false),
        game_loop_sleep_time_ms(400),
        show(s),
        entry({ "Player 1", 0, 1, 0 }),
        stats(Tetris_Statistics::get_instance()),
        nextTetromino(Tetromino::getTetromino()), currentTetromino(nextTetromino), ghost(currentTetromino)
    {
        show->update_preview(nextTetromino.getKind());
        show->draw_highscores(stats->get_high_scores());

        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~Tetris()
    {
        delete stats;
    }

    void start();               // starts a Tetris game, spawns all needed threads, terminates as soon as game is over


private:        

    std::vector<std::vector<Field>> game_field;     // the lower left corner of the game field is defined as the entry at row 0, column 0
    
    bool isActive;                          // is a game active?
    bool isPaused;                          // is a game paused?
    bool ghosting;                          // is ghosting actived?
    int game_loop_sleep_time_ms;            // sleep time in ms for the main game loop, determines how quickly Tetrominos are falling

    Tetris_Draw *show;

    Tetris_Stats_entry entry;
    Tetris_Statistics *stats;
    
    Tetromino nextTetromino;                // the TetrominoKind which is added next to the game field
    Tetromino currentTetromino;             // the Tetromnino which is currently falling
    Tetromino ghost;                        // a projection of the current Tetromino at the location where the current Tetromino would land 

    void run();                             // executes the Tetris game loop; is exectued in a thread
    void detectKeyboardInput();             // listens for keyboard input; is executed in a thread
    
    bool fall();                                    // lets minos fall down one more line, if there is nothing that could fall, return false, otherwise true
    bool shift(const MoveTetromino);                // ...
    void updateGhost();
    void eraseGhost();
    bool tryShift(const MoveTetromino, Tetromino) const;  // ...
    void rotate(const RotateTetromino);             // rotate a TetrominoKind Left or Right
    
    void destroyLine(void);                 // detects a full line, destroys it and will let everything above fall down one more line
    void placeNextTetromino(void);          // sets nextTetromino at the top of the game field and creates a new Tetromino
    void placeCurrentTetromino(void);
};

#endif // _TETRIS_H_

