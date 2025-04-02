#include <stdio.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_RESIZABLE);

    MWL_setBackgroundColor(50, 48, 47);

    MWL_Keybind bind;
    MWL_createKeybind(&bind, MWL_L);
    
    while(MWL_process(&window) != MWL_QUIT) {
        MWL_processKeybind(&bind);
        // if (GetKeyState('Z') & 0x8000) // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        // {
        //     // ALT key is down.
        //     printf("a\n");
        // }
        MWL_waitForMillis(10);
    }

    

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
