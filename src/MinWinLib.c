/* MinWinLib.c */
#include <stdbool.h>

#define MWL_SUCCESS 0
#define MWL_FAILURE -999
#define MWL_QUIT 1
#define MWL_SCREEN_WIDTH_IS_ZERO -1
#define MWL_SCREEN_HEIGHT_IS_ZERO -2

// _WIN32 class
#define MWL_WINDOW_CLASS "MWL Window Class"

// Flags
const int MWL_NO_FLAGS = 0x00;
const int MWL_DEFAULT_FLAGS = 0x999;
const int MWL_RESIZABLE = 0x01;



typedef struct MWL_Window {
    const char* windowTitle;
    int width;
    int height;
    bool running;
} MWL_Window;

// OS specific inlcudes
#ifdef _WIN32 // Windows
#include <windows.h>
#include <wingdi.h>
#elif __linux__ // Linux
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#endif

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
            if(mwl_internalWindow.changeColor) {
                mwl_internalWindow.hdc = (HDC)wParam;
                RECT rect;
            
                GetClientRect(mwl_internalWindow.hwnd, &rect);
                HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(mwl_internalWindow.r, mwl_internalWindow.g, mwl_internalWindow.b));
            
                FillRect(mwl_internalWindow.hdc, &rect, brush);
                DeleteObject(brush);
                mwl_internalWindow.changeColor = false;
                return 1;
            }
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
	// mwl_internalWindow.windowClass.style = NULL;
	mwl_internalWindow.windowClass.lpfnWndProc = MWL_windowProcess;
	mwl_internalWindow.windowClass.cbClsExtra = 0;
	mwl_internalWindow.windowClass.cbWndExtra = 0;
	mwl_internalWindow.windowClass.hInstance = GetModuleHandle(NULL);
	mwl_internalWindow.windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	mwl_internalWindow.windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mwl_internalWindow.windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	mwl_internalWindow.windowClass.lpszMenuName  = NULL;
	mwl_internalWindow.windowClass.lpszClassName = TEXT(MWL_WINDOW_CLASS);
	mwl_internalWindow.windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&mwl_internalWindow.windowClass);



	mwl_internalWindow.hwnd = CreateWindowEx(
		0,
		TEXT(MWL_WINDOW_CLASS),
		TEXT(t),
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
    int xPos = ((GetSystemMetrics(SM_CXSCREEN) - mwl_internalWindow.rect.right) / 2) - w / 2;
    int yPos = ((GetSystemMetrics(SM_CYSCREEN) - mwl_internalWindow.rect.bottom) / 2) - h / 2;

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
    XSelectInput(mwl_internalWindow.display, mwl_internalWindow.window, ExposureMask);

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
    window->width = w;
    window->height = h;
    window->running = true;

    return MWL_SUCCESS;

}

int MWL_deleteWindow(MWL_Window* window) {

    #ifdef _WIN32
    DestroyWindow(mwl_internalWindow.hwnd);
    #elif __linux__
    XCloseDisplay(mwl_internalWindow.display);
    #endif

    return MWL_SUCCESS;
}

int MWL_process(MWL_Window* windowToProcess) {
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
    mwl_internalWindow.changeColor = true;
    mwl_internalWindow.r = r;
    mwl_internalWindow.g = g;
    mwl_internalWindow.b = b;
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
    Sleep(10);
    #elif __linux__
    usleep(amount * 1000);
    #endif

    return MWL_SUCCESS;
}
