#include <cstdlib>
#include <windows.h>

#include <stdio.h>

// Window class name and title
#define TTT_WINCLASS_NAME "TicTacToe"
#define TTT_WINTITLE "TicTacToe"

// Global variables
BOOL IsRunning = false;

// Forward declarations
LRESULT CALLBACK MainWindowCallbackProcedure(HWND, UINT, WPARAM, LPARAM);

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
    case WM_DESTROY: {
        IsRunning = 0;      // Stop the message loop
        PostQuitMessage(0); // Exit the application
    } break;

    case WM_CLOSE: {
        DestroyWindow(Window); // Destroy the window
    } break;

    default: {
        // Handle default messages
        return DefWindowProc(Window, Message, WParam, LParam);
    } break;
    }
    return 0;
}
