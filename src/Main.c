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
            red += 25;
        }
        if(green < 255) {
            green += 25;
        }
        if(blue < 255) {
            blue += 25;
        }

        printf("r: %i\n", red);
        printf("g: %i\n", green);
        printf("b: %i\n", blue);

        MWL_waitForMillis(1);
        printf("Endofframe\n");
    }

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
