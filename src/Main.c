#include <stdio.h>
#include "MWL/MinWinLib.h"

int main() {
    printf("Creating window!\n");

    MWL_Window window = MWL_createWindow("Hi, mum!", 1000, 600, true);

    while(window.closed == false) {
        MWL_process(&window);

        Sleep(10);
    }

    printf("Loop ended\n");
    return 0;

}