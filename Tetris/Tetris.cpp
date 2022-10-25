// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <windows.h>
#include "Tetris.h"


const std::string endLine = " ----------";

inline char minoToChar(TetrominoKind mino)
{
    switch (mino)
    {
    case TetrominoKind::I:      return '#';
    case TetrominoKind::J:      return '#';
    case TetrominoKind::L:      return '#';
    case TetrominoKind::O:      return '#';
    case TetrominoKind::S:      return '#';
    case TetrominoKind::T:      return '#';
    case TetrominoKind::Z:      return '#';
    case TetrominoKind::none:   return ' ';
    default:                    return '?';
    }
}

void Tetris::placeNextTetromino()
{
    // TODO: shall we spwan in row 21 and 22, but keep the game_field of matter 10 by 20??
    currentTetromino = nextTetromino;
    placeCurrentTetromino();
    nextTetromino = Tetromino();
}

void Tetris::placeCurrentTetromino()
{
    std::pair<int, int>* location = static_cast<std::pair<int, int>*>(currentTetromino.getLocation());
    TetrominoKind kind = currentTetromino.getKind();

    for (int i = 0; i < maxMinos; i++)
    {
        // detect game over
        if (game_field[location[i].first][location[i].second].occupied)
            active = false;
        
        // place the next stone anyway
        game_field[location[i].first][location[i].second] = { kind, false };
    }

    if (!active)
    { // on game over: draw the game field a very last time
        draw();
    }
}

void Tetris::draw()
{
    system("cls");
    for (int i = field_height-1; i >= 0; i--)
    {
        std::cout << "+";
        for (auto field : game_field[i])
            std::cout << minoToChar(field.mino);
        std::cout << "+" << std::endl;
    }
    
    std::cout << endLine;
}

void Tetris::rotate(RotateTetromino direction)
{
    Tetromino testTetromino = currentTetromino; // test a rotation on a copy, only if successfull, execute it

    /* does it hit the walls, which actually means, is it out of bounds? */
    bool validRotation = true;
    testTetromino.rotateTetromino(direction);
    std::pair<int, int>* location = testTetromino.getLocation();
    for (int i = 0; i < maxMinos; i++)
    {
        if (location[i].first < 0 || location[i].first >= field_height ||
            location[i].second < 0 || location[i].second >= field_width)
            validRotation = false;
    }

    if (validRotation)
    {
        location = currentTetromino.getLocation();
       
        /* remove from game field */
        for (auto i = maxMinos - 1; i >= 0; i--)
            game_field[location[i].first][location[i].second] = { TetrominoKind::none, false };

        currentTetromino = testTetromino;
        placeCurrentTetromino();
    }
}

bool Tetris::fall()
{
    return shift(MoveTetromino::Down);    
}

bool Tetris::shift(MoveTetromino direction)
{
    int x = 0, y = 0;
    switch (direction)
    {
    case MoveTetromino::Left:   x = -1; y =  0; break;
    case MoveTetromino::Right:  x =  1; y =  0; break;
    case MoveTetromino::Down:   x =  0; y = -1; break;
    }

    /* 1. locate the falling Tetromino */
    std::pair<int, int>* location = currentTetromino.getLocation();

    /* 2. check if it can move by (x,y) */
    bool canMove = true;
    for (auto i = maxMinos - 1; i >= 0; i--)
    { // it is important that we go bottom to top here 

        int row = location[i].first;
        int col = location[i].second;


        if (row + y < 0 || row + y >= field_height ||
            col + x < 0 || col + x >= field_width ||
            game_field[row + y][col + x].occupied == true)
        {
            canMove = false;
            break;
        }
    }

    /* 3. if it can move, move it */
    if (canMove)
    {
        for (auto i = maxMinos - 1; i >= 0; i--)
            game_field[location[i].first][location[i].second] = { TetrominoKind::none, false };
        
        currentTetromino.shiftTetromino(direction);
        placeCurrentTetromino();
    }

    /* if this was a fall down move, then mark the fields as occupied if the Tetromino cannot fall anymore */
    if (canMove == FALSE && direction == MoveTetromino::Down)
    {
        for (auto i = maxMinos - 1; i >= 0; i--)
        {
            int row = location[i].first;
            int col = location[i].second;

            game_field[row][col].occupied = true;
        }
    }
   
    return canMove;
}

void Tetris::destroyLine()
{
    /* from bottom to top */
    bool destroy = false;
    for (int i = 0; i < field_height; i++)
    {
        // detect full row
        bool row_occupied = true;
        for (int j = 0; j < field_width && row_occupied; j++)
            row_occupied &= game_field[i][j].occupied;


        if (row_occupied)
        {
            destroy = true;
            lineCounter++;
            score += 10;

            // destroy full row
            for (int j = 0; j < field_width; j++)
                game_field[i][j] = { TetrominoKind::none, false };

            // move down each row above by one
            for (int k = i + 1; k < field_height; k++)
            {
                for (int j = 0; j < field_width; j++)
                {
                    if (game_field[k][j].occupied)
                    {
                        game_field[k - 1][j] = game_field[k][j];
                        game_field[k][j] = { TetrominoKind::none, false };
                    }
                }
            }
            i--; // row i is now a new row and needs to be checked again
        }

    }

    if (destroy)
    {
        if (lineCounter % level_up == 0)
            level++; // level-up after clearing another 25 lines
    }
}


void Tetris::detectKeyboardInput()
{
    std::cout << "Tetris::detectKeyboardInput() runs..." << std::endl;
    
    while (active)
    {
        if ((GetAsyncKeyState(VK_LEFT) & 0x01))
        {
            //std::cout << "VK_LEFT" << std::endl;
            shift(MoveTetromino::Left);
            
        }

        if ((GetAsyncKeyState(VK_RIGHT) & 0x01))
        {
            //std::cout << "VK_RIGTH" << std::endl;
            shift(MoveTetromino::Right);
            
        }

        if ((GetAsyncKeyState(VK_DOWN) & 0x01))
        {
            rotate(RotateTetromino::CounterClockwise);
            //std::cout << "VK_DOWN" << std::endl;
        }

        if ((GetAsyncKeyState(VK_UP) & 0x01))
        {
            //std::cout << "VK_UP" << std::endl;
            rotate(RotateTetromino::Clockwise);
        }

        if ((GetAsyncKeyState(VK_SPACE) & 0x01))
        {
            //std::cout << "VK_SPACE" << std::endl;
            while(fall());
            destroyLine();
            placeNextTetromino();
        }
    }
}

void Tetris::run()
{
    std::cout << "Tetris::run() runs..." << std::endl;
    while (active)
    { // each eexecution of this loop let's a mino fall another row

        draw();
        
        if (fall() == false)
        {
            destroyLine();
            placeNextTetromino();
        }
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(400));
        
    }
}

void Tetris::start()
{
    if (!active)
    {
        active = true;
        paused = false;
        level = 1;
        score = 0;
        lineCounter = 0;
        
        placeNextTetromino();
        boost::thread game_thread(boost::bind(&Tetris::run, this));
        boost::thread input_thread(boost::bind(&Tetris::detectKeyboardInput, this));

        game_thread.join();
        input_thread.join();

        active = false;
    }
}



int main()
{
    std::cout << "Hello World!\n";

    Tetris T = Tetris();
    T.start();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
