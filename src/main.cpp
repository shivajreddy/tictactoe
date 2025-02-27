#include <windows.h>

#include <stdio.h>

// Window class name and title
#define TTT_WINCLASS_NAME "TicTacToe"
#define TTT_WINTITLE "TicTacToe"
#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Global variables
BOOL IsRunning = false;
NOTIFYICONDATA nid; // Tray icon

// Function to add an icon to the system tray
void AddTrayIcon(HWND hwnd)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT("Minimized to Tray"));

    Shell_NotifyIcon(NIM_ADD, &nid);
}

// Forward declarations
LRESULT CALLBACK MainWindowCallbackProcedure(HWND, UINT, WPARAM, LPARAM);
void handle_keyboard_events(HWND, UINT, WPARAM, LPARAM);
void RenderBackBuffer(HWND, HDC);

// Application entry point
int APIENTRY wWinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance,
                      _In_ LPWSTR CommandLine, _In_ int ShowCommand)
{
    // 1. Register the Window Class
    WNDCLASS WindowClass = { 0 };
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallbackProcedure;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = TTT_WINCLASS_NAME;

    if (!RegisterClass(&WindowClass)) return EXIT_FAILURE;

    // 2. Initialize the Main Window Instance
    HWND WindowHandle =
        CreateWindowEx(0,                 // Optional window styles
                       TTT_WINCLASS_NAME, // Window class name
                       TTT_WINTITLE,      // Window title
                       WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style
                       CW_USEDEFAULT, CW_USEDEFAULT,     // Position
                       800, 600,                         // Size (width, height)
                       NULL,                             // Parent window
                       NULL,                             // Menu
                       Instance,                         // Instance handle
                       NULL // Additional application data
        );
    if (!WindowHandle) return EXIT_SUCCESS;

    // Show & Update Window
    ShowWindow(WindowHandle, ShowCommand);
    UpdateWindow(WindowHandle);
    IsRunning = true;

    // 3. Main message loop
    MSG Message;
    while (IsRunning) {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }

    return EXIT_SUCCESS;
}

// Main Window's callback procedure
LRESULT CALLBACK MainWindowCallbackProcedure(HWND Window, UINT Message,
                                             WPARAM WParam, LPARAM LParam)
{
    switch (Message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(Window, &ps);
        RenderBackBuffer(Window, hdc);
        EndPaint(Window, &ps);
    } break;
    case WM_DESTROY: {
        IsRunning = 0;      // Stop the message loop
        PostQuitMessage(0); // Exit the application
    } break;

    // Handle window-close event
    case WM_CLOSE: {
        DestroyWindow(Window);              // Destroy the window
        Shell_NotifyIcon(NIM_DELETE, &nid); // remove tray icon
    } break;

    // Handle keyboard events
    case WM_KEYDOWN: {
        handle_keyboard_events(Window, Message, WParam, LParam);
    } break;

    // Handle tray icon events
    case WM_TRAYICON: {
        // Tray icon is clicked
        if (LParam == WM_LBUTTONDOWN || LParam == WM_RBUTTONDOWN) {
            ShowWindow(Window, SW_SHOW);
            ShowWindow(Window, SW_RESTORE);
            Shell_NotifyIcon(NIM_DELETE, &nid); // remove tray icon
        }
    } break;

    default: {
        // Handle default messages
        return DefWindowProc(Window, Message, WParam, LParam);
    } break;
    }
    return 0;
}

void handle_keyboard_events(HWND Window, UINT Message, WPARAM WParam,
                            LPARAM LParam)
{

    switch (WParam) {
    case VK_ESCAPE: {
        OutputDebugString("Escape Pressed\n");
    } break;
    case 'B': {
        ShowWindow(Window, SW_HIDE);
        AddTrayIcon(Window);
    } break;
    case 'Q': {
        DestroyWindow(Window);
    } break;
    }
}

void RenderBackBuffer(HWND Window, HDC hdc)
{
    // Create a memory DC
    HDC memDC = CreateCompatibleDC(hdc);

    // Create a bitmap for the back buffer
    HBITMAP hbmBackBuffer =
        CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Select the bitmap into the memory DC
    HBITMAP hbmOld = (HBITMAP)SelectObject(memDC, hbmBackBuffer);

    // Cear the back buffer (Fill with white)
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    FillRect(memDC, &rect, hBrush);
    DeleteObject(hBrush);

    // Draw a red rectangle on the back buffer
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    RECT rectRed = { 100, 100, 300, 300 };
    FillRect(memDC, &rectRed, hRedBrush);
    DeleteObject(hRedBrush);

    // Copy back buffer to the screen (Blitting)
    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);

    // Clean up
    SelectObject(memDC, hbmOld);
    DeleteObject(hbmBackBuffer);
    DeleteDC(memDC);
}
