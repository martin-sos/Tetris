#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <vector>
#include "Tetromino.h"
#include "Tetris_Statistics.h"


/*enum class to differentiate the different minos of Tetris */
static constexpr const int field_width = 10;
static constexpr const int field_height = 20;
static constexpr const int level_up = 10;

struct Field {
    TetrominoKind mino;
    bool occupied;
};

class Tetris {
private:        

    std::vector<std::vector<Field>> game_field;     // the lower left corner of the game field is defined as the entry at row 0, column 0
    
    bool isActive;                          // is a game active?
    bool isPaused;                          // is a game paused?
    bool ghosting;                          // is ghosting actived?

    Tetris_Stats_entry entry;
    Tetris_Statistics* stats;
    
    Tetromino nextTetromino;                // the TetrominoKind which is added next to the game field
    Tetromino currentTetromino;

    
    void run();                             // executes the Tetris game loop; is exectued in a thread
    void detectKeyboardInput();             // listens for keyboard input; is executed in a thread
    
    bool fall();                                    // lets minos fall down one more line, if there is nothing that could fall, return false, otherwise true
    bool shift(const MoveTetromino);                // ...
    bool tryShift(const MoveTetromino, Tetromino);  // ...
    void rotate(const RotateTetromino);             // rotate a TetrominoKind Left or Right
    
    void destroyLine(void);                 // detects a full line, destroys it and will let everything above fall down one more line
    void placeNextTetromino(void);          // sets nextTetromino at the top of the game field and creates a new Tetromino
    void placeCurrentTetromino(void);
    void draw(void);                        // draws the field
    void print_stats(void);
    void clearBoard(void);
    
public:
    Tetris()
          :game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width))),
          isActive(false), isPaused(false), ghosting(false), 
          entry({"", 0, 1, 0}),
          stats(Tetris_Statistics::getInstance()),
          nextTetromino(Tetromino()), currentTetromino(nextTetromino)
          
    {

        std::cout << "\n\nPlease enter a name: ";
        std::cin >> entry.name;
        clearBoard();
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~Tetris()
    {
        delete stats;
    }

    void start();               // starts a Tetris game, spawn all the needed threads, terminates as soon as game is over

};

#endif // _TETRIS_H_

