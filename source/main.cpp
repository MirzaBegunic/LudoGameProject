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
#include <stdlib.h>
#include <ios>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <list>
#include <map>
#include "../headers/diceNumberGenerator.hpp"
#include "../headers/board.hpp"
#include "../headers/resource.h"
#include "../headers/mainFunc.hpp"
#include "../headers/gameutil.hpp"
#include "../headers/player.hpp"

using namespace std;

ofstream logg;


BITMAP bmpBoard;

HBITMAP hbmBackground;
HBITMAP hbmBoard;
HBITMAP hbmBoardMask;
HBITMAP hbmStartBackground;

/**
 * global variables 
 */

HWND mainGameHwnd, startmenuHwnd;


std::map<int, std::map<int, BoardField>> mapOfPlayerHomes;
std::map<int, BoardField> mapOfBoard;
std::vector<Player> players;
std::vector<Pawn> pOnePawns;
std::vector<Pawn> pTwoPawns;
std::vector<Pawn> pFourPawns;
std::vector<Pawn> pThreePawns;
std::list<BoardField> busyFields;

int currentPlayerIndedx = 1;
int currentDiceNumber = 1;
bool shouldPickPawn = false;

TCHAR szClassName[] = _T("LudoGameApp");
TCHAR startClassName[] = _T("Start");
void testMove(Player player, std::ofstream & logg);
int mCmdShow;


int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    logg.open("logs.txt");
    MSG messages;
    WNDCLASSEX wincl, winclStartMenu;
    mCmdShow = nCmdShow;

    if (registerWindow(szClassName, hThisInstance, hPrevInstance, lpszArgument, wincl, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedure) == 0)
    {
        return 0;
    }

    if (registerWindowWithCustomIcon(startClassName, hThisInstance, hPrevInstance, lpszArgument, winclStartMenu, IDI_ICON1, IDI_ICON1, IDC_ARROW, (void *)WindowProcedureStart) == 0)
    {
        return 0;
    }

    mainGameHwnd = CreateWindowEx(
        0,
        szClassName,
        _T("Ludo Game"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        1064,
        NULL,
        NULL,
        hThisInstance,
        NULL);


    startmenuHwnd = CreateWindowEx(
        0,
        startClassName,
        _T("Start Menu"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
        500,
        20,
        500,
        700,
        NULL,
        NULL,
        hThisInstance,
        NULL);
   // showStart();

    if (initialize() && showStart())
    {
        while (TRUE)
        {
            DWORD time_start;

            if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
            {
                if (messages.message == WM_QUIT)
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
            drawScene(hdc, &rect,hbmBackground, hbmBoardMask, hbmBoard);
            testPawn(hdc, players);
            ReleaseDC(mainGameHwnd, hdc);
            while ((GetTickCount() - time_start) < pause)
            {
                Sleep(5);
            }
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        logg.close();
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN: {
        if(shouldPickPawn) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (pawnSelection(logg,players.at(currentPlayerIndedx-1), x, y,mapOfPlayerHomes, mapOfBoard)) {
                if(calculatePlayerMove(players.at(currentPlayerIndedx-1), mapOfBoard, logg)){
                    shouldPickPawn = false;
                    if(checkIfPawnsShouldBeEaten(players, players.at(currentPlayerIndedx-1), mapOfPlayerHomes))
                        MessageBox(mainGameHwnd, "This Guy FUCKS","Ouuu", MB_OKCANCEL);
                    if (currentDiceNumber != 6) {
                        nextPlayer();
                    }
                } else {
                    MessageBox(mainGameHwnd, "Pijun se ne moze pomjeriti","Error", MB_OKCANCEL);
                }
            } else {
                MessageBox(mainGameHwnd, "Los odabir","Fail", MB_OKCANCEL);

            }
        } else {
            MessageBox(mainGameHwnd, "Prvo bacit kockicu","Fail", MB_OKCANCEL);
        }
        break;
    }    

    case WM_RBUTTONDOWN: {
        if (!shouldPickPawn) {
            currentDiceNumber = rollTheDice(players.at(currentPlayerIndedx-1));
            vector<Pawn> pawns = players.at(currentPlayerIndedx-1).pawns;
            bool rollAgain = true;
            if(currentDiceNumber == 6) {
                rollAgain = false;
            }
            for (Pawn pawn : pawns) {
                if(pawn.currentPosition != 0 && !pawn.isFinished) rollAgain = false;
            }
            if (!rollAgain) {
                shouldPickPawn = true;
                char buf[5];
                sprintf(buf, "Bacena je %d,Odaberi pijuna", currentDiceNumber);
                MessageBox(mainGameHwnd,buf,"Kockica",MB_OK);
                logg<<"\n";
            } else {
                MessageBox(mainGameHwnd,"Upps! nisi bacio sest, baci ponovno","Kockica",MB_OK);
                updatePoints(players.at(currentPlayerIndedx-1),-10); 
            }
        }         
        break;
    }
    case WM_KEYDOWN: {
            switch (wParam) 
            { 
                case VK_LEFT: 
                    shouldPickPawn = false;
                    updatePoints(players.at(currentPlayerIndedx-1),-10); 
                    nextPlayer();
                    break; 
            }
            break;
    }

    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedureStart(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) /* handle the messages */
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
        switch (LOWORD(wParam))
        {
        case ID_START:
        {
            ShowWindow(mainGameHwnd, mCmdShow);
            CloseWindow(startmenuHwnd);
            break;
        }
        case ID_EXIT:
        {
            DestroyWindow(startmenuHwnd);
            PostQuitMessage(0);
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        logg.close();
        //PostQuitMessage(0); 
        break;
    default: 
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

void loadResources()
{
    hbmBackground = (HBITMAP)LoadImage(NULL, "resources/background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoard = (HBITMAP)LoadImage(NULL, "resources/ludoboard_black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoardMask = (HBITMAP)LoadImage(NULL, "resources/ludoboard_white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmStartBackground = (HBITMAP)LoadImage(NULL, "resources/startbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

bool initialize()
{
    loadResources();
    mapOfPlayerHomes = generatePlayerHomes(busyFields);
    mapOfBoard = generateBoard();
    players = loadGame(pOnePawns, pTwoPawns, pThreePawns, pFourPawns, mapOfPlayerHomes);

    return true;
}

bool showStart() {
    HDC hdcStart = GetDC(startmenuHwnd);
    RECT rect1;
    GetClientRect(startmenuHwnd, &rect1);
    drawSceneForStart(hdcStart, &rect1, hbmStartBackground);
    ReleaseDC(startmenuHwnd, hdcStart);
    ShowWindow(startmenuHwnd, mCmdShow);
}

void nextPlayer() {
    if(currentPlayerIndedx == 4) {
        currentPlayerIndedx = 1;
    } else {
        currentPlayerIndedx += 1;
    }
}