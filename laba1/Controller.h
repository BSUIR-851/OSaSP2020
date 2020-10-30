#ifndef LABA1_CONTROLLER_H
#define LABA1_CONTROLLER_H

#include <winuser.h>

class Controller {
public:
    void setHDCWithSprite(HDC hdc);
    void setHDCWithBitmap(HDC hdc);
    void changeRects(UINT message, WPARAM wParam);
    void setHWND(HWND hWnd);
    void setBitmap(HBITMAP hBitmap);
    void setBrush(HBRUSH hbr);
    void (Controller::*handler)(HDC) = &Controller::setHDCWithSprite;
    Controller(int startX, int startY, int height, int width, int STEP);

private:
    HDC hdc;
    HWND hWnd;
    HBITMAP hBitmap;
    RECT clientRect;
    HBRUSH hbr;

    bool isPicture = false;


    int rectX;
    int rectY;
    int height;
    int width;
    int STEP;

    void changeRectsWithClientSpace();
};

#endif //LABA1_CONTROLLER_H
