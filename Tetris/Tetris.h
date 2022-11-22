#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <vector>
#include "Tetris_Draw.h"
#include "Tetromino.h"
#include "Tetris_Definitions.h"
#include "Tetris_Statistics.h"

class Tetris {
public:
    Tetris(Tetris_Draw& s, void (*f)(Tetris*) =nullptr)
        :game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width, { TetrominoKind::none, false }))),
        game_state(Tetris_State::idle),
        ghosting(false), letFall(false),
        game_loop_sleep_time_ms(initial_gravity),
        show(s),
        detectKeyboardInput(f),
        entry({ "Player 1", 0, 1, 0 }),
        stats(Tetris_Statistics::get_instance()),
        nextTetromino(Tetromino::getTetromino()), currentTetromino(nextTetromino), ghost(currentTetromino)
    {
    }

    void start();   // starts a Tetris game, spawns all needed threads, terminates as soon as game is over

    void key_up()   { if (game_state == Tetris_State::playing) rotate(RotateTetromino::Clockwise); }
    void key_down() { if (game_state == Tetris_State::playing) rotate(RotateTetromino::CounterClockwise); }
    void key_left() { if (game_state == Tetris_State::playing) shift(MoveTetromino::Left); }
    void key_right(){ if (game_state == Tetris_State::playing) shift(MoveTetromino::Right); }
    void key_space(){ if (game_state == Tetris_State::playing) letFall = true; }
    void key_g()    { if (game_state == Tetris_State::playing) (ghosting = !ghosting) ? updateGhost() : eraseGhost(); }
    
    void key_q()    
    { 
        if (game_state == Tetris_State::playing || game_state == Tetris_State::game_over)
            game_state = Tetris_State::quit;
    }    

    void key_s()    
    { 
        if (game_state == Tetris_State::game_over)
            game_state = Tetris_State::playing;
    }   
    
    void key_escape(); 
    


private:        

    /* 
         *** Tetris state machine *** 
       
       STATE   |     EVENT     |   STATE
    _______________________________________
               |    Tetris()   |   idle
    ---------------------------------------
        idle   |    start()    |  playing
    ---------------------------------------
      playing  |    key_q()    |   quit
    ---------------------------------------
      playing  |  key_escape() |   pause 
    ---------------------------------------
       pause   |  key_escape() |   pause 
    ---------------------------------------
               |  placing new  | 
      playing  | Tetromino on  | game_over
               | occupied field|  
    ---------------------------------------
     game_over |    key_s()    |  playing
    ---------------------------------------
     game_over |    key_q()    |   quit
    ---------------------------------------
      playing  |    key_up()   |  playing
    ---------------------------------------
      playing  |   key_down()  |  playing
    ---------------------------------------
      playing  |   key_left()  |  playing
    ---------------------------------------
      playing  |   key_right() |  playing
    ---------------------------------------
      playing  |   key_space() |  playing
    ---------------------------------------
      playing  |    key_g()    |  playing
    ---------------------------------------



    */

    enum class Tetris_State { idle, playing, pause, game_over, quit };

    std::vector<std::vector<Field>> game_field;     // the lower left corner of the game field is defined as the entry at row 0, column 0
    Tetris_State game_state;
    
    bool ghosting;                                  // is ghosting actived?
    bool letFall;                                   // if true, then let the current Tetromino fall to the very bottom

    int game_loop_sleep_time_ms;                    // sleep time in ms for the main game loop, determines how quickly Tetrominos are falling

    Tetris_Draw& show;
    void (* const detectKeyboardInput)(Tetris*);    // pointer to user defined function which detects keyboard input

    Tetris_Stats_entry entry;
    Tetris_Statistics& stats;
    
    Tetromino nextTetromino;                        // the TetrominoKind which is added next to the game field
    Tetromino currentTetromino;                     // the Tetromnino which is currently falling
    Tetromino ghost;                                // a projection of the current Tetromino at the location where the current Tetromino would land 

    void run();                                             // executes the Tetris game loop; is exectued in a thread
    bool fall();                                            // lets minos fall down one more line, if there is nothing that could fall, return false, otherwise true
    bool shift(const MoveTetromino);                        // shifts / moves the Tetromino in the asked direction, return true if successful, otherwise false
    bool tryShift(const MoveTetromino, Tetromino) const;    // tests if a shift of a given Tetromino and a given direction is possible
    void rotate(const RotateTetromino);                     // rotate a TetrominoKind Left or Right
    void updateGhost();                                     // set the ghost stone into the game field
    void eraseGhost();                                      // removes the ghost stone from the game field
    void destroyLine(void);                                 // detects a full lines, destroy them and will let line above fall down accordingly
    void placeNextTetromino(void);                          // sets nextTetromino at the top of the game field and loads the next Tetromino
    void placeCurrentTetromino(void);                       // after a rotation or shift, updates the game field according to the position of the currentTetromino
};

#endif // _TETRIS_H_

