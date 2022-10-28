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

	std::vector<std::vector<Field>> game_field;		// the lower left corner of the game field is defined as the entry at row 0, column 0
    
	bool isActive;                          // is a game active?
    bool isPaused;                          // is a game paused?
    bool ghosting;                          // is ghosting actived?

	std::string player_name;				
    int score;                              // point score of the current game
    int level;                              // level := lines / 10; determines how fast minos are falling
    int lines;								// counts how many lines has been destroyed
	Tetris_Statistics* stats;
	
	Tetromino nextTetromino;				// the TetrominoKind which is added next to the game field
	Tetromino currentTetromino;

	
	void run();								// executes the Tetris game loop; is exectued in a thread
	void detectKeyboardInput();				// listens for keyboard input; is executed in a thread
	
	bool fall();									// lets minos fall down one more line, if there is nothing that could fall, return false, otherwise true
    bool shift(const MoveTetromino);				// ...
    bool tryShift(const MoveTetromino, Tetromino);  // ...
    void rotate(const RotateTetromino);				// rotate a TetrominoKind Left or Right
	
	void destroyLine();						// detects a full line, destroys it and will let everything above fall down one more line
	void placeNextTetromino();				// sets nextTetromino at the top of the game field and creates a new Tetromino
	void placeCurrentTetromino();
	void draw();							// draws the field
	void clearBoard(void);
	
public:
    Tetris()
          :game_field(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width))),
		  isActive(false), isPaused(false), ghosting(false), 
		  score(0), level(1), lines(0), stats(Tetris_Statistics::getInstance()),
          nextTetromino(Tetromino()), currentTetromino(nextTetromino)
          
    {
		std::cout << "\n\nPlease enter a name: ";
		std::cin >> player_name;
        clearBoard();
		std::srand(static_cast<unsigned>(std::time(nullptr)));
	}

	~Tetris()
	{
		delete stats;
	}

	void start();				// starts a Tetris game, spawn all the needed threads, terminates as soon as game is over

};

#endif // _TETRIS_H_

