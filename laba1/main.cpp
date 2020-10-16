#include <stdio.h>
#include <tchar.h>
#include <winuser.h>
#include <winbase.h>

#define START_X 0
#define START_Y 0
#define HEIGHT 100
#define WIDTH 100
#define STEP 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

class Controller {
public:
    HDC hdc;
    void setHDC(HDC hdc) {
        this->hdc = hdc;
        int endX = this->rectX + this->width;
        int endY = this->rectY + this->height;
        GetClientRect(this->hWnd, &this->rect);
        if (this->rectX <= this->rect.left)
            this->rectX += this->width/2;

        if (this->rectY <= this->rect.top)
            this->rectY += this->height/2;

        if (endX >= this->rect.right)
            this->rectX -= this->width/2;

        if (endY >= this->rect.bottom)
            this->rectY -= this->height/2;

        BITMAP bitmap;
        HGDIOBJ oldBitmap;
        HDC hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, this->hBitmap);
        GetObject(this->hBitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, this->rectX, this->rectY, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

    };

    void changeRectsWithClientSpace(UINT message, WPARAM wParam) {
        switch (message) {
            case WM_KEYDOWN:
                switch (wParam) {
                    case VK_UP:
                        this->rectY -= STEP;
                        break;
                    case VK_DOWN:
                        this->rectY += STEP;
                        break;
                    case VK_RIGHT:
                        this->rectX += STEP;
                        break;
                    case VK_LEFT:
                        this->rectX -= STEP;
                        break;
                }
                break;
            case WM_MOUSEWHEEL:
                switch (LOWORD(wParam)) {
                    case MK_SHIFT:
                        if ((short)HIWORD(wParam) < 0) {
                            this->rectX += STEP;
                        } else {
                            this->rectX -= STEP;
                        }
                        break;

                    default:
                        if ((short)HIWORD(wParam) < 0) {
                            this->rectY += STEP;
                        } else {
                            this->rectY -= STEP;
                        }
                        break;
                }
                break;
        }
    };

    void setHWND(HWND hWnd) {
        this->hWnd = hWnd;
        GetClientRect(this->hWnd, &this->rect);
    }

    void setBitmap(HBITMAP hBitmap) {
        this->hBitmap = hBitmap;
    }

    Controller(int startX, int startY, int height, int width) {
        this->rectX = startX;
        this->rectY = startY;
        this->height = height;
        this->width = width;
}

private:
    HWND hWnd;
    HBITMAP hBitmap;
    int rectX;
    int rectY;
    int height;
    int width;
    RECT rect;
};

Controller sprite(START_X, START_Y, HEIGHT, WIDTH);

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
            _T("Test"),
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
            sprite.changeRectsWithClientSpace(message, wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_MOUSEWHEEL:
            sprite.changeRectsWithClientSpace(message, wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
