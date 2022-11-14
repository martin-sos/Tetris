#include <iostream>
#include "Tetris.h"
#include "Tetris_Draw_Windows_Console.h"

std::string ASCII_LOGO = R"(

 _________     _______    _________     ________     ___      ________      
|\___   ___\  |\  ___ \   \___   ___\  |\   __  \   |\  \    |\   ____\     
\|___ \  \_|  \ \   __/|  |___ \  \_|  \ \  \|\  \  \ \  \   \ \  \___|_    
     \ \  \    \ \  \_|/__    \ \  \    \ \   _  _\  \ \  \   \ \_____  \   
      \ \  \    \ \  \_|\ \    \ \  \    \ \  \\  \|  \ \  \   \|____|\  \  
       \ \__\    \ \_______\    \ \__\    \ \__\\ _\   \ \__\    ____\_\  \ 
        \|__|     \|_______|     \|__|     \|__|\|__|   \|__|   |\_________\
                                                                \|_________|
)";

int main()
{
    std::cout << ASCII_LOGO;
#if (defined (_WIN32) || defined (_WIN64))
    Tetris_Draw_Windows_Console show = Tetris_Draw_Windows_Console();
#elif (defined LINUX) || defined (__linux__))
    Tetris_Draw_Linux_Console show = Tetris_Draw_Linux_Console();
#endif

    Tetris T = Tetris(&show);
    T.start();
    
    return 0;
}