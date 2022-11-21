#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

// TODO: once another plartform is added, exclude platform specific parts into separate files and control interface usage via header files
#if (defined (_WIN32) || defined (_WIN64))
#include <windows.h>
#endif

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


#if (defined (_WIN32) || defined (_WIN64))
void detectKeyboardInputWindows(Tetris* T)
{
    while (1)
    {
        if ((GetAsyncKeyState(VK_LEFT) & 0x01))
            T->key_left();

        if ((GetAsyncKeyState(VK_RIGHT) & 0x01))
            T->key_right();

        if ((GetAsyncKeyState(VK_DOWN) & 0x01))
            T->key_down();

        if ((GetAsyncKeyState(VK_UP) & 0x01))
            T->key_up();

        if ((GetAsyncKeyState(VK_SPACE) & 0x01))
            T->key_space();

        if ((GetAsyncKeyState(VK_G) & 0x01))
            T->key_g();

        if ((GetAsyncKeyState(VK_Q) & 0x01))
            T->key_q();

        if ((GetAsyncKeyState(VK_C) & 0x01))
            T->key_s();

        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01))
        {

            T->key_escape();
            /* flush keyboard input in the meantime, otherwise steering Tetrominos although pause is possible */
            GetAsyncKeyState(VK_SPACE);
            GetAsyncKeyState(VK_UP);
            GetAsyncKeyState(VK_DOWN);
            GetAsyncKeyState(VK_RIGHT);
            GetAsyncKeyState(VK_LEFT);
        }

        boost::this_thread::sleep_for(boost::chrono::milliseconds(thread_sleep_time_in_ms));
    }
}
#endif

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // seed prng
    std::cout << ASCII_LOGO;
#if (defined (_WIN32) || defined (_WIN64))
    Tetris_Draw_Windows_Console show = Tetris_Draw_Windows_Console();
    void (*keyboard_input)(Tetris* Tetris_object) = detectKeyboardInputWindows;
#elif (defined LINUX) || defined (__linux__))
    Tetris_Draw_Linux_Console show = Tetris_Draw_Linux_Console();
    void (*func)(Tetris* Tetris_object) = nullptr;
#endif

    Tetris T = Tetris(show, keyboard_input);
    T.start();
   
    
    return 0;
}