#include <stdio.h>
#include "CWindow/CWindow.h"

int main() {
    printf("Creating window!\n");

    Window window = createWindow("Hi, mum!", 1000, 600);

    while(window.shouldClose == false) {

        process(&window);

        Sleep(10);
    }

    ("Should be closed?\n");
    return 0;

}