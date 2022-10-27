#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <vector>
#include "Tetromino.h"


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
	static TetrominoKind getRandomTetromino() { return static_cast<TetrominoKind>((std::rand() % 7) + 1); }	// randomly selects a mino

	std::vector<std::vector<Field>> game_field;		// the lower left corener of the game field is defined as the entry at row 0, column 0
	int score;								// point score of the current game
	bool isActive;							// is a game active?
	bool isPaused;
	int level;								// level := linecounter / 10, determines how fast minos are falling
	int lineCounter;						// counts how many lines has been destroyed
	Tetromino currentTetromino;
	Tetromino nextTetromino;				// the TetrominoKind which is added next to the game field

	
	void run();								// executes the Tetris game loop; is exectued in a thread
	void detectKeyboardInput();				// listens for keyboard input; is executed in a thread
	
	bool fall();							// lets minos fall down one more line, if there is nothing that could fall, return false, otherwise true
	bool shift(MoveTetromino);				// ...
	void rotate(RotateTetromino);			// rotate a TetrominoKind Left or Right
	
	void destroyLine();						// detects a full line, destroys it and will let everything above fall down one more line
	void placeNextTetromino();				// sets nextTetromino at the top of the game field and creates a new Tetromino
	void placeCurrentTetromino();
	void draw();							// draws the field
	


public:
	Tetris() {
		game_field = std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width));
		Field init = { TetrominoKind::none, false };
		for (auto row : game_field)
			std::fill(row.begin(), row.end(), init);

		std::srand(static_cast<unsigned>(std::time(nullptr)));
		score = 0;
		isActive = false;
		isPaused = false;
		level = 0;
		lineCounter = 0;
		currentTetromino = nextTetromino = Tetromino();
	}

	void start();				// starts a Tetris game, spawn all the needed threads, terminates as soon as game is over

};

#endif // _TETRIS_H_

