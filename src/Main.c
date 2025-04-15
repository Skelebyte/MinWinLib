#include <stdio.h>
#include <stdlib.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_RESIZABLE);
    int x = 0;
    int y = 0;
    while(MWL_process(&window) != MWL_QUIT) {
        MWL_getCursorPos(&x, &y);
        printf("%d, %d \n", x, y);
        MWL_waitForMillis(10);
    }

    

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");
    return 0;
}
