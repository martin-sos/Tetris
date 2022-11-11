#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <utility>
#include "Tetris_Definitions.h"

class Tetromino {
private:
    enum class TetrominoOrientation { Zero, Ninety, OneEighty, TwoSeventy };
    TetrominoKind kind;
    TetrominoOrientation orientation;
    std::pair<int, int> location[4];

    void setInitiallocation(TetrominoKind);

    void rotateI(RotateTetromino direction);
    void rotateJ(RotateTetromino direction);
    void rotateL(RotateTetromino direction);
    void rotateO(RotateTetromino direction);
    void rotateS(RotateTetromino direction);
    void rotateT(RotateTetromino direction);
    void rotateZ(RotateTetromino direction);

public:
    Tetromino() {
        kind = static_cast<TetrominoKind>((std::rand() % 7) + 1); 
        orientation = TetrominoOrientation::Zero;
        setInitiallocation(kind);
    }
    
    void shiftTetromino(MoveTetromino direction);
    void rotateTetromino(RotateTetromino direction);

    std::pair<int, int>* getLocation(void);
    TetrominoKind getKind() { return kind; }
};

#endif // _TETROMINO_H_