#include <iostream>
#include "Tetris.h"
#include "Tetromino.h"


std::pair<int, int>* Tetromino::getLocation(void)
{
    return location;
}

void Tetromino::shiftTetromino(MoveTetromino direction)
{
    int x = 0, y = 0;
    switch (direction)
    {
        case MoveTetromino::Left:  x = -1; break;
        case MoveTetromino::Right: x =  1; break;
        case MoveTetromino::Down:  y = -1; break;
    default: std::cout << "ERROR: this is not a valid direction: " << static_cast<int>(direction) << std::endl; break;
    }

    location[0].first += y;  location[1].first += y;  location[2].first += y;  location[3].first += y;
    location[0].second += x; location[1].second += x; location[2].second += x; location[3].second += x;
}

void Tetromino::rotateTetromino(RotateTetromino direction)
{   /* rotation is implemented according to https://tetris.wiki/File:SRS-pieces.png */
    switch (kind)
    {
        case TetrominoKind::I: rotateI(direction); break;
        case TetrominoKind::J: rotateJ(direction); break;
        case TetrominoKind::L: rotateL(direction); break;
        case TetrominoKind::O: rotateO(direction); break;
        case TetrominoKind::S: rotateS(direction); break; 
        case TetrominoKind::T: rotateT(direction); break;
        case TetrominoKind::Z: rotateZ(direction); break;
    default: /* nothing to rotate here */ break;
    }
}

void Tetromino::rotateI(RotateTetromino direction) 
{   // TODO: this code could be reduced
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
            case TetrominoOrientation::Zero:
                location[0].first  += 1;  location[1].first  += 0;  location[2].first  += -1;  location[3].first  += -2;
                location[0].second += 2;  location[1].second += 1;  location[2].second +=  0;  location[3].second += -1;
                orientation = TetrominoOrientation::Ninety;
                break;

            case TetrominoOrientation::Ninety:
                location[0].first  += -2;  location[1].first  += -1;  location[2].first  +=  0;  location[3].first  +=  1;
                location[0].second +=  1;  location[1].second +=  0;  location[2].second += -1;  location[3].second += -2;
                orientation = TetrominoOrientation::OneEighty;
                break;                                                                         
                                                                                               
            case TetrominoOrientation::OneEighty:                                              
                location[0].first  += -1;  location[1].first  +=  0;  location[2].first  +=  1;  location[3].first  += 2;
                location[0].second += -2;  location[1].second += -1;  location[2].second +=  0;  location[3].second += 1;
                orientation = TetrominoOrientation::TwoSeventy;
                break;                                                                         
                                                                                               
            case TetrominoOrientation::TwoSeventy:                                             
                location[0].first  +=  2;  location[1].first  += 1;  location[2].first  += 0;  location[3].first  += -1;
                location[0].second += -1;  location[1].second += 0;  location[2].second += 1;  location[3].second +=  2;
                orientation = TetrominoOrientation::Zero;
                break;
        }
    }
    else // CounterClockwise
    {
        switch (orientation)
        {
            case TetrominoOrientation::Zero:
                location[0].first  += -2;  location[1].first  += -1;  location[2].first  +=  0;  location[3].first  +=  1;
                location[0].second +=  1;  location[1].second +=  0;  location[2].second += -1;  location[3].second += -2;
                orientation = TetrominoOrientation::TwoSeventy;
                break;

            case TetrominoOrientation::TwoSeventy:
                location[0].first  += 1;  location[1].first  += 0;  location[2].first  += -1;  location[3].first  += -2;
                location[0].second += 2;  location[1].second += 1;  location[2].second +=  0;  location[3].second += -1;
                orientation = TetrominoOrientation::OneEighty;
                break;

            case TetrominoOrientation::OneEighty:
                location[0].first  +=  2;  location[1].first  += 1;  location[2].first  += 0;  location[3].first  += -1;
                location[0].second += -1;  location[1].second += 0;  location[2].second += 1;  location[3].second +=  2;
                orientation = TetrominoOrientation::Ninety;
                break;

            case TetrominoOrientation::Ninety:
                location[0].first  += -1;  location[1].first  +=  0;  location[2].first  += 1;  location[3].first  += 2;
                location[0].second += -2;  location[1].second += -1;  location[2].second += 0;  location[3].second += 1;
                orientation = TetrominoOrientation::Zero;
                break;
        }
    }
}


void Tetromino::rotateJ(RotateTetromino direction) 
{
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }
    else // CounterClockwise
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }
}


void Tetromino::rotateL(RotateTetromino direction) 
{
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  += 1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second += 1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    } 
    else // CounterClockwise
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }

}


void Tetromino::rotateO(RotateTetromino direction) 
{
    /* rotation for the O-Tetromino is visually pointless according to the "Basic rotation system" */
}

void Tetromino::rotateS(RotateTetromino direction) 
{
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    } 
    else
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  += 0; location[1].first  +=  1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += 2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  += 2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += 0; location[1].second += -1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }

}

void Tetromino::rotateT(RotateTetromino direction) 
{
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -1; location[1].first  += 1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  1; location[1].second += 1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  += -1; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -1; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  1; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -1; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  1; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  1; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }
    else
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -1; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -1; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  += -1; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  1; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  1; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  1; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  1; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -1; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }
}

void Tetromino::rotateZ(RotateTetromino direction) 
{
    if (direction == RotateTetromino::Clockwise)
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += 0; location[1].first  += -1; location[2].first  += 0; location[3].first  += -1;
            location[0].second += 2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }
    else
    {
        switch (orientation)
        {
        case TetrominoOrientation::Zero:
            location[0].first  += -2; location[1].first  += -1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second +=  0; location[1].second += -1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::TwoSeventy;
            break;

        case TetrominoOrientation::TwoSeventy:
            location[0].first  +=  0; location[1].first  += -1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  2; location[1].second +=  1; location[2].second += 0; location[3].second += -1;
            orientation = TetrominoOrientation::OneEighty;
            break;

        case TetrominoOrientation::OneEighty:
            location[0].first  +=  2; location[1].first  +=  1; location[2].first  += 0; location[3].first  += -1;
            location[0].second +=  0; location[1].second +=  1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Ninety;
            break;

        case TetrominoOrientation::Ninety:
            location[0].first  +=  0; location[1].first  +=  1; location[2].first  += 0; location[3].first  +=  1;
            location[0].second += -2; location[1].second += -1; location[2].second += 0; location[3].second +=  1;
            orientation = TetrominoOrientation::Zero;
            break;
        }
    }

}



void Tetromino::setInitiallocation(TetrominoKind kind)
{
    /* create a new TetrominoKind, orientation according to https://tetris.wiki/Super_Rotation_System */
    switch (kind)
    {
        case TetrominoKind::I:
            /*
               ####
            */
            location[0] = { field_height - 1, 3 }; location[1] = { field_height - 1, 4 }; 
            location[2] = { field_height - 1, 5 }; location[3] = { field_height - 1, 6 };
            break;

        case TetrominoKind::J:
            /*
               #
               ###
            */
            location[0] = { field_height - 1, 3 }; location[1] = { field_height - 2, 3 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 5 };
            break;

        case TetrominoKind::L:
            /*
                 #
               ###
            */
            location[0] = { field_height - 1, 5 }; location[1] = { field_height - 2, 5 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 3 };
            break;

        case TetrominoKind::O:
            /*
               ##
               ##
            */
            location[0] = { field_height - 1, 4 }; location[1] = { field_height - 1, 5 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 5 };
            break;

        case TetrominoKind::S:
            /*
                ##
               ##
            */
            location[0] = { field_height - 1, 5 }; location[1] = { field_height - 1, 4 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 3 };
            break;

        case TetrominoKind::T:
            /*
               #
              ###
            */
            location[0] = { field_height - 1, 4 }; location[1] = { field_height - 2, 3 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 5 };
            break;

        case TetrominoKind::Z:
            /*
               ##
                ##
            */
            location[0] = { field_height - 1, 3 }; location[1] = { field_height - 1, 4 };
            location[2] = { field_height - 2, 4 }; location[3] = { field_height - 2, 5 };
            break;

    default:
        std::cout << "ERROR: getRandomTetromino() returned a bad mino: " << static_cast<int>(kind) << std::endl;
        exit(1);
        break;
    }
}