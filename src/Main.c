#include <stdio.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_RESIZABLE);
    int red = 0;
    int green = 0;
    int blue = 0;
    MWL_setBackgroundColor(red, green, blue);
    while(MWL_process(&window) != MWL_QUIT) {
        MWL_setBackgroundColor(red, green, blue);
        if(red < 255) {
            red += 4;
        }
        if(green < 255) {
            green += 5;
        }
        if(blue < 255) {
            blue += 1;
        }

        //MWL_waitForMillis(10);

    }

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
