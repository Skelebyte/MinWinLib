#ifndef CWINDOW_H
#define CWINDOW_H

#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

/*
Window struct.
Holds data related the created window.

`windowName` - Name of the window. (`char*`)
`shouldClose` - If the window needs to close. (`bool`)
`width` - Width of the window. (`int`)
`height` - Height of the window. (`int`)
*/
typedef struct {
    // Name of the window.
    char* windowName;
    // If the window needs to close.
    bool shouldClose;
    // Width of the window.
    int width;
    // Height of the window.
    int height;

} Window;

// Has the window been created?
bool windowCreated;

// windows exclusive code
#ifdef _WIN32

#include <Windows.h>



typedef struct {
    HINSTANCE instance;
    HWND hwnd;

    wchar_t className;

    RECT rect;

} InternalWindow;


Window test;
InternalWindow internalWindow;

static inline LRESULT CALLBACK windowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if(message == WM_CLOSE) {
        deleteWindow(hwnd);
    } else if(message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

// TODO: allow for window resizing, unless specified otherwise
/*
Creates the window with specified name, width, height.
 
`windowName` - Name of the window. (`char*`)
`width` - Width of the window. (`int`)
`height` - Height of the window. (`int`)
 
```c
Window window = createWindow("Hi, mum!", 1000, 600);
```
*/
static inline Window createWindow(char* windowName, int width, int height, bool resizable) {
    Window newWindow;
    newWindow.windowName = windowName;
    newWindow.shouldClose = false;

    internalWindow.instance = GetModuleHandle(NULL);

    internalWindow.className = L"CWindow"; // L means wide string



    /*
    WS_CAPTION - window title
    WS_MAXIMIZEBOX - maximize button
    WS_MINIMIZEBOX - minimize button
    WS_SYSMENU - close button etc
    WS_SIZEBOX - allow window resizing
    */
    DWORD style;
    if(resizable == true) {
        style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
    } else {
        style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }

    //windowClass.style = style; CAUSES CRASH       why?

    WNDCLASS windowClass = {0};
    windowClass.lpszClassName = internalWindow.className;
    windowClass.lpszMenuName = windowName;
    windowClass.hInstance = internalWindow.instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpfnWndProc = windowProcess;

    RegisterClass(&windowClass);

    internalWindow.rect.left = 250;
    internalWindow.rect.top = 250;
    internalWindow.rect.right = internalWindow.rect.left + width;
    internalWindow.rect.bottom = internalWindow.rect.top + height;

    AdjustWindowRect(&internalWindow.rect, style, FALSE);

    internalWindow.hwnd = CreateWindowEx(
        0,
        internalWindow.className,
        newWindow.windowName,
        style,
        internalWindow.rect.left,
        internalWindow.rect.top,
        width, // can also be RECT.left - RECT.right
        height, // can also be RECT.bottom - RECT.top
        NULL,
        NULL,
        internalWindow.instance,
        NULL
    );

    // check if hwnd failed to be created
    if(internalWindow.hwnd == NULL) {
        MessageBox(internalWindow.hwnd, "Could not create window.", "CWindow Err", MB_ICONERROR);
        Window empty = {0};
        return empty;

    }

    windowCreated = true;

    ShowWindow(internalWindow.hwnd, SW_SHOW);

    return newWindow;
}

inline bool deleteWindow() {

    windowCreated = false;
    UnregisterClass(internalWindow.className, internalWindow.instance);
    return true;
}

static inline bool processMessages() {

    MSG message = {0};

    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {

        if(message.message == WM_QUIT/* || window.shouldClose == true */) {
            return false;
        }
        RECT temp;
        GetWindowRect(internalWindow.hwnd, &temp);

        // window.width = temp.right - temp.left;
        // window.height = temp.bottom - temp.top;

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return true;
}
/* 
Check if the window is trying to close.
Should be called in the programs while loop:
```c
while(window.shouldClose == false) {
    process();

    // what ever else the program does...
}
```
*/ 
static inline int process(Window* windowToProcess) {

    if(windowToProcess->shouldClose) deleteWindow();

    if(!processMessages()) {
        windowToProcess->shouldClose = true;
    }

    if(UpdateWindow(internalWindow.hwnd) == 0 && windowToProcess->shouldClose == false) {
        MessageBox(internalWindow.hwnd, "Window failed to update.", "CWindow Err", MB_ICONERROR);
        windowToProcess->shouldClose = true;
        
        return 1;
    }

    return 0;
}

#endif








// TODO: linux exclusive code
#ifdef __linux__

#include <X11/Xlib.h>



#endif

// TODO: mac exclusive code??       is it worth mac support? what poor soul uses mac?


#endif // CWINDOW_H