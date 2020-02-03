#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <commctrl.h>

#include "../headers/board.hpp"
#include "../headers/resource.h"
#include "../headers/mainFunc.hpp"

void drawScene(HDC hdc, RECT *rect, HBITMAP hbmBackground, HBITMAP hbmBoardMask, HBITMAP hbmBoard)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBackground);
    BitBlt(hdcBuffer, 0, 0, 1000, 1024, hdcMem, 0, 0,
           SRCCOPY);

    (HBITMAP) SelectObject(hdcMem, hbmBoardMask);
    BitBlt(hdcBuffer, 0, 0, 1024, 1024, hdcMem,
           0, 0, SRCAND);

    (HBITMAP) SelectObject(hdcMem, hbmBoard);
    BitBlt(hdcBuffer, 0, 0, 1024, 1024, hdcMem,
           0, 0, SRCPAINT);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}

void drawSceneForStart(HDC hdc, RECT *rect, HBITMAP hbmStartBackground)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmStartBackground);
    BitBlt(hdcBuffer, 0, 0, 500, 700, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);
    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}
void drawSceneForMainResult(HDC hdc, RECT *rect, HBITMAP hbmBackground)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBackground);
    BitBlt(hdcBuffer, 0, 0, 600, 1021, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);
    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}