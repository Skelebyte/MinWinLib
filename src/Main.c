#include <stdio.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_RESIZABLE);
    int r, g, b;
    MWL_setBackgroundColor(50, 48, 47);

    MWL_Keybind red;
    MWL_createKeybind(&red, MWL_R);
    MWL_Keybind green;
    MWL_createKeybind(&green, MWL_G);
    MWL_Keybind blue;
    MWL_createKeybind(&blue, MWL_B);
    MWL_Keybind reset;
    MWL_createKeybind(&reset, MWL_F5);
    while(MWL_process(&window) != MWL_QUIT) {
        MWL_setBackgroundColor(r, g, b);
        if(MWL_processKeybind(&red)) {
            r++;
        }
        if(MWL_processKeybind(&green)) {
            g++;
        }
        if(MWL_processKeybind(&blue)) {
            b++;
        }
        if(MWL_processKeybind(&reset)) {
            r = 0;
            g = 0;
            b = 0;
        }
        MWL_waitForMillis(10);
    }

    

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
