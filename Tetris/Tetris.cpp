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


void Tetris::print_stats()
{
    std::vector<Tetris_Stats_entry> highscores = stats->get_high_scores();
    std::cout << "\n\n\t H I G H S C O R E S \n\n";

    for (auto row : highscores)
    {
        std::cout << row.name << "..." << row.lines << "..." << row.level << "..." << row.score << std::endl;
    }
}

void Tetris::placeCurrentTetromino()
{
    std::pair<int, int>* location = static_cast<std::pair<int, int>*>(currentTetromino.getLocation());
    TetrominoKind kind = currentTetromino.getKind();

    for (int i = 0; i < maxMinos; i++)
    {
        // detect game over
        if (game_field[location[i].first][location[i].second].occupied)
            isActive = false;
        
        // place the next stone anyway
        game_field[location[i].first][location[i].second] = { kind, false };
    }

    if (!isActive)
    {   // on game over: draw the game field a very last time, save the score and print all highscores
        draw();
        stats->add_stats(entry);
        print_stats();
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

void Tetris::rotate(const RotateTetromino direction)
{
    Tetromino testTetromino = currentTetromino; // test a rotation on a copy, only if successfull, execute it


    bool validRotation = true;
    testTetromino.rotateTetromino(direction);
    const std::pair<int, int>* location = testTetromino.getLocation();
    for (int i = 0; i < maxMinos; i++)
    {
        /* does it hit the walls, which actually means, is it out of bounds? */
        if (location[i].first < 0 || location[i].first >= field_height ||
            location[i].second < 0 || location[i].second >= field_width)
        {
            validRotation = false;
            break;
        }

        /* does it rotate into another mino? */
        if(game_field[location[i].first][location[i].second].occupied)
        {
            validRotation = false;
            break;
        }
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

bool Tetris::tryShift(const MoveTetromino direction, Tetromino t)
{
    int x = 0, y = 0;
    switch (direction)
    {
    case MoveTetromino::Left:   x = -1; y =  0; break;
    case MoveTetromino::Right:  x =  1; y =  0; break;
    case MoveTetromino::Down:   x =  0; y = -1; break;
    }

    /* 1. locate the falling Tetromino */
    std::pair<int, int>* location = t.getLocation();

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

    return canMove;
}

bool Tetris::shift(const MoveTetromino direction)
{
    /* 1. check if move is possible */
    bool canMove = tryShift(direction, currentTetromino);

    std::pair<int, int>* location = currentTetromino.getLocation();

    /* 2. if it can move, move it */
    if (canMove)
    {
        for (auto i = maxMinos - 1; i >= 0; i--)
            game_field[location[i].first][location[i].second] = { TetrominoKind::none, false };

        currentTetromino.shiftTetromino(direction);
        placeCurrentTetromino();
    }

    /* 3. if this was a fall down move, then mark the fields as occupied if the Tetromino cannot fall anymore */
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
            entry.lines++;
            entry.score += 10;

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
        if (entry.lines % level_up == 0)
            entry.level++; // level-up after clearing another 25 lines
    }
}


void Tetris::detectKeyboardInput()
{
    std::cout << "Tetris::detectKeyboardInput() runs..." << std::endl;
    
    while (isActive)
    {
        if ((GetAsyncKeyState(VK_LEFT) & 0x01))
        {
            shift(MoveTetromino::Left);
        }

        if ((GetAsyncKeyState(VK_RIGHT) & 0x01))
        {
            shift(MoveTetromino::Right);
        }

        if ((GetAsyncKeyState(VK_DOWN) & 0x01))
        {
            rotate(RotateTetromino::CounterClockwise);
        }

        if ((GetAsyncKeyState(VK_UP) & 0x01))
        {
            rotate(RotateTetromino::Clockwise);
        }

        if ((GetAsyncKeyState(VK_SPACE) & 0x01))
        {
            while(fall());
            destroyLine();
            placeNextTetromino();
        }

        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01))
        {
            isActive = false;
        }
    }
}

void Tetris::run()
{
    std::cout << "Tetris::run() runs..." << std::endl;
    while (isActive)
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

void Tetris::clearBoard(void)
{
    Field init = { TetrominoKind::none, false };

    for(int i = 0; i < field_height; i++)
        for(int j = 0; j < field_width; j++)
            game_field[i][j] = init;
}

void Tetris::start()
{
    if (!isActive)
    {
        isActive = true;
        isPaused = false;
        entry.level = 1;
        entry.score = 0;
        entry.lines = 0;
        
        placeNextTetromino();
        boost::thread game_thread(boost::bind(&Tetris::run, this));
        boost::thread input_thread(boost::bind(&Tetris::detectKeyboardInput, this));

        game_thread.join();
        input_thread.join();

        isActive = false;
    }
}
