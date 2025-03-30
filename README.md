> [!WARNING]
> This is completely out of date

# MWL
MinWinLib (MWL) is a simple, single-file library to create and manage windows, currently only supporting the Win32 API.
> [!IMPORTANT]
> Linux support is coming! (with X11)


## Creating a window with MWL
Creating a window is simple, just include `MinWinLib.h` and call `MWL_createWindow`.
```c
#include <stdio.h>
#include "path/to/MinWinLib.h"

int main() {
    MWL_Window window = MWL_createWindow("Hi, mum!", 1000, 600, true);
    return 0;
}
```
To make sure the window doesn't crash the moment you run the code you need to call `MWL_process` function and pass in a reference to a `MWL_Window`.
This must be done inside a while loop that checks to see if the window is closed, or the program will crash.
```c
int main() {
    printf("Creating window!\n");

    MWL_Window window = MWL_createWindow("Hi, mum!", 1000, 600, true);

// The loop will run only if the window is open. This is where your program's main code goes.
    while(window.closed == false) {
        MWL_process(&window);

        Sleep(10);
    }

    printf("Loop ended\n");
    return 0;

}
```
