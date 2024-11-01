#ifndef MINWINLIB_H
#define MINWINLIB_H

#include <stdio.h>
#include <stdbool.h>




/*
Window struct.
Holds data related the created window.

`windowName` - Name of the window. (`char*`)
`closed` - If the window needs to close. (`bool`)
`width` - Width of the window. (`int`)
`height` - Height of the window. (`int`)
*/
typedef struct {
    // Name of the window.
    char* windowName;
    // If the window needs to close.
    bool closed;
    // Width of the window.
    int width;
    // Height of the window.
    int height;

} MWL_Window;

// Has the MWL_Window been created?
bool MWL_windowCreated;

// Windows OS exclusive code
#ifdef _WIN32

#include <Windows.h>


// Internal Window
// ! WARNING ! This struct is strictly for uses INSIDE the MWL header. It is not compatable with other APIs or systems
// Should NOT be referenced in main code, it is not cross-platform.
typedef struct {

    HINSTANCE instance;
    HWND hwnd;

    wchar_t className;

    RECT rect;


} MWL_InternalWindow;


MWL_Window MWL_window;
MWL_InternalWindow MWL_internal;

static inline LRESULT CALLBACK MWL_windowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if(message == WM_CLOSE) {
        MWL_deleteWindow();
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
static inline MWL_Window MWL_createWindow(char* windowName, int width, int height, bool resizable) {
    MWL_Window newWindow;
    newWindow.windowName = windowName;
    newWindow.closed = false;

    MWL_internal.instance = GetModuleHandle(NULL);

    MWL_internal.className = L"MinWin"; // L means wide string



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
    windowClass.lpszClassName = MWL_internal.className;
    windowClass.lpszMenuName = windowName;
    windowClass.hInstance = MWL_internal.instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpfnWndProc = MWL_windowProcess;

    RegisterClass(&windowClass);

    MWL_internal.rect.left = 250;
    MWL_internal.rect.top = 250;
    MWL_internal.rect.right = MWL_internal.rect.left + width;
    MWL_internal.rect.bottom = MWL_internal.rect.top + height;

    AdjustWindowRect(&MWL_internal.rect, style, FALSE);

    MWL_internal.hwnd = CreateWindowEx(
        0,
        MWL_internal.className,
        newWindow.windowName,
        style,
        MWL_internal.rect.left,
        MWL_internal.rect.top,
        width, // can also be RECT.left - RECT.right
        height, // can also be RECT.bottom - RECT.top
        NULL,
        NULL,
        MWL_internal.instance,
        NULL
    );

    // check if hwnd failed to be created
    if(MWL_internal.hwnd == NULL) {
        MessageBox(MWL_internal.hwnd, "Could not create window.", "CWindow Err", MB_ICONERROR);
        MWL_Window empty = {0};
        return empty;

    }

    MWL_windowCreated = true;

    ShowWindow(MWL_internal.hwnd, SW_SHOW);

    return newWindow;
}

inline int MWL_deleteWindow() {
    MWL_windowCreated = false;

    /* strange error:
     src\MWL/MinWinLib.h:159:22: error: passing argument 1 of 'UnregisterClassW' makes pointer from integer without a cast [-Wint-conversion]
     159 |     UnregisterClassW((int) MWL_internal.className, MWL_internal.instance);
     |                      ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
     |                      |
     |                      int
    */
    // FIXME: this error also occurs where MWL_internal.className is referenced
    UnregisterClassW(MWL_internal.className, MWL_internal.instance);
    return 0;
}

static inline bool MWL_processMessages() {

    MSG message = {0};

    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {

        if(message.message == WM_QUIT/* || window.closed == true */) {
            return false;
        }
        RECT temp;
        GetWindowRect(MWL_internal.hwnd, &temp);

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
while(window.closed == false) {
    process();

    // what ever else the program does...
}
```
*/ 
static inline int MWL_process(MWL_Window* windowToProcess) {

    if(!MWL_processMessages()) {
        windowToProcess->closed = true;
    }

    if(UpdateWindow(MWL_internal.hwnd) == 0 && windowToProcess->closed == false) {
        MessageBox(MWL_internal.hwnd, "Window failed to update.", "CWindow Err", MB_ICONERROR);
        windowToProcess->closed = true;
        
        return 1;
    }

    return 0;
}

#endif








// TODO: Linux OS exclusive code
#ifdef __linux__ // thank you https://gist.github.com/m1nuz/8f8f10a7f8715b62fe79

#include <X11/Xlib.h> 
// Internal Window
// ! WARNING ! This struct is strictly for uses INSIDE the MWL header. It is not compatable with other APIs or systems
// Should NOT be referenced in main code, it is not cross-platform.
typedef struct {
    Display* display;
    Window root;
    Window window;
    XEvent event;
    Atom deleteWindow;
} MWL_InternalWindow;

MWL_InternalWindow MWL_internal;


static inline MWL_Window MWL_createWindow(char* windowName, int width, int height, bool resizable) {
    MWL_internal.display = XOpenDisplay(NULL);

    if(MWL_internal.display == NULL) {
        printf("Failed to open Display.\n");
        MWL_Window empty = {0};
        return empty;
    }

    MWL_internal.root = DefaultRootWindow(MWL_internal.display);

    if (MWL_internal.root == NULL) {
        printf("Failed to get display root.\n");
        MWL_Window empty = { 0 };

        XCloseDisplay(MWL_internal.display);

        return empty;
    }

    MWL_internal.window = XCreateSimpleWindow(MWL_internal.display, MWL_internal.root, 0, 0, width, height, 0, 0, 0, 0);

    if (MWL_internal.window == NULL) {
        printf("Failed to create window.\n");
        MWL_Window empty = { 0 };

        XCloseDisplay(MWL_internal.display);

        return empty;
    }

    XMapWindow(MWL_internal.display, MWL_internal.window);

    MWL_internal.deleteWindow = XInternAtom(MWL_internal.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(MWL_internal.display, MWL_internal.window, &deleteWindow, 1);

    MWL_Window window;
    window.windowName = windowName;
    window.closed = false;



    

}

inline int MWL_deleteWindow() {
    XCloseDisplay(MWL_internal.display);
}

static inline int MWL_process(MWL_Window* windowToProcess) {
    XNextEvent(MWL_internal.display, &MWL_internal.event);

    switch(MWL_internal.event.type) {
        case ClientMessage:
            if(MWL_internal.event.xclient.data.1[0] == MWL_internal.deleteWindow) {
                on_delete(MWL_internal.event.xclient.display, MWL_internal.event.xclient.window);
                windowToProcess->closed = true;
            }
            break;
    }

    return 0;
    
}

#endif

// TODO: mac exclusive code??       is it worth mac support? what poor soul uses mac?


#endif // MINWINLIB_H