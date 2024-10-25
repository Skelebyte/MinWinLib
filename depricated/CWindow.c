// #include <stdio.h>
// #include "CWindow.h"

// // bool windowCreated = false;

// #ifdef _WIN32


// Window createWindow(char* windowName) {
//     Window newWindow;

//     internalWindow.instance = GetModuleHandle(NULL);

//     internalWindow.className = L"CWindow"; // L means wide string

//     WNDCLASS windowClass = {0};
//     windowClass.lpszClassName = internalWindow.className;
//     windowClass.hInstance = internalWindow.instance;
//     windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//     windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//     windowClass.lpfnWndProc = windowProcess;

//     RegisterClass(&windowClass);

//     /*            title      |minimise in corner|display close, minimise etc            */
//     DWORD style = WS_CAPTION | WS_MINIMIZEBOX   | WS_SYSMENU; 

//     RECT rect;

//     int width = 640;
//     int height = 480;

//     rect.left = 250;
//     rect.top = 250;
//     rect.right = rect.left + width;
//     rect.bottom = rect.top + height;

//     AdjustWindowRect(&rect, style, FALSE);

//     internalWindow.hwnd = CreateWindowEx(
//         0,
//         internalWindow.className,
//         window.windowName,
//         style,
//         rect.left,
//         rect.top,
//         rect.right - rect.left, // width
//         rect.bottom - rect.top, // height
//         NULL,
//         NULL,
//         internalWindow.instance,
//         NULL
//     );

//     ShowWindow(internalWindow.hwnd, SW_SHOW);

//     return newWindow;
// }

// bool deleteWindow() {

//     UnregisterClass(internalWindow.className, internalWindow.instance);

//     return true;
// }

// bool processMessages() {

//     MSG message = {0};

//     while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {

//         if(message.message == WM_QUIT || window.shouldClose == true) {
//             return false;
//         }

//         TranslateMessage(&message);
//         DispatchMessage(&message);
//     }

//     return true;
// }

// LRESULT CALLBACK windowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

//     if(message == WM_CLOSE) {
//         deleteWindow(hwnd);
//     } else if(message == WM_DESTROY) {
//         PostQuitMessage(0);
//         return 0;
//     }

//     return DefWindowProc(hwnd, message, wParam, lParam);
// }

// #endif