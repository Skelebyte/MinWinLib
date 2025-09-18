#include <stdio.h>
#include <stdlib.h>
// #include <EGL/egl.h>
#define MWL_USE_GLAD "glad/gl.h"
#include "MinWinLib.c"
int main(void)
{
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_RESIZABLE_WINDOW);
    int color = 255;
    while (MWL_process(&window) != MWL_QUIT)
    {
        if(MWL_processKey(MWL_SPACE) == MWL_KEY_PRESSED)
        {
            color -= 5;
        }
        MWL_setBackgroundColor(color, color, color);
        MWL_waitForMillis(10);
    }

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");
    return 0;
}
