#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define pause 10

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
#include <algorithm>
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
HBITMAP hbmEnterNamesBackground;
HBITMAP hbmGameMenuBackground;
HBITMAP hbmMainResultBackground;
HBITMAP hbmFinalResultBackground;

HBITMAP hbmGreenPawn;
HBITMAP hbmGreenPawnMask;
HBITMAP hbmRedPawn;
HBITMAP hbmRedPawnMask;
HBITMAP hbmYellowPawn;
HBITMAP hbmYellowPawnMask;
HBITMAP hbmBluePawn;
HBITMAP hbmBluewPawnMask;

HBITMAP hbmGreenPawnSmall;
HBITMAP hbmGreenPawnSmallMask;
HBITMAP hbmRedPawnSmall;
HBITMAP hbmRedPawnSmallMask;
HBITMAP hbmYellowPawnSmall;
HBITMAP hbmYellowPawnSmallMask;
HBITMAP hbmBluePawnSmall;
HBITMAP hbmBluewPawnSmallMask;

HBITMAP hbmDiceMask;
HBITMAP hbmDice;

COLORREF blue = RGB(13, 71, 161);
COLORREF red = RGB(183, 28, 28);
COLORREF green = RGB(27, 94, 32);
COLORREF yellow = RGB(253, 216, 53);

HWND mainGameHwnd, startmenuHwnd, enterNamesHwnd, gameMenuHwnd, mainResultHwnd, finalResultHwnd;
HWND p1Result, p2Result, p3Result, p4Result;
HWND p1, p1Points, p1Pawns, p2, p2Points, p2Pawns, p3, p3Points, p3Pawns, p4, p4Points, p4Pawns;
HWND muteButton;
std::map<int, std::map<int, BoardField>> mapOfPlayerHomes;
std::map<int, BoardField> mapOfBoard;
std::vector<Player> players;
std::vector<Pawn> pOnePawns;
std::vector<Pawn> pTwoPawns;
std::vector<Pawn> pFourPawns;
std::vector<Pawn> pThreePawns;
std::map<int, std::vector<unsigned int>> busyFields;
std::vector<bool> playersDone;
std::map<int, BoardField> mapOfDices;

char Player1[40];
char Player2[40];
char Player3[40];
char Player4[40];
int currentPlayerIndedx = 1;
int currentDiceNumber = 1;
bool shouldPickPawn = false;
bool diceIsRolling = false;
int diceAnimPos = 0;
int diceAnim = 0;
bool muted = false;

TCHAR szClassName[] = _T("LudoGameApp");
TCHAR startClassName[] = _T("Start");
TCHAR enterNameClassName[] = _T("Enter Names");
TCHAR gameMenuClassName[] = _T("Game Menu");
TCHAR mainResultClassName[] = _T("Main Result");
TCHAR FinalResultClassName[] = _T("Final Result");

int mCmdShow;

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    logg.open("logs.txt");
    MSG messages;
    WNDCLASSEX wincl, winclStartMenu, winclEnterNames, winclGameMenu, winclMainResult, winclFinalResult;
    mCmdShow = nCmdShow;

    if (registerWindow(szClassName, hThisInstance, hPrevInstance, lpszArgument, wincl, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedure) == 0)
    {
        return 0;
    }

    if (registerWindow(FinalResultClassName, hThisInstance, hPrevInstance, lpszArgument, winclFinalResult, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedureFinalResult) == 0)
    {
        return 0;
    }

    if (registerWindowWithCustomIcon(startClassName, hThisInstance, hPrevInstance, lpszArgument, winclStartMenu, IDI_ICON1, IDI_ICON1, IDC_ARROW, (void *)WindowProcedureStart) == 0)
    {
        return 0;
    }
    if (registerWindow(gameMenuClassName, hThisInstance, hPrevInstance, lpszArgument, winclGameMenu, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedureGameMenu) == 0)
    {
        return 0;
    }

    if (registerWindow(mainResultClassName, hThisInstance, hPrevInstance, lpszArgument, winclMainResult, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedureMainResult) == 0)
    {
        return 0;
    }

    if (registerWindow(enterNameClassName, hThisInstance, hPrevInstance, lpszArgument, winclEnterNames, IDI_APPLICATION, IDI_APPLICATION, IDC_ARROW, (void *)WindowProcedureEnterNames) == 0)
    {
        return 0;
    }

    mainGameHwnd = CreateWindowEx(
        0,
        szClassName,
        _T("Ludo Game"),
        WS_OVERLAPPED | WS_MINIMIZEBOX,
        100,
        10,
        1000,
        1024,
        HWND_DESKTOP,
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
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL);

    enterNamesHwnd = CreateWindowEx(
        0,
        enterNameClassName,
        _T("Enter Player Names"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
        500,
        20,
        500,
        700,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL);

    finalResultHwnd = CreateWindowEx(
        0,
        FinalResultClassName,
        _T("Final Result"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP,
        500,
        20,
        400,
        950,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL);

    ShowWindow(enterNamesHwnd, SW_HIDE);
    ShowWindow(gameMenuHwnd, SW_HIDE);
    ShowWindow(finalResultHwnd, SW_HIDE);

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
            drawScene(hdc, &rect);
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

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_DESTROY:
        logg.close();
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:
    {
        if (shouldPickPawn)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (pawnSelection(logg, players.at(currentPlayerIndedx - 1), x, y, mapOfPlayerHomes, mapOfBoard))
            {
                if (calculatePlayerMove(players.at(currentPlayerIndedx - 1), mapOfBoard, logg, p1Result, p2Result, p3Result, p4Result, playersDone, busyFields))
                {
                    shouldPickPawn = false;
                    if (checkIfPawnsShouldBeEaten(players, players.at(currentPlayerIndedx - 1), mapOfPlayerHomes, p1Result, p2Result, p3Result, p4Result, playersDone, busyFields))
                    {
                        if (!muted)
                            PlaySound("resources/PawnEaten.wav", NULL, SND_FILENAME | SND_ASYNC);
                        MessageBox(mainGameHwnd, "+200 Points", "Woaaah", MB_OK);
                    }
                    if (currentDiceNumber != 6)
                    {
                        nextPlayer();
                    }
                    bool finish = true;
                    for (bool isFinish : playersDone)
                    {
                        if (!isFinish)
                        {
                            finish = false;
                        }
                    }
                    if (finish)
                    {
                        endGame();
                    }
                }
                else
                {
                    MessageBox(mainGameHwnd, "You need to skip this round", "Skip It", MB_OK);
                }
            }
            else
            {
                if (!muted)
                    PlaySound("resources/WrongPick.wav", NULL, SND_FILENAME | SND_ASYNC);
                MessageBox(mainGameHwnd, "Can't touch this", "Nope", MB_OK);
            }
        }
        else
        {
            MessageBox(mainGameHwnd, "You need to rool the dice, or skip... Your call", "Hold on", MB_OK);
        }
        break;
    }

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            endGame();
            break;

        case VK_F6:
            diceRoller(6);
            break;

        case VK_F5:
            diceRoller(5);
            break;

        case VK_F4:
            diceRoller(4);
            break;

        case VK_F3:
            diceRoller(3);
            break;

        case VK_F2:
            diceRoller(2);
            break;

        case VK_F1:
            diceRoller(1);
            break;

        default:
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
    HDC hdc;
    PAINTSTRUCT ps;

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

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForStart(hdc, &rect, hbmStartBackground);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_START:

        {
            ShowWindow(enterNamesHwnd, SW_SHOW);
            showEnterNames();
            ShowWindow(hwnd, SW_HIDE);

            break;
        }
        case ID_EXIT:
        {
            PostQuitMessage(0);
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        logg.close();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedureEnterNames(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH hbrushEditBox = CreateSolidBrush(RGB(159, 168, 218));

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        HWND button = CreateWindowA("Button", "CONTINUE", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                                    130, 570, 300, 60, hwnd, (HMENU)ID_CONTINUE, hinst, NULL);
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 130, 190, 300, 30, hwnd, (HMENU)ID_PLAYER1, hinst, NULL);
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 130, 290, 300, 30, hwnd, (HMENU)ID_PLAYER2, hinst, NULL);
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 130, 390, 300, 30, hwnd, (HMENU)ID_PLAYER3, hinst, NULL);
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 130, 490, 300, 30, hwnd, (HMENU)ID_PLAYER4, hinst, NULL);
        break;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForStart(hdc, &rect, hbmEnterNamesBackground);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CTLCOLOREDIT:
    {
        if (GetDlgCtrlID((HWND)lParam) == ID_PLAYER1)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, yellow);
            SetBkColor(hdcEdit, RGB(159, 168, 218));
            return (LONG)hbrushEditBox;
        }
        if (GetDlgCtrlID((HWND)lParam) == ID_PLAYER2)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, blue);
            SetBkColor(hdcEdit, RGB(159, 168, 218));
            return (LONG)hbrushEditBox;
        }
        if (GetDlgCtrlID((HWND)lParam) == ID_PLAYER3)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, red);
            SetBkColor(hdcEdit, RGB(159, 168, 218));
            return (LONG)hbrushEditBox;
        }
        if (GetDlgCtrlID((HWND)lParam) == ID_PLAYER4)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, green);
            SetBkColor(hdcEdit, RGB(159, 168, 218));
            return (LONG)hbrushEditBox;
        }

        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_PLAYER1:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                GetWindowText((HWND)lParam, Player1, 40);
                players.at(0).playerName = Player1;
            }
            break;

        case ID_PLAYER2:

            if (HIWORD(wParam) == EN_CHANGE)
            {
                GetWindowText((HWND)lParam, Player2, 40);
                players.at(1).playerName = Player2;
            }
            break;
        case ID_PLAYER3:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                GetWindowText((HWND)lParam, Player3, 40);
                players.at(2).playerName = Player3;
            }
            break;
        case ID_PLAYER4:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                GetWindowText((HWND)lParam, Player4, 40);
                players.at(3).playerName = Player4;
            }
            break;
        case ID_CONTINUE:

        {
            HWND p1 = GetDlgItem(hwnd, ID_PLAYER1);
            HWND p2 = GetDlgItem(hwnd, ID_PLAYER2);
            HWND p3 = GetDlgItem(hwnd, ID_PLAYER3);
            HWND p4 = GetDlgItem(hwnd, ID_PLAYER4);
            int length1 = (int)SendMessage(p1, WM_GETTEXTLENGTH, 0, 0);
            int length2 = (int)SendMessage(p2, WM_GETTEXTLENGTH, 0, 0);
            int length3 = (int)SendMessage(p3, WM_GETTEXTLENGTH, 0, 0);
            int length4 = (int)SendMessage(p4, WM_GETTEXTLENGTH, 0, 0);
            if (length1 != 0 && length2 != 0 && length3 != 0 && length4 != 0)
            {
                ShowWindow(mainGameHwnd, mCmdShow);
                createMainResult();
                ShowWindow(mainResultHwnd, SW_SHOW);
                showMainResult();
                ShowWindow(hwnd, SW_HIDE);
                break;
            }
            else
            {
                MessageBox(0, "Please enter all players", 0, 0);
                break;
            }
        }
        }
        return 0;
    }
    case WM_DESTROY:
        logg.close();
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK WindowProcedureGameMenu(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        if (muted)
        {
            muteButton = CreateWindowA("Button", "UNMUTE", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                                       140, 270, 300, 60, hwnd, (HMENU)ID_MUTE, hinst, NULL);
        }
        else
        {
            muteButton = CreateWindowA("Button", "MUTE", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                                       140, 270, 300, 60, hwnd, (HMENU)ID_MUTE, hinst, NULL);
        }

        CreateWindowA("Button", "RESTART", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      140, 380, 300, 60, hwnd, (HMENU)ID_RESTART, hinst, NULL);
        CreateWindowA("Button", "EXIT", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      140, 490, 300, 60, hwnd, (HMENU)ID_EXIT1, hinst, NULL);
        break;
    }

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForStart(hdc, &rect, hbmGameMenuBackground);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_RESTART:
        { //Restart igre
            reinitialize();
            break;
        }
        case ID_MUTE:
        {
            if (muted)
            {
                SendMessage(muteButton, WM_SETTEXT, 0, (LPARAM) _T("MUTE"));
                muted = false;
            }
            else
            {
                SendMessage(muteButton, WM_SETTEXT, 0, (LPARAM) _T("UNMUTE"));
                muted = true;
            }

            break;
        }
        case ID_EXIT1:
        {
            PostQuitMessage(0);
            break;
        }
        }
        return 0;
    }
    case WM_DESTROY:
        logg.close();
        ShowWindow(gameMenuHwnd, SW_HIDE);
        break;
    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK WindowProcedureMainResult(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HBRUSH defaultbrush = NULL;
    static HBRUSH hBrushR = CreateSolidBrush(RGB(239, 83, 80));
    static HBRUSH hBrushY = CreateSolidBrush(RGB(255, 241, 118));
    static HBRUSH hBrushG = CreateSolidBrush(RGB(129, 199, 132));
    static HBRUSH hBrushB = CreateSolidBrush(RGB(100, 181, 246));

    switch (message) /* handle the messages */
    {

    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        HWND p1 = CreateWindow("static", Player1, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 140, 310, 230, 30, hwnd, (HMENU)ID_P1, hinst, NULL);
        p1Result = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 450, 310, 80, 30, hwnd, (HMENU)ID_P1RESULT, hinst, NULL);
        HWND p2 = CreateWindow("static", Player2, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 140, 410, 230, 30, hwnd, (HMENU)ID_P2, hinst, NULL);
        p2Result = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 450, 410, 80, 30, hwnd, (HMENU)ID_P2RESULT, hinst, NULL);
        HWND p3 = CreateWindow("static", Player3, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 140, 510, 230, 30, hwnd, (HMENU)ID_P3, hinst, NULL);
        p3Result = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 450, 510, 80, 30, hwnd, (HMENU)ID_P3RESULT, hinst, NULL);
        HWND p4 = CreateWindow("static", Player4, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 140, 610, 230, 30, hwnd, (HMENU)ID_P4, hinst, NULL);
        p4Result = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 450, 610, 80, 30, hwnd, (HMENU)ID_P4RESULT, hinst, NULL);
        CreateWindow("button", "ROLL DICE", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE, 140, 700, 230, 80, hwnd, (HMENU)ID_DICE, hinst, NULL);
        CreateWindow("button", "SKIP", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE, 140, 800, 230, 80, hwnd, (HMENU)ID_SKIP, hinst, NULL);
        CreateWindow("button", "OPEN MENU", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE, 140, 900, 230, 80, hwnd, (HMENU)ID_GAMEMENU, hinst, NULL);
        blue = COLORREF(RGB(0, 0, 255));

        HFONT hf;
        HDC hdc;
        long lfHeight;
        hdc = GetDC(NULL);
        lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        ReleaseDC(NULL, hdc);

        hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
        SendMessage(p1, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p2, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p3, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p4, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p1Result, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p2Result, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p3Result, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p4Result, WM_SETFONT, (WPARAM)hf, TRUE);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        DWORD CtrlID = GetDlgCtrlID((HWND)lParam);
        if (CtrlID == ID_P1 || CtrlID == ID_P1RESULT) //If desired control
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(255, 241, 118));
            return (INT_PTR)hBrushY;
        }
        else if (CtrlID == ID_P2 || CtrlID == ID_P2RESULT)
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(100, 181, 246));
            return (INT_PTR)hBrushB;
        }
        else if (CtrlID == ID_P3 || CtrlID == ID_P3RESULT)
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(239, 83, 80));
            return (INT_PTR)hBrushR;
        }
        else if (CtrlID == ID_P4 || CtrlID == ID_P4RESULT)
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(129, 199, 132));
            return (INT_PTR)hBrushG;
        }
        break;
    }

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForMainResult(hdc, &rect, hbmMainResultBackground);
        EndPaint(hwnd, &ps);
        break;
    }
        /* case WM_CTLCOLOREDIT:
    { 
      if(GetWindowLong ((HWND) lParam,GWL_ID)  == ID_P1){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, blue); 
        return (LONG)hbrushEditBox; 
        } 
        if(GetDlgCtrlID((HWND)lParam) == ID_P2){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, green); 
        return (LONG)hbrushEditBox; 
        }
        if(GetDlgCtrlID((HWND)lParam) == ID_P3){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, red); 
        return (LONG)hbrushEditBox; 
        }
        if(GetDlgCtrlID((HWND)lParam) == ID_P4){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, yellow); 
        return (LONG)hbrushEditBox; 
        }

        break;
    }
    */
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_DICE:
        {
            diceRoller(0);
            break;
        }
        case ID_GAMEMENU:
        {
            createGameMenu();
            ShowWindow(gameMenuHwnd, SW_SHOW);
            showGameMenu();
            break;
        }
        case ID_SKIP:
        {
            shouldPickPawn = false;
            updatePoints(players.at(currentPlayerIndedx - 1), -10, p1Result, p2Result, p3Result, p4Result);
            nextPlayer();
            break;
        }
        }
        return 0;
    }

    case WM_DESTROY:
        logg.close();
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedureFinalResult(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc;
    PAINTSTRUCT ps;
    static HBRUSH brushing = CreateSolidBrush(RGB(159, 168, 218));

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        CreateWindowA("Button", "EXIT", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      80, 680, 230, 60, hwnd, (HMENU)ID_EXITFINAL, hinst, NULL);
        CreateWindowA("Button", "RESTART", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      80, 750, 230, 60, hwnd, (HMENU)ID_RESTARTFINAL, hinst, NULL);
        p1 = CreateWindow("static", "Player1", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 160, 230, 30, hwnd, (HMENU)ID_P1, hinst, NULL);
        CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 200, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p1Points = CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 200, 100, 30, hwnd, (HMENU)1, hinst, NULL);
        CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 240, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p1Pawns = CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 240, 100, 30, hwnd, (HMENU)2, hinst, NULL);
        p2 = CreateWindow("static", "Player2", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 280, 230, 30, hwnd, (HMENU)ID_P2, hinst, NULL);
        CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 320, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p2Points = CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 320, 100, 30, hwnd, (HMENU)1, hinst, NULL);
        CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 360, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p2Pawns = CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 360, 100, 30, hwnd, (HMENU)2, hinst, NULL);
        p3 = CreateWindow("static", "Player3", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 400, 230, 30, hwnd, (HMENU)ID_P3, hinst, NULL);
        CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 440, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p3Points = CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 440, 100, 30, hwnd, (HMENU)1, hinst, NULL);
        CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 480, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p3Pawns = CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 480, 100, 30, hwnd, (HMENU)2, hinst, NULL);
        p4 = CreateWindow("static", "Player4", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 520, 230, 30, hwnd, (HMENU)ID_P4, hinst, NULL);
        CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 560, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        p4Points = CreateWindow("static", "Points:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 560, 100, 30, hwnd, (HMENU)1, hinst, NULL);
        p4Pawns = CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 210, 600, 100, 30, hwnd, (HMENU)2, hinst, NULL);
        CreateWindow("static", "Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 600, 90, 30, hwnd, (HMENU)9, hinst, NULL);
        HFONT hf;
        HDC hdc;
        long lfHeight;
        hdc = GetDC(NULL);
        lfHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        ReleaseDC(NULL, hdc);

        hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
        SendMessage(p1, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p2, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p3, WM_SETFONT, (WPARAM)hf, TRUE);
        SendMessage(p4, WM_SETFONT, (WPARAM)hf, TRUE);
        break;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForMainResult(hdc, &rect, hbmFinalResultBackground);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(74, 0, 114));
        SetBkColor(hdcStatic, RGB(159, 168, 218));
        return (INT_PTR)brushing;
        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_RESTARTFINAL:

        {
            reinitialize();
            ShowWindow(finalResultHwnd, SW_HIDE);
            ShowWindow(mainGameHwnd, SW_SHOW);
            ShowWindow(mainResultHwnd, SW_SHOW);
            break;
        }
        case ID_EXITFINAL:
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

bool createGameMenu()
{
    gameMenuHwnd = CreateWindowEx(
        0,
        gameMenuClassName,
        _T("Game Menu"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
        500,
        20,
        500,
        700,
        HWND_DESKTOP,
        NULL,
        (HINSTANCE)GetWindowLong(mainGameHwnd, GWLP_HINSTANCE),
        NULL);
}
bool createMainResult()
{
    RECT clientrect;
    GetClientRect(mainGameHwnd, &clientrect);
    mainResultHwnd = CreateWindowEx(
        0,
        mainResultClassName,
        _T("Result"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP,
        clientrect.right + 125,
        clientrect.top + 10,
        600,
        1021,
        HWND_DESKTOP,
        NULL,
        (HINSTANCE)GetWindowLong(mainGameHwnd, GWLP_HINSTANCE),
        NULL);
    ShowWindow(mainResultHwnd, SW_HIDE);
}

void loadResources()
{
    hbmBackground = (HBITMAP)LoadImage(NULL, "resources/background2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoard = (HBITMAP)LoadImage(NULL, "resources/ludoboard_black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoardMask = (HBITMAP)LoadImage(NULL, "resources/ludoboard_white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmStartBackground = (HBITMAP)LoadImage(NULL, "resources/startbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmEnterNamesBackground = (HBITMAP)LoadImage(NULL, "resources/namesbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmGameMenuBackground = (HBITMAP)LoadImage(NULL, "resources/gamemenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmMainResultBackground = (HBITMAP)LoadImage(NULL, "resources/resultbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmFinalResultBackground = (HBITMAP)LoadImage(NULL, "resources/finalresultbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmGreenPawn = (HBITMAP)LoadImage(NULL, "resources/green_black54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmGreenPawnMask = (HBITMAP)LoadImage(NULL, "resources/green_white54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmRedPawn = (HBITMAP)LoadImage(NULL, "resources/red_black54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmRedPawnMask = (HBITMAP)LoadImage(NULL, "resources/red_white54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmYellowPawn = (HBITMAP)LoadImage(NULL, "resources/yellow_black54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmYellowPawnMask = (HBITMAP)LoadImage(NULL, "resources/yellow_white54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBluePawn = (HBITMAP)LoadImage(NULL, "resources/blue_black54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBluewPawnMask = (HBITMAP)LoadImage(NULL, "resources/blue_white54.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmGreenPawnSmall = (HBITMAP)LoadImage(NULL, "resources/green_black27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmGreenPawnSmallMask = (HBITMAP)LoadImage(NULL, "resources/green_white27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmRedPawnSmall = (HBITMAP)LoadImage(NULL, "resources/red_black27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmRedPawnSmallMask = (HBITMAP)LoadImage(NULL, "resources/red_white27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmYellowPawnSmall = (HBITMAP)LoadImage(NULL, "resources/yellow_black27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmYellowPawnSmallMask = (HBITMAP)LoadImage(NULL, "resources/yellow_white27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBluePawnSmall = (HBITMAP)LoadImage(NULL, "resources/blue_black27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBluewPawnSmallMask = (HBITMAP)LoadImage(NULL, "resources/blue_white27.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hbmDiceMask = (HBITMAP)LoadImage(NULL, "resources/dice_white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmDice = (HBITMAP)LoadImage(NULL, "resources/dice_black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

bool initialize()
{
    loadResources();
    mapOfPlayerHomes = generatePlayerHomes();
    mapOfBoard = generateBoard();
    players = loadGame(pOnePawns, pTwoPawns, pThreePawns, pFourPawns, mapOfPlayerHomes);
    for (int i = 0; i < 4; ++i)
    {
        playersDone.push_back(false);
    }
    vector<unsigned int> tmp;
    for (int i = 0; i < 4; ++i)
    {
        tmp.push_back(0);
    }
    for (int i = 1; i <= 81; ++i)
    {
        busyFields[i] = tmp;
    }
    for (int i = 1; i <= 4; ++i)
    {
        map<int, BoardField> mapBf = mapOfPlayerHomes[i];
        BoardField bf1 = mapBf[1];
        BoardField bf2 = mapBf[2];
        BoardField bf3 = mapBf[3];
        BoardField bf4 = mapBf[4];
        int xPos = (bf1.xPos + bf2.xPos) / 2;
        int yPos = (bf1.yPos + bf3.yPos) / 2;
        BoardField bf;
        bf.xPos = xPos;
        bf.yPos = yPos;
        mapOfDices[i] = bf;
    }
    return true;
}

bool reinitialize()
{
    pOnePawns.clear();
    pTwoPawns.clear();
    pThreePawns.clear();
    pFourPawns.clear();
    players = loadGame(pOnePawns, pTwoPawns, pThreePawns, pFourPawns, mapOfPlayerHomes);
    players.at(0).playerName = Player1;
    players.at(1).playerName = Player2;
    players.at(2).playerName = Player3;
    players.at(3).playerName = Player4;
    currentPlayerIndedx = 1;
    currentDiceNumber = 1;
    shouldPickPawn = false;
    SetWindowText(p1Result, "0");
    SetWindowText(p2Result, "0");
    SetWindowText(p3Result, "0");
    SetWindowText(p4Result, "0");

    playersDone.clear();
    for (int i = 0; i < 4; ++i)
    {
        playersDone.push_back(false);
    }
    busyFields.clear();
    vector<unsigned int> tmp;
    for (int i = 0; i < 4; ++i)
    {
        tmp.push_back(0);
    }
    for (int i = 1; i <= 81; ++i)
    {
        busyFields[i] = tmp;
    }

    return true;
}

bool showStart()
{
    HDC hdcStart = GetDC(startmenuHwnd);
    RECT rect1;
    GetClientRect(startmenuHwnd, &rect1);
    drawSceneForStart(hdcStart, &rect1, hbmStartBackground);
    ReleaseDC(startmenuHwnd, hdcStart);
    ShowWindow(startmenuHwnd, mCmdShow);
}
bool showEnterNames()
{
    HDC hdcEnter = GetDC(enterNamesHwnd);
    RECT rect1;
    GetClientRect(enterNamesHwnd, &rect1);
    drawSceneForStart(hdcEnter, &rect1, hbmEnterNamesBackground);
    ReleaseDC(enterNamesHwnd, hdcEnter);
    ShowWindow(enterNamesHwnd, SW_SHOW);
}
bool showGameMenu()
{
    HDC hdcGame = GetDC(gameMenuHwnd);
    RECT rect1;
    GetClientRect(gameMenuHwnd, &rect1);
    drawSceneForStart(hdcGame, &rect1, hbmGameMenuBackground);
    ReleaseDC(gameMenuHwnd, hdcGame);
    //  ShowWindow(gameMenuHwnd, mCmdShow);
}

bool showMainResult()
{
    HDC hdcResult = GetDC(mainResultHwnd);
    RECT rect1;
    GetClientRect(mainResultHwnd, &rect1);
    drawSceneForMainResult(hdcResult, &rect1, hbmMainResultBackground);
    ReleaseDC(mainResultHwnd, hdcResult);
    ShowWindow(mainResultHwnd, SW_SHOW);
}

bool showFinalResult()
{
    HDC hdcResult = GetDC(finalResultHwnd);
    RECT rect1;
    GetClientRect(finalResultHwnd, &rect1);
    drawSceneForMainResult(hdcResult, &rect1, hbmFinalResultBackground);
    ReleaseDC(finalResultHwnd, hdcResult);
    ShowWindow(finalResultHwnd, SW_SHOW);
}

void nextPlayer()
{
    if (currentPlayerIndedx == 4)
    {
        currentPlayerIndedx = 1;
    }
    else
    {
        currentPlayerIndedx += 1;
    }
    if (playersDone.at(currentPlayerIndedx - 1))
        nextPlayer();
}

void diceRoller(int num)
{
    if (!shouldPickPawn)
    {
        currentDiceNumber = rollTheDice(players.at(currentPlayerIndedx - 1), num);
        diceIsRolling = true;
        vector<Pawn> pawns = players.at(currentPlayerIndedx - 1).pawns;
        bool rollAgain = true;
        if (currentDiceNumber == 6)
        {
            rollAgain = false;
        }
        for (Pawn pawn : pawns)
        {
            if (pawn.currentPosition != 0 && !pawn.isFinished)
                rollAgain = false;
        }
        if (!rollAgain)
        {
            shouldPickPawn = true;
            if (currentDiceNumber == 6)
            {
                if (!muted)
                    PlaySound("resources/RolledSix.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
        }
        else
        {
            if (!muted)
                PlaySound("resources/LooserSound.wav", NULL, SND_FILENAME | SND_ASYNC);
            updatePoints(players.at(currentPlayerIndedx - 1), -10, p1Result, p2Result, p3Result, p4Result);
        }
    }
}

bool comparePlayerPoints(PlayerForSort p1, PlayerForSort p2)
{
    return (p1.playerPoints > p2.playerPoints);
}

void setEndText(PlayerForSort player, HWND &p, HWND &pPoint, HWND &pPawn)
{
    string player1Name = player.playerName;
    int player1Points = player.playerPoints;
    int player1PawnsEaten = player.pawnsEaten;
    char nameBuff1[player1Name.length() + 1];
    strcpy(nameBuff1, player1Name.c_str());
    char pointsBuff1[7];
    sprintf(pointsBuff1, "%d", player1Points);
    char pawnsBuff1[10];
    sprintf(pawnsBuff1, "%d", player1PawnsEaten);
    SetWindowText(p, nameBuff1);
    SetWindowText(pPoint, pointsBuff1);
    SetWindowText(pPawn, pawnsBuff1);
}

void endGame()
{
    PlaySound("resources/EndGame.wav", NULL, SND_FILENAME | SND_ASYNC);
    ShowWindow(mainGameHwnd, SW_HIDE);
    ShowWindow(mainResultHwnd, SW_HIDE);
    std::vector<PlayerForSort> sortedPlayers;
    for (Player player : players)
    {
        PlayerForSort ps;
        ps.playerName = player.playerName;
        ps.pawnsEaten = player.pawnsEaten;
        ps.playerPoints = player.playerPoints;
        sortedPlayers.push_back(ps);
    }
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), comparePlayerPoints);
    setEndText(sortedPlayers.at(0), p1, p1Points, p1Pawns);
    setEndText(sortedPlayers.at(1), p2, p2Points, p2Pawns);
    setEndText(sortedPlayers.at(2), p3, p3Points, p3Pawns);
    setEndText(sortedPlayers.at(3), p4, p4Points, p4Pawns);
    ShowWindow(finalResultHwnd, SW_SHOW);
    showFinalResult();
}

void drawPawn(HDC &hdcBuffer, HDC &hdcMem, int index, int xPos, int yPos, bool drawSmall)
{
    switch (index)
    {
    case 1:
    {
        if (drawSmall)
        {
            (HBITMAP) SelectObject(hdcMem, hbmYellowPawnSmallMask);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 27, 27, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmYellowPawnSmall);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 27, 27, hdcMem,
                   0, 0, SRCPAINT);
        }
        else
        {
            (HBITMAP) SelectObject(hdcMem, hbmYellowPawnMask);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmYellowPawn);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCPAINT);
        }

        break;
    }
    case 2:
    {
        if (drawSmall)
        {
            (HBITMAP) SelectObject(hdcMem, hbmBluewPawnSmallMask);
            BitBlt(hdcBuffer, xPos, yPos - 27, 27, 27, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmBluePawnSmall);
            BitBlt(hdcBuffer, xPos, yPos - 27, 27, 27, hdcMem,
                   0, 0, SRCPAINT);
        }
        else
        {
            (HBITMAP) SelectObject(hdcMem, hbmBluewPawnMask);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmBluePawn);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCPAINT);
        }

        break;
    }
    case 3:
    {
        if (drawSmall)
        {
            (HBITMAP) SelectObject(hdcMem, hbmRedPawnSmallMask);
            BitBlt(hdcBuffer, xPos, yPos, 27, 27, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmRedPawnSmall);
            BitBlt(hdcBuffer, xPos, yPos, 27, 27, hdcMem,
                   0, 0, SRCPAINT);
        }
        else
        {
            (HBITMAP) SelectObject(hdcMem, hbmRedPawnMask);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmRedPawn);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCPAINT);
        }

        break;
    }
    case 4:
    {
        if (drawSmall)
        {
            (HBITMAP) SelectObject(hdcMem, hbmGreenPawnSmallMask);
            BitBlt(hdcBuffer, xPos - 27, yPos, 27, 27, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmGreenPawnSmall);
            BitBlt(hdcBuffer, xPos - 27, yPos, 27, 27, hdcMem,
                   0, 0, SRCPAINT);
        }
        else
        {
            (HBITMAP) SelectObject(hdcMem, hbmGreenPawnMask);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCAND);

            (HBITMAP) SelectObject(hdcMem, hbmGreenPawn);
            BitBlt(hdcBuffer, xPos - 27, yPos - 27, 54, 54, hdcMem,
                   0, 0, SRCPAINT);
        }

        break;
    }

    default:
        break;
    }
}

void drawDice(HDC &hdcBuffer, HDC &hdcMem, int index, int counter)
{
    switch (index)
    {
    case 1:
    {
        BoardField bf = mapOfDices[index];
        int position = 3;
        (HBITMAP) SelectObject(hdcMem, hbmDiceMask);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCAND);

        (HBITMAP) SelectObject(hdcMem, hbmGreenPawn);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCPAINT);
        break;
    }

    case 2:
    {
        BoardField bf = mapOfDices[index];
        int position = 5;
        (HBITMAP) SelectObject(hdcMem, hbmDiceMask);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCAND);

        (HBITMAP) SelectObject(hdcMem, hbmGreenPawn);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCPAINT);
        break;
    }

    case 3:
    {
        BoardField bf = mapOfDices[index];
        int position = 1;
        (HBITMAP) SelectObject(hdcMem, hbmDiceMask);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCAND);

        (HBITMAP) SelectObject(hdcMem, hbmGreenPawn);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCPAINT);
        break;
    }

    case 4:
    {
        BoardField bf = mapOfDices[index];
        int position = 4;
        (HBITMAP) SelectObject(hdcMem, hbmDiceMask);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCAND);

        (HBITMAP) SelectObject(hdcMem, hbmGreenPawn);
        BitBlt(hdcBuffer, bf.xPos - 32, bf.yPos - 32, 64, 64, hdcMem,
               counter * 64, position * 64, SRCPAINT);
        break;
    }

    default:
        break;
    }
}

void drawScene(HDC hdc, RECT *rect)
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

    for (Player player : players)
    {
        std::vector<Pawn> pawns = player.pawns;
        for (Pawn pawn : pawns)
        {
            bool drawSmall = false;
            if (pawn.currentPosition != 0)
            {
                std::vector<unsigned int> vec = busyFields[pawn.currentPosition];
                for (int i = 0; i < 4; ++i)
                {
                    if (i + 1 != player.playerIndex && vec.at(i) > 0)
                        drawSmall = true;
                }
            }
            if (!pawn.isFinished)
                drawPawn(hdcBuffer, hdcMem, player.playerIndex, pawn.xPos, pawn.yPos, drawSmall);
        }
    }

    if (diceIsRolling)
    {
        diceAnimPos = (diceAnimPos + 1) % 6;
        if (diceAnimPos == 0)
        {
            diceAnim = (diceAnim + 1);
        }
        if (diceAnim == 6)
        {
            diceIsRolling = false;
            diceAnim = 0;
        }
        drawDice(hdcBuffer, hdcMem, currentPlayerIndedx, diceAnimPos);
    }
    else
    {
        drawDice(hdcBuffer, hdcMem, currentPlayerIndedx, currentDiceNumber - 1);
    }

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, rect->right, rect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, oldHbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}