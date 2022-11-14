// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <windows.h>
#include "Tetris.h"


void Tetris::placeNextTetromino()
{
    // TODO: shall we spwan in row 21 and 22, but keep the game_field of matter 10 by 20??
    ghost = currentTetromino = nextTetromino;
    placeCurrentTetromino();
    nextTetromino = Tetromino::getTetromino();
    show->update_preview(nextTetromino.getKind());
}

void Tetris:: placeCurrentTetromino()
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
        show->draw_scene(game_field);
        stats->add_stats(entry);
        show->draw_highscores(stats->get_high_scores());
    }
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

bool Tetris::tryShift(const MoveTetromino direction, Tetromino t) const
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
        updateGhost();
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

void Tetris::updateGhost()
{
    if (ghosting)
    {  

        /* 1. reset previous ghost */
        eraseGhost();

        /* 2. compute ghost projection */
        ghost = currentTetromino;
        bool canMove = tryShift(MoveTetromino::Down, ghost);
        while (canMove)
        {
            ghost.shiftTetromino(MoveTetromino::Down);
            canMove = tryShift(MoveTetromino::Down, ghost);
        }

        /* 3. place the ghost */
        const std::pair<int, int>* location = ghost.getLocation();
        for (int i = 0; i < maxMinos; i++)
        {
            // but do not overwrite the current Tetromino when it just landed
            if (game_field[location[i].first][location[i].second].mino == TetrominoKind::none)
                game_field[location[i].first][location[i].second] = { TetrominoKind::Ghost, false };
        }
    }
}

void Tetris::eraseGhost()
{
    const std::pair<int, int>* location = ghost.getLocation();

    /* reset previous ghost */
    for (int i = 0; i < maxMinos; i++)
    {
        if (game_field[location[i].first][location[i].second].mino == TetrominoKind::Ghost)
            game_field[location[i].first][location[i].second] = { TetrominoKind::none, false };
    }
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
        {
            entry.level++; // level-up after clearing another 10 lines
            game_loop_sleep_time_ms = game_loop_sleep_time_ms - (entry.level - 1) * 20; // reduce sleep by 20ms, and hence speed up the game
        }
    }
}


void Tetris::detectKeyboardInput()
{    
    while (isActive)
    {
        if (!isPaused)
        { // controls are enabled only if game is not paused
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
                letFall = true;
            }

            if ((GetAsyncKeyState(VK_G) & 0x01))
            {
                ghosting = !ghosting;
                eraseGhost();
            }
        }
        
        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01))
        {
            isPaused = !isPaused;
            
            if (isPaused)
            {   /* overwrite preview and game field */
                show->update_preview(TetrominoKind::Pause);
                show->draw_scene(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width, { TetrominoKind::Pause, false })));
            }
            else
            {   /* redraw scene as it was before the pause */
                show->update_preview(nextTetromino.getKind());
                show->draw_scene(game_field);
            }

            /* flush keyboard input in the meantime, otherwise steering Tetrominos although pause is possible */
            GetAsyncKeyState(VK_SPACE);
            GetAsyncKeyState(VK_UP);
            GetAsyncKeyState(VK_DOWN);
            GetAsyncKeyState(VK_RIGHT);
            GetAsyncKeyState(VK_LEFT);
        }

        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    }
}

void Tetris::run()
{
    int tetris_sleep_period_ms = game_loop_sleep_time_ms;
    int thread_sleep_period_ms = 10;
    
    /* main game loop */
    while (isActive)
    {
        if (!isPaused)
        {
            if (tetris_sleep_period_ms <= 0 || letFall)
            {
                bool could_fall = false;
                do {
                    could_fall = fall();        // every period tetris_sleep_period_ms let the stone fall
                    letFall &= could_fall;
                } while (letFall);              // if space has been pressed, let the current Tetromino fall to the very bottom

                letFall = false;
                
                if(could_fall == false)         // if the very bottom has been reached
                {
                    destroyLine();
                    show->update_stats(entry);
                    placeNextTetromino();
                }
                tetris_sleep_period_ms = game_loop_sleep_time_ms;   // wind up the sleep time again
            }
            show->draw_scene(game_field);       
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(thread_sleep_period_ms));
        tetris_sleep_period_ms -= thread_sleep_period_ms;
    }
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
