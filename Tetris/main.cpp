#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include "Tetris.h"
#include "Tetris_Draw_Console_Factory.h"


#if (defined (_WIN32) || defined (_WIN64))
#include <windows.h>
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
#if (defined (_WIN32) || defined (_WIN64))
    void (*keyboard_input)(Tetris* Tetris_object) = detectKeyboardInputWindows;
#elif ((defined __unix__) || (defined __APPLE__))
    void (*keyboard_input)(Tetris* Tetris_object) = nullptr;
#endif

    //std::unique_ptr<Tetris_Draw> show = Tetris_Draw_Console_Factory::create();
    
    Tetris_Draw_Linux_Console s;
    Tetris T = Tetris(s, keyboard_input);
    //Tetris T = Tetris(*show, keyboard_input);

#if ((defined __unix__) || (defined __APPLE__))
    boost::thread keyboard_thread(boost::bind(&Tetris_Draw_Linux_Console::detectKeyboardInputLinux, s, &T) );
    //boost::thread keyboard_thread(boost::bind(&Tetris_Draw_Linux_Console::detectKeyboardInputLinux, *((Tetris_Draw_Linux_Console*)show.get()), &T) );
#endif
    T.start();

#if ((defined __unix__) || (defined __APPLE__))
    keyboard_thread.interrupt();
    keyboard_thread.join();
#endif
    return 0;
}
