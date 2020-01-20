#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define Pressed(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define pause 100


#include <tchar.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <commctrl.h>

#include "headers/board.hpp"
#include "headers/resource.h"

using namespace std;

BITMAP bmpBoard;

HBITMAP hbmBackground;
HBITMAP hbmBoard;
HBITMAP hbmBoardMask;
HBITMAP hbmStartBackground;
void loadResources();
void drawScene(HDC, RECT*);
void drawSceneForStart(HDC, RECT*);
void drawBackground(HDC, RECT*);

bool initialize();
void CheckInput(RECT * rect);
void test(HDC hdc);

/**
 * global variables 
 */

HWND mainGameHwnd,startmenuHwnd;

ofstream logg;

std::map<int,std::map<int,board_field>> mapOfPlayerHomes;
std::map<int,board_field> mapOfBoard;

INT_PTR CALLBACK DlgStart(HWND hdlg, UINT message,WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcedureStart (HWND, UINT, WPARAM, LPARAM);

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("LudoGameApp");
TCHAR startClassName[ ] = _T("Start");
int mCmdShow;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl,winc2;        /* Data structure for the windowclass */
    mCmdShow = nCmdShow;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;               /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    winc2.hInstance = hThisInstance;
    winc2.lpszClassName = startClassName;
    winc2.lpfnWndProc = WindowProcedureStart;      /* This function is called by windows */
    winc2.style = CS_DBLCLKS;               /* Catch double-clicks */
    winc2.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    winc2.hIcon = LoadIcon (NULL, MAKEINTRESOURCE(IDI_ICON1));
    winc2.hIconSm = LoadIcon (NULL,  MAKEINTRESOURCE(IDI_ICON1));
    winc2.hCursor = LoadCursor (NULL, IDC_ARROW);
    winc2.lpszMenuName = NULL;                 /* No menu */
    winc2.cbClsExtra = 0;                       /* No extra bytes after the window class */
    winc2.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    winc2.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    winc2.lpfnWndProc = WindowProcedureStart;
    winc2.lpszClassName="Start";

    if(!RegisterClassEx(&winc2))
        return 0;

    /* The class is registered, let's create the program*/
    mainGameHwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Ludo Game"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1280,                 /* The programs width */
           1064,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    startmenuHwnd= CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           "Start",         /* Classname */
           _T("Start Menu"),       /* Title Text */
           DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP |WS_SYSMENU, /* default window */
           500,       /* Windows decides the position */
            20,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           700,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    /* Make the window visible on the screen */
   // ShowWindow (startmenuHwnd, nCmdShow);
    logg.open ("logs.txt");


    if(initialize()) {
        while(TRUE)
        {
            DWORD time_start;

            if(PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
            {
                if(messages.message == WM_QUIT)
                {
                    break;
                }
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
            time_start = GetTickCount();
            HDC hdc = GetDC(mainGameHwnd);
            RECT rect;
            GetClientRect(mainGameHwnd, &rect);
            drawScene(hdc, &rect);
            ReleaseDC(mainGameHwnd, hdc);
            //GetClientRect(hwnd, &rect);
            CheckInput(&rect);
            //Update(hwnd);
            //Render(hwnd);
            while((GetTickCount() - time_start) < pause)
            {
                Sleep(5);
            }
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

    switch (message)                  /* handle the messages */
    {   case WM_CREATE:
            break;
        case WM_DESTROY:
            logg.close();
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



LRESULT CALLBACK WindowProcedureStart (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{     
      switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        CreateWindowA("Button", "START", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
        110, 320, 250, 60, hwnd, (HMENU)ID_START, hinst, NULL);
        CreateWindowA("Button", "EXIT", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
        110, 390, 250, 60, hwnd, (HMENU)ID_EXIT, hinst, NULL);
        break;
        }
         case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case ID_START:
        {    ShowWindow (mainGameHwnd, mCmdShow);
             CloseWindow(hwnd);
            break;
        }
        case ID_EXIT:
        {
            PostQuitMessage (0);
            break;
        }
        }
        break;
    }
        case WM_DESTROY:
            logg.close();
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;


}
void drawScene(HDC hdc, RECT* rect) {
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, hbmBackground);
    BitBlt(hdcBuffer, 0, 0, 1280, 1024, hdcMem, 0, 0,
           SRCCOPY);

    (HBITMAP) SelectObject(hdcMem, hbmBoardMask);
    BitBlt(hdcBuffer, 0, 0, 1024, 1024, hdcMem,
           0,0, SRCAND);

    (HBITMAP) SelectObject(hdcMem, hbmBoard);
    BitBlt(hdcBuffer, 0, 0, 1024, 1024, hdcMem,
           0,0, SRCPAINT);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    test(hdc);

    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);


}

void drawBackground(HDC hdc, RECT* rect) {
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, hbmBackground);
    BitBlt(hdcBuffer, 0, 0, 1280, 1024, hdcMem, 0, 0,
           SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);

}

void drawSceneForStart(HDC hdc, RECT* rect) {
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect->right, rect->bottom);
    HBITMAP oldHbmBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld =  (HBITMAP) SelectObject(hdcMem, hbmStartBackground);
    BitBlt(hdcBuffer, 0, 0, 500, 700, hdcMem, 0, 0,SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);
    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}
void loadResources() {
    hbmBackground = (HBITMAP)LoadImage(NULL, "resources/background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoard = (HBITMAP)LoadImage(NULL, "resources/ludoboard_black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoardMask = (HBITMAP)LoadImage(NULL, "resources/ludoboard_white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmStartBackground = (HBITMAP)LoadImage(NULL, "resources/startbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

bool initialize() {
    loadResources();
    HDC hdcStart=GetDC(startmenuHwnd);
    RECT rect1;
    GetClientRect(startmenuHwnd,&rect1);
    drawSceneForStart(hdcStart,&rect1);
    ReleaseDC(startmenuHwnd,hdcStart);
    ShowWindow (startmenuHwnd, mCmdShow);
    mapOfPlayerHomes = generatePlayerHomes();
    mapOfBoard = generateBoard();
    return true;
}

void CheckInput(RECT * rect){
    if(Pressed(VK_LBUTTON)) {
        POINT p;
        GetCursorPos(&p);
        logg<<p.x<<":"<<p.y<<"\n";
    }
}

void test(HDC hdc) {
    for (int i=1; i<=4; ++i) {
        map<int, board_field> tmp = mapOfPlayerHomes[i];

        for(int j=1; j<=4; ++j) {
            board_field bf = tmp[j];
            Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
        }
    }
    for(int i=0; i<=81; ++i) {
        board_field bf = mapOfBoard[i];
        Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
    }
}