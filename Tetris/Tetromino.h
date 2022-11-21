#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <utility>
#include "Tetris_Definitions.h"
#include <queue>

class Tetromino {
private:
    enum class TetrominoOrientation { Zero, Ninety, OneEighty, TwoSeventy };
    TetrominoKind kind;
    TetrominoOrientation orientation;
    std::vector<std::pair<int, int>> location;
    static std::queue<Tetromino> bag_of_seven;

    Tetromino(TetrominoKind k) {
        kind = k;
        orientation = TetrominoOrientation::Zero;
        location = std::vector <std::pair<int, int>>(maxMinos, std::pair<int, int>(0, 0));
        setInitiallocation(kind);
    }

    void setInitiallocation(TetrominoKind);

    void rotateI(RotateTetromino direction);
    void rotateJ(RotateTetromino direction);
    void rotateL(RotateTetromino direction);
    void rotateO(RotateTetromino direction);
    void rotateS(RotateTetromino direction);
    void rotateT(RotateTetromino direction);
    void rotateZ(RotateTetromino direction);

public:
    static Tetromino getTetromino();
    
    void shiftTetromino(MoveTetromino direction);
    void rotateTetromino(RotateTetromino direction);

    std::vector<std::pair<int, int>> getLocation(void);
    TetrominoKind getKind() { return kind; }
};

#endif // _TETROMINO_H_