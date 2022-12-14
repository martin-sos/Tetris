#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include <chrono>
#include "Tetris.h"

std::mutex m;

void Tetris::placeNextTetromino()
{
    // TODO: shall we spwan in row 21 and 22, but keep the game_field of matter 10 by 20??
    ghost = currentTetromino = nextTetromino;
    placeCurrentTetromino();
    nextTetromino = bag_of_tetrominos.grab_from_bag();
    show.update_preview(nextTetromino.getKind());
}

void Tetris::placeCurrentTetromino()
{
    std::vector<std::pair<int, int>> location = currentTetromino.getLocation();
    TetrominoKind kind = currentTetromino.getKind();

    for (int i = 0; i < maxMinos; i++)
    {
        // detect game over
        if (game_field[location[i].first][location[i].second].occupied)
            game_state = Tetris_State::game_over;

        // place the next stone anyway
        game_field[location[i].first][location[i].second] = { kind, false };
    }

    // as the position of the current tetromino is now determined, the position of its ghost can be determined as well
    updateGhost();

    if (game_state == Tetris_State::game_over)
    {   // on game over: draw the game field a very last time, save the score and print all highscores
        show.draw_scene(game_field);
        stats.add_stats(entry);
        show.draw_highscores(stats.get_high_scores());
    }

}

void Tetris::rotate(const RotateTetromino direction)
{
    std::lock_guard<std::mutex> lockGuard(m);
    Tetromino testTetromino = currentTetromino; // test a rotation on a copy, only if successfull, execute it

    bool validRotation = true;
    testTetromino.rotateTetromino(direction);
    std::vector<std::pair<int, int>> location = testTetromino.getLocation();
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
    std::vector<std::pair<int, int>> location = t.getLocation();

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
    std::lock_guard<std::mutex> lockGuard(m);

    /* 1. check if move is possible */
    bool canMove = tryShift(direction, currentTetromino);
    std::vector<std::pair<int, int>> location = currentTetromino.getLocation();

    /* 2. if it can move, move it */
    if (canMove)
    {
        for (auto i = maxMinos - 1; i >= 0; i--)
            game_field[location[i].first][location[i].second] = { TetrominoKind::none, false };

        currentTetromino.shiftTetromino(direction);
        placeCurrentTetromino();
    }

    /* 3. if this was a fall down move, then mark the fields as occupied if the Tetromino cannot fall anymore */
    if (canMove == false && direction == MoveTetromino::Down)
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
        std::vector<std::pair<int, int>> location = ghost.getLocation();
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
    std::vector<std::pair<int, int>> location = ghost.getLocation();

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
            entry.score += points_per_line;

            // destroy full row
            for (int j = 0; j < field_width; j++)
                game_field[i][j] = { TetrominoKind::none, false };

            // move down each row above by one
            for (int k = i + 1; k < field_height; k++)
            {
                game_field[k - 1] = game_field[k];
                game_field[k] = std::vector<Field>(field_width, { TetrominoKind::none, false });
            }
            i--; // row i is now a new row and needs to be checked again
        }
    }

    if (destroy)
    {
        if (entry.lines / level_up == entry.level)
        {   // after clearing another 10 lines...
            entry.level++; // ...level-up
            game_loop_sleep_time_ms -= gravity_reduction ; // ..reduce sleep time by gravity constant, and hence speed up the game
        }
    }
}

void Tetris::key_escape()
{

    if (game_state == Tetris_State::playing)
    {
        game_state = Tetris_State::pause;
        /* overwrite preview and game field */
        show.update_preview(TetrominoKind::Pause);
        show.draw_scene(std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width, { TetrominoKind::Pause, false })));
    }
    else if (game_state == Tetris_State::pause)
    {
        game_state = Tetris_State::playing;
        /* redraw scene as it was before the pause */
        show.update_preview(nextTetromino.getKind());
        show.draw_scene(game_field);
    }
    else
    {
        /* nothing to do */
    }
}

void Tetris::run()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = (end_time - start_time) / std::chrono::milliseconds(1);

    /* main game loop */
    while (game_state == Tetris_State::playing || game_state == Tetris_State::pause)
    {
        if (game_state == Tetris_State::playing)
        {
            end_time = std::chrono::high_resolution_clock::now();
            duration_ms = (end_time - start_time) / std::chrono::milliseconds(1);
            if (duration_ms >= game_loop_sleep_time_ms || letFall)
            {
                start_time = end_time;
                bool could_fall = false;

                do {
                    could_fall = fall();        // every tetris_sleep_period_ms let the stone fall
                    letFall &= could_fall;
                } while (letFall);              // if space has been pressed, let the current Tetromino fall to the very bottom

                letFall = false;

                if(could_fall == false)         // if the very bottom has been reached
                {
                    std::lock_guard<std::mutex> lockGuard(m);
                    destroyLine();
                    show.update_stats(entry);
                    placeNextTetromino();
                }
            }
            show.draw_scene(game_field);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(thread_sleep_time_in_ms));
              
    }
}

void Tetris::start()
{
    if (game_state == Tetris_State::idle)
    {
        std::thread keyboard_thread;
        if (detectKeyboardInput != nullptr)
            keyboard_thread = std::thread(detectKeyboardInput, this);

        game_state = Tetris_State::playing;

        do
        {
            show.draw_layout();
            show.update_preview(nextTetromino.getKind());
            show.update_stats(entry);
            show.draw_highscores(stats.get_high_scores());

            placeNextTetromino();
            std::thread game_thread(std::bind(&Tetris::run, this));
            game_thread.join();

            /* ***  from here : game is over *** */

            // reset game state
            game_field = std::vector<std::vector<Field>>(field_height, std::vector<Field>(field_width, { TetrominoKind::none, false }));
            game_loop_sleep_time_ms = initial_gravity;
            entry.level = 1; entry.lines = 0; entry.score = 0;

            bag_of_tetrominos.clear();
            nextTetromino = bag_of_tetrominos.grab_from_bag();
            ghost = currentTetromino = nextTetromino;

            show.draw_game_over();

            // busy wait for user quitting or starting another game
            while (game_state == Tetris_State::game_over)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } while (game_state == Tetris_State::playing);

        if (detectKeyboardInput != nullptr)
        {
            keyboard_thread.join();
        }
    }

}
