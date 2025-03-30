#include <stdio.h>
#include "MinWinLib.c"

int main(void) {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 300, 400, MWL_DEFAULT_FLAGS);
    int red = 0;
    int green = 0;
    int blue = 0;
    MWL_Color pixel;
    MWL_color(&pixel, red, green, blue);
    MWL_setBackgroundColor(pixel);
    while(MWL_process(&window) != MWL_QUIT) {
        MWL_setBackgroundColor(pixel);
        if(red < 255) {
            red += 4;
        }
        if(green < 255) {
            green += 5;
        }
        if(blue < 255) {
            blue += 1;
        }
        MWL_color(&pixel, red, green, blue);

        MWL_waitForMillis(10);

    }

    MWL_deleteWindow(&window);

    printf("Hi, mum!\n");

    return 0;
}
