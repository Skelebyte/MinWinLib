# MWL
MinWinLib (MWL) is a simple, single-file library to create a window, supporting Windows (Win32) and Linux (X11).

## Creating a window with MWL
Creating a window is simple, just include `MinWinLib.c` and call `MWL_createWindow`.
```c
#include <stdio.h>
#include "path/to/MinWinLib.c"

int main() {
    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 1000, 600, MWL_DEFAULT_FLAGS);
    return 0;
}
```
To make sure the window doesn't close the moment you run the code you need to call `MWL_process` function and pass in a reference to a `MWL_Window`.
This must be done inside a while loop that checks to check events from the window.
```c
int main() {
    printf("Creating window!\n");

    MWL_Window window;
    MWL_createWindow(&window, "Hi, mum!", 1000, 600, MWL_DEFAULT_FLAGS);

// The loop will run only if the window is open. This is where your program's main code goes.
    while(MWL_process(&window) != MWL_QUIT) {
        // Code goes here...
    }

    printf("Loop ended\n");
    return 0;

}
```
