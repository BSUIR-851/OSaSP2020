#ifndef LABA1_CONTROLLER_H
#define LABA1_CONTROLLER_H

#include <winuser.h>

class Controller {
public:
    HDC hdc;

    void setHDC(HDC hdc);
    void changeRects(UINT message, WPARAM wParam);
    void setHWND(HWND hWnd);
    void setBitmap(HBITMAP hBitmap);
    Controller(int startX, int startY, int height, int width, int STEP);

private:
    HWND hWnd;
    HBITMAP hBitmap;
    RECT rect;

    int rectX;
    int rectY;
    int height;
    int width;
    int STEP;

    bool isPicture;

    void changeRectsWithClientSpace();
};

#endif //LABA1_CONTROLLER_H
