#include <stdio.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_NO_FLAGS);
    int red = 50;
    int green = 48;
    int blue = 47;
    MWL_setBackgroundColor(red, green, blue);
    while(MWL_process(&window) != MWL_QUIT) {


        MWL_waitForMillis(10);
    }

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
