#ifndef TETRIS_DRAW_CONSOLE_FACTORY_H
#define TETRIS_DRAW_CONSOLE_FACTORY_H
#include <memory>
#ifdef __unix__
#include "Tetris_Draw_Linux_Console.h"
#elif (defined (_WIN32) || defined (_WIN64))
#include "Tetris_Draw_Windows_Console.h"
#endif



class Tetris_Draw_Console_Factory
{
public:
    static std::unique_ptr<Tetris_Draw> create()
    {
    #ifdef __unix__
        return std::unique_ptr<Tetris_Draw>(new Tetris_Draw_Linux_Console());
    #elif (defined (_WIN32) || defined (_WIN64))
        return std::unique_ptr<Tetris_Draw>(new Tetris_Draw_Windows_Console());
    #endif
    }
};

#endif // TETRIS_DRAW_CONSOLE_FACTORY_H
