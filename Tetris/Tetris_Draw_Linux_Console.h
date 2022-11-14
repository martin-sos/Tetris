#ifndef _TETRIS_DRAW_LINUX_CONSOLE_H_
#define _TETRIS_DRAW_LINUX_CONSOLE_H_

#if ((defined LINUX) || defined (__linux__))

#include <curses.h>
#include <signal.h>
#include "Tetris_Draw.h"

class Tetris_Draw_Linux_Console : public Tetris_Draw
{

};

#endif // (defined LINUX) || defined (__linux__))
#endif // _TETRIS_DRAW_LINUX_CONSOLE_H_
