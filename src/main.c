/*
  TIC TAC TOE
  Author: smpl
  Date: 02-25-2025
  Dependencies: 0
  Features: Multiplayer over lan
*/

#include <stdlib.h>

#include <WinSock2.h>
#include <corecrt_wstdlib.h>
#include <float.h>
#include <winbase.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

//-----------------------------------------------------------------------
// GAME CONSTANTS
//-----------------------------------------------------------------------
#define TTT_WINTITLE "TicTacToe"
#define TTT_WINCLASS_NAME "TicTacToe"
#define FPS 60
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

//-------------------------------------------------------------------
// GLOBAL VARIABLES
//-------------------------------------------------------------------
unsigned char IsRunning = 0; // 0=False, 1=True
HINSTANCE global_hInstance;

//-------------------------------------------------------------------
// FORWARD DECLARATIONS
//-------------------------------------------------------------------
LRESULT CALLBACK MainWindowCallbackProcedure(HWND, UINT, WPARAM, LPARAM);
BOOL InitializeWindowInstance(HINSTANCE, int);

//-------------------------------------------------------------------
// APP ENTRY POINT
//-------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance,
                      _In_ LPWSTR CommandLine, _In_ int ShowCommand)
{
    // 1. Register the Window Class
    WNDCLASS WindowClass = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = MainWindowCallbackProcedure,
        .hInstance = Instance,
        .lpszClassName = TTT_WINCLASS_NAME,
    };

    // 2. Initialize the Main Window Instance
    if (!InitializeWindowInstance(Instance, ShowCommand)) return FALSE;
    IsRunning = 1;

    // 3. Main message loop
    // (WindowProc fn will handle these disptched messages)
    MSG Message;
    HACCEL hAccelTable = LoadAccelerators(Instance, MAKEINTRESOURCE(109));
    OutputDebugStringA("hello once\n");
    while (IsRunning) {
        BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
        if (MessageResult > 0) {
            TranslateMessage(&Message); // basically converts messages into
                                        // proper keyboard messages
            DispatchMessage(&Message);
        } else {
            break;
        }
    }

    return EXIT_SUCCESS;
}

// 2.1: Saves instance handle and creates main window
BOOL InitializeWindowInstance(HINSTANCE Instance, int ShowCommand)
{
    global_hInstance = Instance; // store in global variable

    // Create a main window, and get the handle to this window
    HWND WindowHandle = CreateWindowEx(
        0, TTT_WINCLASS_NAME, TTT_WINTITLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
        Instance, 0);

    // FAILED TO CREATE THE MAIN WINDOW
    // will fail if window class name is not same as Instances classname
    if (!WindowHandle) return FALSE;

    return TRUE;
}

void do_paint(HWND *);
// 1.2 (requires gdi32.lib at linking)
LRESULT CALLBACK MainWindowCallbackProcedure(HWND Window, UINT Message,
                                             WPARAM WParam, LPARAM LParam)
{
    LRESULT result = 0;
    // Handle all the messages that Windows Sends to the window
    switch (Message) {
    case WM_SIZE: {
        // Get the new size
        RECT ClientRect;
        GetClientRect(Window, &ClientRect);
        int width = ClientRect.right - ClientRect.left;
        int height = ClientRect.top - ClientRect.bottom;

        // ResizeDBISection(width, height);
        OutputDebugStringA("WM_SIZE");
    } break;
    case WM_DESTROY: { // this is force close
        IsRunning = 0; // TODO: Handle this with a message to the user?
                       // OutputDebugStringA("WM_DESTROY");
    } break;
    case WM_CLOSE: {
        IsRunning = 0; // TODO: Handle this as an error, recreate the window
                       // OutputDebugStringA("WM_CLOSE");
                       //  DestroyWindow(Window);
                       //  exit(0);
    } break;
    case WM_ACTIVATEAPP: {
        OutputDebugStringA("WM_ACTIVATEAPP");
    } break;
    case WM_PAINT: {
        do_paint(&Window);
    } break;
    default: {
        // Other unhandled Messages will be passed to WindowsOS to handle
        result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
    };

    return result;
};

void do_paint(HWND *Wind)
{
    HWND Window = *Wind;
    PAINTSTRUCT Paint;
    HDC DeviceContext = BeginPaint(Window, &Paint);
    int X = Paint.rcPaint.left;
    int Y = Paint.rcPaint.top;
    LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
    LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

    static DWORD Operation = WHITENESS;
    PatBlt(DeviceContext, X, Y, Width, Height, Operation);
    Operation = Operation == WHITENESS ? BLACKNESS : WHITENESS;

    /*
    HBRUSH hPurpleBrush = CreateSolidBrush(0x00800080L);  // L for DWORD
    HGDIOBJ holdBrush = SelectObject(DeviceContext, hPurpleBrush);
    PatBlt(DeviceContext, X, Y, Width, Height, PATCOPY);
    SelectObject(DeviceContext, holdBrush);  // restore old brush
    DeleteObject(hPurpleBrush);              // clean up purple brush
    */

    EndPaint(Window, &Paint);
}
