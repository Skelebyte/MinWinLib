// #ifndef CWINDOW_H
// #define CWINDOW_H

// typedef int bool;
// #define true 1
// #define false 0

// typedef struct {
//     char* windowName;
//     bool shouldClose;
// } Window;

// bool windowCreated;

// // windows exclusive code
// #ifdef _WIN32

// #include <Windows.h>

// LRESULT CALLBACK windowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// typedef struct {
//     HINSTANCE instance;
//     HWND hwnd;

//     wchar_t className;

// } InternalWindow;

// Window window;
// InternalWindow internalWindow;


// Window createWindow(char* windowName);
// bool deleteWindow();

// bool processMessages();

// #endif








// // TODO: linux exclusive code
// #ifdef __linux__



// #endif

// // TODO: mac exclusive code??       is it worth mac support? what poor soul uses mac?


// #endif