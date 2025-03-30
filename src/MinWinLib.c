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



typedef struct {
    const char* windowTitle;
    int width;
    int height;
    bool running;
} MWL_Window;

// System specific inlcudes
#ifdef _WIN32

#include <windows.h>

#elif __linux__
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
    #elif __linux__
    Window window;
    Display* display;
    XEvent event;
    Atom deleteWindow;
    #endif

} MWL_InternalWindow;

typedef struct MWL_Pixel {
    int red;
    int green;
    int blue;
    unsigned long color;
} MWL_Color;

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

    w = &_w;
    h = &_h;
    return MWL_SUCCESS;
}

#ifdef _WIN32
LRESULT CALLBACK MWL_windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if(msg == WM_CLOSE) {
        mwl_internalWindow.shouldCloseWindow = true;

    } else if(msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif


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
	mwl_internalWindow.windowClass.style = style;
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

	// Check if resizing is supported
    if(!(flags & MWL_RESIZABLE)) {
        XSizeHints* hints = XAllocSizeHints();
        hints->flags = PMinSize | PMaxSize;
        hints->min_width = w;
        hints->max_width = w;
        hints->min_height = h;
        hints->max_height = h;
        XSetWMNormalHints(mwl_internalWindow.display, mwl_internalWindow.window, hints);
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
 * @brief Returns an `unsigned long` that represents a color value (RGB)
 * @param r Red value
 * @param g Green value
 * @param b Blue value
*/
int MWL_color(MWL_Color* color, int r, int g, int b) {
    // make sure the values aren't more than 255
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;

    // make sure the values aren't less than 0
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;

    color->red = r;
    color->green = g;
    color->blue = b;
    color->color = (r << 16) | (g << 8) | (b);

    return MWL_SUCCESS;
}
/**
 * @brief Sets the background color of the window.
 * @param pixel The `MWL_Color` value
*/
int MWL_setBackgroundColor(MWL_Color color) {
    #ifdef _WIN32
    mwl_internalWindow.windowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(color.red, color.green, color.blue));
    #elif __linux__
    XSetWindowBackground(mwl_internalWindow.display, mwl_internalWindow.window, color.color);
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
