#include "Controller.h"

Controller::Controller(int startX, int startY, int height, int width, int STEP) {
    this->rectX = startX;
    this->rectY = startY;
    this->height = height;
    this->width = width;
    this->STEP = STEP;
}

void Controller::setHDC(HDC hdc) {
    this->hdc = hdc;
    this->changeRectsWithClientSpace();
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    HDC hdcMem = CreateCompatibleDC(this->hdc);
    oldBitmap = SelectObject(hdcMem, this->hBitmap);
    GetObject(this->hBitmap, sizeof(bitmap), &bitmap);
    BitBlt(this->hdc, this->rectX, this->rectY, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
}

void Controller::changeRects(UINT message, WPARAM wParam) {
    switch (message) {
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_UP:
                    this->rectY -= this->STEP;
                    break;
                case VK_DOWN:
                    this->rectY += this->STEP;
                    break;
                case VK_RIGHT:
                    this->rectX += this->STEP;
                    break;
                case VK_LEFT:
                    this->rectX -= this->STEP;
                    break;
            }
            break;
        case WM_MOUSEWHEEL:
            switch (LOWORD(wParam)) {
                case MK_SHIFT:
                    if ((short)HIWORD(wParam) < 0) {
                        this->rectX += this->STEP;
                    } else {
                        this->rectX -= this->STEP;
                    }
                    break;

                default:
                    if ((short)HIWORD(wParam) < 0) {
                        this->rectY += this->STEP;
                    } else {
                        this->rectY -= this->STEP;
                    }
                    break;
            }
            break;
    }
}

void Controller::setHWND(HWND hWnd) {
    this->hWnd = hWnd;
    GetClientRect(this->hWnd, &this->rect);
}

void Controller::setBitmap(HBITMAP hBitmap) {
    this->hBitmap = hBitmap;
}

void Controller::changeRectsWithClientSpace() {
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
}

