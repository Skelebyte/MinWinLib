#include <stdio.h>
#include "CWindow/CWindow.h"

int main() {
    printf("Creating window!\n");

    Window window = createWindow("Hi, mum!", 1000, 600);

    while(window.shouldClose == false) {
        process();

        Sleep(10);
    }


    return 0;

}