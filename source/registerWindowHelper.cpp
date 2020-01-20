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

int registerWindow(TCHAR szClassName[],
                   HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   WNDCLASSEX& wincl,
                   LPCSTR HICON,
                   LPCSTR HICONSM,
                   LPCSTR HCURSOR,
                   void * winClass) {
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = (WNDPROC)winClass; /* This function is called by windows */
    wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, HICON);
    wincl.hIconSm = LoadIcon(NULL, HICONSM);
    wincl.hCursor = LoadCursor(NULL, HCURSOR);
    wincl.lpszMenuName = NULL; /* No menu */
    wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;
}

int registerWindowWithCustomIcon(TCHAR szClassName[],
                   HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   WNDCLASSEX& wincl,
                   int HICON,
                   int HICONSM,
                   LPCSTR HCURSOR,
                   void * winClass) {
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = (WNDPROC)winClass; /* This function is called by windows */
    wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(HICON));
    wincl.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(HICONSM));
    wincl.hCursor = LoadCursor(NULL, HCURSOR);
    wincl.lpszMenuName = NULL; /* No menu */
    wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;
}
