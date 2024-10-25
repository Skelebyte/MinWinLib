#include <stdio.h>
#include "CWindow/CWindow.h"

int main() {
    printf("Creating window!\n");

    Window window = createWindow("Hi, mum!", 1000, 600);

    printf("(%i, %i)", window.width, window.height);

    while(window.shouldClose == false) {
        // do some crazy renderering 
        if(!processMessages()) {

            window.shouldClose = true;

        }

        Sleep(10);

    }


    return 0;

}