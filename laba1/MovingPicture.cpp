#include <stdio.h>
#include <tchar.h>
#include <winuser.h>
#include <winbase.h>

#include "Controller.h"

#define START_X 0
#define START_Y 0
#define HEIGHT 100
#define WIDTH 100
#define STEP 5

Controller sprite(START_X, START_Y, HEIGHT, WIDTH, STEP);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode) {
    HWND hWnd;
    MSG msg;
    WNDCLASS wc;
    wc.hInstance = This;
    wc.lpszClassName = WinName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        return 0;
    }

    hWnd = CreateWindow(
            WinName,
            _T("Moving picture"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            700,
            700,
            HWND_DESKTOP,
            NULL,
            This,
            NULL);

    ShowWindow(hWnd, mode);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdcMem;
    static HBRUSH hBrush;
    sprite.setHWND(hWnd);

    switch(message) {
        case WM_CREATE:
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            sprite.setBitmap((HBITMAP)LoadImage(NULL, (LPCSTR)"C:\\Study\\ksisbw-1.bmp", IMAGE_BITMAP, WIDTH, HEIGHT, LR_LOADFROMFILE));
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            sprite.setHDC(hdc);
            EndPaint(hWnd, &ps);
            break;

        case WM_KEYDOWN:
            sprite.changeRects(message, wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_MOUSEWHEEL:
            sprite.changeRects(message, wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
