#include <cstdlib>
#include <thread>
#include <chrono>
#include "Tetris.h"
#include "Tetris_Draw_Linux_Console.h"
#include "Tetris_Draw_Windows_Console.h"

#if (defined (_WIN32) || defined (_WIN64))
#include <windows.h>
void detectKeyboardInputWindows(Tetris* T)
{
    while (T->quit() == false)
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

        std::this_thread::sleep_for(std::chrono::milliseconds(thread_sleep_time_in_ms));
    }
}
#endif


int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // seed prng
    
#if (defined (_WIN32) || defined (_WIN64))
    
    void (*keyboard_input)(Tetris* Tetris_object) = detectKeyboardInputWindows;
    Tetris_Draw_Windows_Console show;
    Tetris T = Tetris(show, keyboard_input);
    T.start();
    
#elif ((defined __unix__) || (defined __APPLE__))
    
    void (*keyboard_input)(Tetris* Tetris_object) = nullptr;
    Tetris_Draw_Linux_Console show;
    Tetris T = Tetris(show, keyboard_input);
    
    std::thread keyboard_thread(std::bind(&Tetris_Draw_Linux_Console::detectKeyboardInputLinux, &show, &T));
    
    T.start();
    keyboard_thread.join();
    
#endif

    return 0;
}
