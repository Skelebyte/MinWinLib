/* MinWinLib.c */
#include <stdbool.h>
// OS specific inlcudes
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
#define MWL_KEYBIND_ACTIVE         0
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
const int MWL_RCTRL  = 0xA2; // Right control
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
const int MWL_NMPD8  = 0x67; // Numpad 8
const int MWL_NMPD9  = 0x67; // Numpad 9
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
const int MWL_GRAVE  = 0xC0; // Grave
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
// A little ulgy but a temporary solution to the either uppercase or lowercase
// letters is to OR them together
const int MWL_A      = 0x0041 | 0x0061; // A
const int MWL_B      = 0x0042 | 0x0062; // B
const int MWL_C      = 0x0043 | 0x0063; // C
const int MWL_D      = 0x0044 | 0x0064; // D
const int MWL_E      = 0x0045 | 0x0065; // E
const int MWL_F      = 0x0046 | 0x0066; // F
const int MWL_G      = 0x0047 | 0x0067; // G
const int MWL_H      = 0x0048 | 0x0068; // H
const int MWL_I      = 0x0049 | 0x0069; // I
const int MWL_J      = 0x004a | 0x006a; // J
const int MWL_K      = 0x004b | 0x006b; // K
const int MWL_L      = 0x004c | 0x006c; // L
const int MWL_M      = 0x004d | 0x006d; // M
const int MWL_N      = 0x004e | 0x006e; // N
const int MWL_O      = 0x004f | 0x006f; // O
const int MWL_P      = 0x0050 | 0x0070; // P
const int MWL_Q      = 0x0051 | 0x0071; // Q
const int MWL_R      = 0x0052 | 0x0072; // R
const int MWL_S      = 0x0053 | 0x0073; // S
const int MWL_T      = 0x0054 | 0x0074; // T
const int MWL_U      = 0x0055 | 0x0075; // U
const int MWL_V      = 0x0056 | 0x0076; // V
const int MWL_W      = 0x0057 | 0x0077; // W
const int MWL_X      = 0x0058 | 0x0078; // X
const int MWL_Y      = 0x0059 | 0x0079; // Y
const int MWL_Z      = 0x005a | 0x007a; // Z
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
const int MWL_GRAVE  = 0x0060; // Grave
#endif



typedef struct MWL_Window {
    const char* windowTitle;
    int width;
    int height;
    bool running;
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
    #endif

} MWL_InternalWindow;

typedef struct MWL_Keybind {
    int keycode;
    bool active;
} MWL_Keybind;


static MWL_InternalWindow mwl_internalWindow;

/**
 * @brief Get the dimensions of the screen.
 * @param w The target variable to set the width to
 * @param h The target variable to set the height to
 */
int MWL_getScreenDimensions(int* w, int* h) {
    #ifdef _WIN32
    int _w = GetSystemMetrics(SM_CXSCREEN);
    int _h = GetSystemMetrics(SM_CYSCREEN);
    #elif __linux__
    int _w = DisplayWidth(mwl_internalWindow.display, DefaultScreen(mwl_internalWindow.display));
    int _h = DisplayHeight(mwl_internalWindow.display, DefaultScreen(mwl_internalWindow.display));
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
            HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(mwl_internalWindow.r, mwl_internalWindow.g, mwl_internalWindow.b));
            
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
	// mwl_internalWindow.windowClass.style      = NULL; // <-- causes crash if not NULL
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
    ShowWindow(mwl_internalWindow.hwnd, SW_SHOW);
    UpdateWindow(mwl_internalWindow.hwnd);

    mwl_internalWindow.rect = (RECT){0};

    // Get center of screen then offset by half width and height of window
    int xPos = ((GetSystemMetrics(SM_CXSCREEN) - mwl_internalWindow.rect.right)  / 2)  - w / 2;
    int yPos = ((GetSystemMetrics(SM_CYSCREEN) - mwl_internalWindow.rect.bottom) / 2)  - h / 2;

    SetWindowPos(mwl_internalWindow.hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

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
    XSelectInput(mwl_internalWindow.display, mwl_internalWindow.window, ExposureMask | KeyReleaseMask | KeyPressMask);

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
    #endif

    window->windowTitle = t;
    window->width       = w;
    window->height      = h;
    window->running     = true;

    return MWL_SUCCESS;

}

/**
 * @brief Deletes the window.
 * @param window Window to delete
 */
int MWL_deleteWindow(MWL_Window* window) {

    #ifdef _WIN32
    DestroyWindow(mwl_internalWindow.hwnd);
    #elif __linux__
    XCloseDisplay(mwl_internalWindow.display);
    #endif

    return MWL_SUCCESS;
}

int MWL_createKeybind(MWL_Keybind* keybind, int kc) {
    keybind->keycode = kc;
    keybind->active = false;
    return MWL_SUCCESS;
}

int MWL_processKeybind(MWL_Keybind* keybind) {
    #if _WIN32

    #elif __linux__
    if(mwl_internalWindow.event.type == KeyPress) {
        KeySym key = XLookupKeysym(&mwl_internalWindow.event.xkey, 0);
        if(key == keybind->keycode) {
            printf("gogo\n");
        }
        
    }
    #endif
}

/**
 * @brief Handles the windows events.
 * @param window The target window to handle
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
 */
int MWL_setBackgroundColor(int r, int g, int b) {
    #ifdef _WIN32
    mwl_internalWindow.r = r;
    mwl_internalWindow.g = g;
    mwl_internalWindow.b = b;
    InvalidateRect(mwl_internalWindow.hwnd, NULL, true);
    #elif __linux__
    XSetWindowBackground(mwl_internalWindow.display, mwl_internalWindow.window, (r << 16) | (g << 8) | (b));
    XClearWindow(mwl_internalWindow.display, mwl_internalWindow.window);
    #endif

    return MWL_SUCCESS;
}

/**
 * @brief Waits for the amount of specified milliseconds before continuing.
 * @param amount The amount of milliseconds to wait for
 */
int MWL_waitForMillis(int amount) {
    #ifdef _WIN32
    Sleep(amount);
    #elif __linux__
    usleep(amount * 1000);
    #endif

    return MWL_SUCCESS;
}

