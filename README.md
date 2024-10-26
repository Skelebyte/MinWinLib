# CWindow
CWindow is a simple, single-file library to create and manage windows, currently supporting Win32 system.
> [!IMPORTANT]
> Linux support is coming! (with X11)


## Creating a window
Super simple, just include the header and call `createWindow`.
```c
#include <stdio.h>
#include "path/to/CWindow.h"

int main() {
    Window window = createWindow("Hi, mum!", 1000, 600);
    return 0;
}
```
To make sure the window doesn't close the moment you run the code you need a while loop, and call the `process` function.
```c
int main() {
    printf("Creating window!\n");

    Window window = createWindow("Hi, mum!", 1000, 600);

    while(window.shouldClose == false) {
        process();

        Sleep(10);
    }

    printf("Closing...");
    return 0;

}
```
