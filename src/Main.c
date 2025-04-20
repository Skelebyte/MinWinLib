#include <stdio.h>
#include <stdlib.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, 0);
    while(MWL_process(&window) != MWL_QUIT) {
        if(MWL_processKey(MWL_SPACE)) {
            MWL_toggleCursor(&window); // TODO: make sure the linux side of this actually works
        }
        MWL_waitForMillis(10);
    }

    

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");
    return 0;
}
