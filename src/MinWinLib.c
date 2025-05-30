/* MinWinLib.c */
#include <stdbool.h>
// OS specific includes
#ifdef _WIN32 // Windows
#include <windows.h>
#include <winnt.h>
#include <wingdi.h>
#elif __linux__ // Linux
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <unistd.h>
#endif

#define MWL_SUCCESS                0
#define MWL_KEY_PRESSED            1
#define MWL_KEY_RELEASED           0
#define MWL_FAILURE               -999
#define MWL_QUIT                   1
#define MWL_SCREEN_WIDTH_IS_ZERO  -1
#define MWL_SCREEN_HEIGHT_IS_ZERO -2

// _WIN32 class
#define MWL_WINDOW_CLASS "MWL Window Class"

// Flags
const int MWL_NO_FLAGS      = 0x00;
const int MWL_DEFAULT_FLAGS = 0x999;
const int MWL_RESIZABLE     = 0x01;

// Inputs
#ifdef _WIN32
// Mouse buttons
const int MWL_LMB    = 0x01; // Left mouse button
const int MWL_RMB    = 0x02; // Right mouse button
const int MWL_MMB    = 0x04; // Middle mouse button
const int MWL_PMB1   = 0x05; // Programmable mouse button 1
const int MWL_PMB2   = 0x06; // Programmable mouse button 2

// Keyboard keys
const int MWL_BCKSPC = 0x08; // Backspace
const int MWL_TAB    = 0x09; // Tab
const int MWL_ENTER  = 0x0D; // Return/Enter
const int MWL_LSHIFT = 0xA0; // Left shift
const int MWL_RSHIFT = 0xA1; // Right shift
const int MWL_LCTRL  = 0xA2; // Left control
const int MWL_RCTRL  = 0xA3; // Right control
const int MWL_LALT   = 0xA4; // Left alt
const int MWL_RALT   = 0xA5; // Left alt
const int MWL_CAPLCK = 0x14; // Caps lock
const int MWL_ESC    = 0x1B; // Escape
const int MWL_SPACE  = 0x20; // Space bar
const int MWL_LEFT   = 0x25; // Left arrow
const int MWL_UP     = 0x26; // Up arrow
const int MWL_RIGHT  = 0x27; // Right arrow
const int MWL_DOWN   = 0x28; // Down arrow
const int MWL_A      = 65  ; // A
const int MWL_B      = 66  ; // B
const int MWL_C      = 67  ; // C
const int MWL_D      = 68  ; // D
const int MWL_E      = 69  ; // E
const int MWL_F      = 70  ; // F
const int MWL_G      = 71  ; // G
const int MWL_H      = 72  ; // H
const int MWL_I      = 73  ; // I
const int MWL_J      = 74  ; // J
const int MWL_K      = 75  ; // K
const int MWL_L      = 76  ; // L
const int MWL_M      = 77  ; // M
const int MWL_N      = 78  ; // N
const int MWL_O      = 79  ; // O
const int MWL_P      = 80  ; // P
const int MWL_Q      = 81  ; // Q
const int MWL_R      = 82  ; // R
const int MWL_S      = 83  ; // S
const int MWL_T      = 84  ; // T
const int MWL_U      = 85  ; // U
const int MWL_V      = 86  ; // V
const int MWL_W      = 87  ; // W
const int MWL_X      = 88  ; // X
const int MWL_Y      = 89  ; // Y
const int MWL_Z      = 90  ; // Z
const int MWL_NMPD0  = 0x60; // Numpad 0
const int MWL_NMPD1  = 0x61; // Numpad 1
const int MWL_NMPD2  = 0x62; // Numpad 2
const int MWL_NMPD3  = 0x63; // Numpad 3
const int MWL_NMPD4  = 0x64; // Numpad 4
const int MWL_NMPD5  = 0x65; // Numpad 5
const int MWL_NMPD6  = 0x66; // Numpad 6
const int MWL_NMPD7  = 0x67; // Numpad 7
const int MWL_NMPD8  = 0x68; // Numpad 8
const int MWL_NMPD9  = 0x69; // Numpad 9
const int MWL_F1     = 0x70; // F1
const int MWL_F2     = 0x71; // F2
const int MWL_F3     = 0x72; // F3
const int MWL_F4     = 0x73; // F4
const int MWL_F5     = 0x74; // F5
const int MWL_F6     = 0x75; // F6
const int MWL_F7     = 0x76; // F7
const int MWL_F8     = 0x77; // F8
const int MWL_F9     = 0x78; // F9
const int MWL_F10    = 0x79; // F10
const int MWL_F11    = 0x80; // F11
const int MWL_F12    = 0x81; // F12
const int MWL_GRAVE  = 0xC0; // Grave/backtick/tilde (`~`)
#elif __linux__
// Keyboard keys
const int MWL_BCKSPC = 0xff08; // Backspace
const int MWL_TAB    = 0xff09; // Tab
const int MWL_ENTER  = 0xff0d; // Return/Enter
const int MWL_LSHIFT = 0xffe1; // Left shift
const int MWL_RSHIFT = 0xffe2; // Right shift
const int MWL_LCTRL  = 0xffe3; // Left control
const int MWL_RCTRL  = 0xffe4; // Right control
const int MWL_LALT   = 0xffe9; // Left alt
const int MWL_RALT   = 0xffea; // Left alt
const int MWL_CAPLCK = 0xffe5; // Caps lock
const int MWL_ESC    = 0xff1b; // Escape
const int MWL_SPACE  = 0x0020; // Space bar
const int MWL_LEFT   = 0xff51; // Left arrow
const int MWL_UP     = 0xff52; // Up arrow
const int MWL_RIGHT  = 0xff53; // Right arrow
const int MWL_DOWN   = 0xff54; // Down arrow
const int MWL_A      = 0x0061; // A
const int MWL_B      = 0x0062; // B
const int MWL_C      = 0x0063; // C
const int MWL_D      = 0x0064; // D
const int MWL_E      = 0x0065; // E
const int MWL_F      = 0x0066; // F
const int MWL_G      = 0x0067; // G
const int MWL_H      = 0x0068; // H
const int MWL_I      = 0x0069; // I
const int MWL_J      = 0x006a; // J
const int MWL_K      = 0x006b; // K
const int MWL_L      = 0x006c; // L
const int MWL_M      = 0x006d; // M
const int MWL_N      = 0x006e; // N
const int MWL_O      = 0x006f; // O
const int MWL_P      = 0x0070; // P
const int MWL_Q      = 0x0071; // Q
const int MWL_R      = 0x0072; // R
const int MWL_S      = 0x0073; // S
const int MWL_T      = 0x0074; // T
const int MWL_U      = 0x0075; // U
const int MWL_V      = 0x0076; // V
const int MWL_W      = 0x0077; // W
const int MWL_X      = 0x0078; // X
const int MWL_Y      = 0x0079; // Y
const int MWL_Z      = 0x007a; // Z
const int MWL_NMPD0  = 0xffb0; // Numpad 0
const int MWL_NMPD1  = 0xffb1; // Numpad 1
const int MWL_NMPD2  = 0xffb2; // Numpad 2
const int MWL_NMPD3  = 0xffb3; // Numpad 3
const int MWL_NMPD4  = 0xffb4; // Numpad 4
const int MWL_NMPD5  = 0xffb5; // Numpad 5
const int MWL_NMPD6  = 0xffb6; // Numpad 6
const int MWL_NMPD7  = 0xffb7; // Numpad 7
const int MWL_NMPD8  = 0xffb8; // Numpad 8
const int MWL_NMPD9  = 0xffb9; // Numpad 9
const int MWL_F1     = 0xffbe; // F1
const int MWL_F2     = 0xffbf; // F2
const int MWL_F3     = 0xffc0; // F3
const int MWL_F4     = 0xffc1; // F4
const int MWL_F5     = 0xffc2; // F5
const int MWL_F6     = 0xffc3; // F6
const int MWL_F7     = 0xffc4; // F7
const int MWL_F8     = 0xffc5; // F8
const int MWL_F9     = 0xffc6; // F9
const int MWL_F10    = 0xffc7; // F10
const int MWL_F11    = 0xffc8; // F11
const int MWL_F12    = 0xffc9; // F12
const int MWL_GRAVE  = 0x0060; // Grave/backtick/tilde (`~`)
#endif


/**
 * @brief Contains variables that relate to the window.
 * @param windowTitle The name of the window
 * @param width Width of the window in pixels
 * @param height Height of the window in pixels
 * @param running If the window is currently running
 * @param cursorVisible If the cursor is visible
 */
typedef struct MWL_Window {
    const char* windowTitle;
    int width;
    int height;
    bool running;
    bool cursorVisible;

} MWL_Window;


typedef struct MWL_InternalWindow {
    bool shouldCloseWindow;
    #ifdef _WIN32
	WNDCLASSEX windowClass;
	MSG message;
    HWND hwnd;
    RECT rect;
    HDC hdc;
    bool changeColor;
    int r;
    int g;
    int b;
    #elif __linux__
    Window window;
    Display* display;
    XEvent event;
    Atom deleteWindow;
    Cursor cursor;
    #endif

} MWL_InternalWindow;


static MWL_InternalWindow mwl_internalWindow;

/**
 * @brief Get the dimensions of the screen.
 * @param w The target variable to set the width to
 * @param h The target variable to set the height to
 * @returns `MWL_SUCCESS`
 */
int MWL_getScreenDimensions(int* w, int* h) {
    #ifdef _WIN32
    int _w = GetSystemMetrics(SM_CXSCREEN);
    int _h = GetSystemMetrics(SM_CYSCREEN);
    #elif __linux__
    int _w = DisplayWidth(
        mwl_internalWindow.display, 
        DefaultScreen(mwl_internalWindow.display)
    );

    int _h = DisplayHeight(
        mwl_internalWindow.display, 
        DefaultScreen(mwl_internalWindow.display)
    );
    #endif

    if(_w == 0) {
        return MWL_SCREEN_WIDTH_IS_ZERO;
    }
    if(_h == 0) {
        return MWL_SCREEN_HEIGHT_IS_ZERO;
    }

    *w = _w;
    *h = _h;
    return MWL_SUCCESS;
}

#ifdef _WIN32
LRESULT CALLBACK MWL_windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
        case WM_CLOSE:
        mwl_internalWindow.shouldCloseWindow = true;
        break;
        case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        case WM_ERASEBKGND: {
            RECT rect;
            mwl_internalWindow.hdc = (HDC)wParam;

            
            
            GetClientRect(mwl_internalWindow.hwnd, &rect);
            HBRUSH brush = (HBRUSH)CreateSolidBrush(
                RGB(mwl_internalWindow.r, mwl_internalWindow.g, mwl_internalWindow.b)
            );
            
            FillRect(mwl_internalWindow.hdc, &rect, brush);
            DeleteObject(brush);
            
            return 1;
        }
        
    }

    

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

/**
 * @brief Creates a window with specified width, height, and flags.
 * @param window Reference to the `MWL_Window` variable
 * @param t The title of the window
 * @param w Width of the window
 * @param h Height of the window
 * @param flags Flags for the window, such as `MWL_RESIZABLE`
 * @returns `MWL_SUCCESS` 
 * 
 * `MWL_createWindow(&myWindow, "Hello, world!", 600, 400, MWL_RESIZABLE)`
 */
int MWL_createWindow(MWL_Window* window, const char* t, int w, int h, int flags) {
    #ifdef _WIN32
    DWORD style;
    style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    if(flags & MWL_DEFAULT_FLAGS) {
        style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
    }

    if(flags & MWL_RESIZABLE) {
        style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
    }

	mwl_internalWindow.windowClass.cbSize = sizeof(WNDCLASSEX);
	mwl_internalWindow.windowClass.lpfnWndProc   = MWL_windowProcess;
	mwl_internalWindow.windowClass.cbClsExtra    = 0;
	mwl_internalWindow.windowClass.cbWndExtra    = 0;
	mwl_internalWindow.windowClass.hInstance     = GetModuleHandle(NULL);
	mwl_internalWindow.windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	mwl_internalWindow.windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	mwl_internalWindow.windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	mwl_internalWindow.windowClass.lpszMenuName  = NULL;
	mwl_internalWindow.windowClass.lpszClassName = TEXT(MWL_WINDOW_CLASS);
	mwl_internalWindow.windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&mwl_internalWindow.windowClass);



	mwl_internalWindow.hwnd = CreateWindowEx(
		0,
		TEXT(MWL_WINDOW_CLASS),
		t,
		style,
		mwl_internalWindow.rect.left,
        mwl_internalWindow.rect.top,
		w,
		h,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);


    mwl_internalWindow.rect = (RECT){0};

    // Get center of screen then offset by half width and height of window
    int xPos = ((GetSystemMetrics(SM_CXSCREEN) - mwl_internalWindow.rect.right)  / 2)  - w / 2;
    int yPos = ((GetSystemMetrics(SM_CYSCREEN) - mwl_internalWindow.rect.bottom) / 2)  - h / 2;

    //SetWindowPos(mwl_internalWindow.hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    ShowWindow(mwl_internalWindow.hwnd, SW_SHOW);
    UpdateWindow(mwl_internalWindow.hwnd);
    #elif __linux__
    mwl_internalWindow.display = XOpenDisplay(NULL);
    int screenW, screenH;
    MWL_getScreenDimensions(&screenW, &screenH);

    mwl_internalWindow.window = XCreateSimpleWindow(
        mwl_internalWindow.display,
        DefaultRootWindow(mwl_internalWindow.display),
        screenW / 2,
        screenH / 2,
        w,
        h,
        1,
        BlackPixel(mwl_internalWindow.display, 0),
        WhitePixel(mwl_internalWindow.display, 0)
    );
    XStoreName(mwl_internalWindow.display, mwl_internalWindow.window, t);
    XMapWindow(mwl_internalWindow.display, mwl_internalWindow.window);
    XSelectInput(
        mwl_internalWindow.display, 
        mwl_internalWindow.window, 
        ExposureMask | KeyReleaseMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask
    );

	if(flags & MWL_DEFAULT_FLAGS) {
	   // default stuff... 0
    }

	// Check if resizing is not supported
    if(!(flags & MWL_RESIZABLE)) {
        XSizeHints* hints = XAllocSizeHints();
        hints->flags = PMinSize | PMaxSize;
        hints->min_width = w;
        hints->max_width = w;
        hints->min_height = h;
        hints->max_height = h;
        XSetWMNormalHints(mwl_internalWindow.display, mwl_internalWindow.window, hints);
        XFree(hints);
    }

    mwl_internalWindow.deleteWindow = XInternAtom(mwl_internalWindow.display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(mwl_internalWindow.display, mwl_internalWindow.window, &mwl_internalWindow.deleteWindow, 1);
    mwl_internalWindow.cursor = 0;
    #endif

    window->windowTitle   = t;
    window->width         = w;
    window->height        = h;
    window->running       = true;
    window->cursorVisible = true;

    return MWL_SUCCESS;

}

/**
 * @brief Deletes the window.
 * @param window Window to delete
 * @returns `MWL_SUCCESS`
 */
int MWL_deleteWindow(MWL_Window* window) {

    #ifdef _WIN32
    DestroyWindow(mwl_internalWindow.hwnd);
    #elif __linux__
    XCloseDisplay(mwl_internalWindow.display);
    #endif

    return MWL_SUCCESS;
}

/**
 * @brief checks if a specified key is down
 * @param `keycode` The key to process
 * @returns `MWL_KEY_PRESSED` if the key is down or `MWL_KEY_RELEASED` if it is not
 */
int MWL_processKey(int keycode) {
    #if _WIN32
    if (GetKeyState(keycode) & 0x8000) // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    {
        return MWL_KEY_PRESSED;
    }
    #elif __linux__
    if(mwl_internalWindow.event.type == KeyPress) {
        KeySym keysym = XLookupKeysym(&mwl_internalWindow.event.xkey, 0);
        if(keysym == keycode) {
            return MWL_KEY_PRESSED;
        }
        
    }
    #endif
    return MWL_KEY_RELEASED;
}

/**
 * @brief Handles the windows events.
 * @param window The target window to handle
 * @returns `MWL_SUCCESS` or `MWL_QUIT` if the window is trying to close
 */
int MWL_process(MWL_Window* window) {
    if(mwl_internalWindow.shouldCloseWindow == true) {
        return MWL_QUIT;
    }

    #ifdef _WIN32

    MSG message = {0};
    
	while(PeekMessage(&message, mwl_internalWindow.hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

    return mwl_internalWindow.message.wParam;
    #elif __linux__

    if(mwl_internalWindow.event.type == ClientMessage) {
        if(mwl_internalWindow.event.xclient.data.l[0] == mwl_internalWindow.deleteWindow) {

            return MWL_QUIT;
        }
    }

    while(XPending(mwl_internalWindow.display)) {
        XNextEvent(mwl_internalWindow.display, &mwl_internalWindow.event);
    }

    #endif


    return MWL_SUCCESS;
}

/**
 * @brief Sets the background color of the window.
 * @param r The red value
 * @param g The green value
 * @param b The blue value
 * @returns `MWL_SUCCESS`
 */
int MWL_setBackgroundColor(int r, int g, int b) {
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;

    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    #ifdef _WIN32
    mwl_internalWindow.r = r;
    mwl_internalWindow.g = g;
    mwl_internalWindow.b = b;
    InvalidateRect(mwl_internalWindow.hwnd, NULL, true);
    #elif __linux__
    XSetWindowBackground(
        mwl_internalWindow.display, 
        mwl_internalWindow.window, 
        (r << 16) | (g << 8) | (b)
    );
    XClearWindow(mwl_internalWindow.display, mwl_internalWindow.window);
    #endif

    return MWL_SUCCESS;
}

/**
 * @brief Waits for the amount of specified milliseconds before continuing.
 * @param amount The amount of milliseconds to wait for
 * @returns `MWL_SUCCESS`
 */
int MWL_waitForMillis(int amount) {
    #ifdef _WIN32
    Sleep(amount);
    #elif __linux__
    usleep(amount * 1000);
    #endif

    return MWL_SUCCESS;
}

//TODO move this into `MWL_process`, and set the `width` and `heigh` variables in the window
/**
 * @brief Gets the size of the window.
 * @param w Reference to the width variable
 * @param h Reference to the height variable
 * @returns `MWL_SUCCESS`
 */
int MWL_getWindowSize(int* w, int* h) {
    int tempW;
    int tempH;

    #ifdef _WIN32
    RECT rect;

    if(GetWindowRect(mwl_internalWindow.hwnd, &rect)) {
        tempW = rect.right - rect.left;
        tempH = rect.bottom - rect.top;
    }
    #elif __linux__

    #endif
    
    return MWL_SUCCESS;
}

/**
 * @brief Gets the position of the window.
 * @param x Reference to the `x` coordinate
 * @param y Reference to the `y` coordinate
 * @returns `MWL_SUCCESS`
 */
int MWL_getWindowPos(int* x, int* y) {
    int tempX;
    int tempY;
    #ifdef _WIN32
    RECT rect;
    GetWindowRect(mwl_internalWindow.hwnd, &rect);
    tempX = rect.left;
    tempY = rect.top;
    #elif __linux__
    XWindowAttributes windowAttrib;
    XGetWindowAttributes(mwl_internalWindow.display, mwl_internalWindow.window, &windowAttrib);
    tempX = windowAttrib.x;
    tempY = windowAttrib.y;
    #endif

    if(tempX < 0) {
        tempX = 0;
    }
    if(tempY < 0) {
        tempY = 0;
    }
    *x = tempX;
    *y = tempY;
    
    return MWL_SUCCESS;
}

/**
 * @brief Gets the position of the cursor, relative to the window.
 * @param x Reference to the `x` coordinate
 * @param y Reference to the `y` coordinate
 * @returns `MWL_SUCCESS`
 */
int MWL_getCursorPos(int* x, int* y) {
    int tempX;
    int tempY;

    #ifdef _WIN32
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(mwl_internalWindow.hwnd, &point);
    tempX = point.x;
    tempY = point.y;

    #elif __linux__
    tempX = mwl_internalWindow.event.xbutton.x;
    tempY = mwl_internalWindow.event.xbutton.y;
    #endif
    


    *x = tempX;
    *y = tempY;
    
    return MWL_SUCCESS;

}

/**
 * @brief Toggles visibility of the cursor.
 * @param window Reference to the window
 * @returns `MWL_SUCCESS`
 */
int MWL_toggleCursor(MWL_Window* window) {
    window->cursorVisible = !window->cursorVisible;
    #ifdef _WIN32
    ShowCursor(window->cursorVisible);
    #elif __linux__
    if(window->cursorVisible) {
        XDefineCursor(mwl_internalWindow.display, mwl_internalWindow.window, mwl_internalWindow.cursor);
    } else {
        XUndefineCursor(mwl_internalWindow.display, mwl_internalWindow.window);
    }
    #endif
}