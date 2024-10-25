#ifndef CWINDOW_H
#define CWINDOW_H

#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

typedef struct {
    char* windowName;
    bool shouldClose;

    int width;
    int height;

} Window;

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

Window window;
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
static inline Window createWindow(char* windowName, int width, int height) {
    Window newWindow;
    newWindow.windowName = windowName;
    newWindow.shouldClose = false;

    internalWindow.instance = GetModuleHandle(NULL);

    internalWindow.className = L"CWindow"; // L means wide string

    WNDCLASS windowClass = {0};
    windowClass.lpszClassName = internalWindow.className;
    windowClass.lpszMenuName = windowName;
    windowClass.hInstance = internalWindow.instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpfnWndProc = windowProcess;

    RegisterClass(&windowClass);

    /*            title      |minimise in corner|display close, minimise etc            */
    DWORD style = WS_CAPTION | WS_MINIMIZEBOX   | WS_SYSMENU; 

    


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

    ShowWindow(internalWindow.hwnd, SW_SHOW);

    return newWindow;
}

inline bool deleteWindow() {

    UnregisterClass(internalWindow.className, internalWindow.instance);
    return true;
}

static inline bool processMessages() {

    MSG message = {0};

    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {

        if(message.message == WM_QUIT || window.shouldClose == true) {
            return false;
        }
        RECT temp;
        GetWindowRect(internalWindow.hwnd, &temp);

        window.width = temp.right - temp.left;
        window.height = temp.bottom - temp.top;

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return true;
}

#endif








// TODO: linux exclusive code
#ifdef __linux__



#endif

// TODO: mac exclusive code??       is it worth mac support? what poor soul uses mac?


#endif // CWINDOW_H