#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define Pressed(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
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
HBRUSH hbrushEditBox = 0;
COLORREF blue=RGB(0,0,255);
COLORREF red=RGB(255,0,0);
COLORREF green=RGB(0,255,0);
COLORREF yellow=RGB(255,255,0);

/**
 * global variables 
 */

HWND mainGameHwnd, startmenuHwnd,enterNamesHwnd,gameMenuHwnd,mainResultHwnd,finalResultHwnd;
HWND p1Result, p2Result, p3Result, p4Result;
HWND p1,p1Points,p1Pawns,p2,p2Points,p2Pawns,p3,p3Points,p3Pawns,p4,p4Points,p4Pawns;

std::map<int, std::map<int, BoardField>> mapOfPlayerHomes;
std::map<int, BoardField> mapOfBoard;
std::vector<Player> players;
std::vector<Pawn> pOnePawns;
std::vector<Pawn> pTwoPawns;
std::vector<Pawn> pFourPawns;
std::vector<Pawn> pThreePawns;
std::list<BoardField> busyFields;
std::vector<bool> playersDone;

char Player1[40];
char Player2[40];
char Player3[40];
char Player4[40];
int currentPlayerIndedx = 1;
int currentDiceNumber = 1;
bool shouldPickPawn = false;

TCHAR szClassName[] = _T("LudoGameApp");
TCHAR startClassName[] = _T("Start");
TCHAR enterNameClassName[] = _T("Enter Names");
TCHAR gameMenuClassName[] = _T("Game Menu");
TCHAR mainResultClassName[] = _T("Main Result");
TCHAR FinalResultClassName[] = _T("Final Result");

void testMove(Player player, std::ofstream & logg);
int mCmdShow;


int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    logg.open("logs.txt");
    MSG messages;
    WNDCLASSEX wincl, winclStartMenu,winclEnterNames,winclGameMenu,winclMainResult,winclFinalResult;
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
        WS_OVERLAPPEDWINDOW,
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
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION|WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
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
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION|WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
        500,
        20,
        400,
        950,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL);

        ShowWindow(enterNamesHwnd,SW_HIDE);
        ShowWindow(gameMenuHwnd,SW_HIDE);
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
            drawScene(hdc, &rect,hbmBackground, hbmBoardMask, hbmBoard);
            //testPawn(hdc, players);
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
    case WM_LBUTTONDOWN: {
        if(shouldPickPawn) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (pawnSelection(logg,players.at(currentPlayerIndedx-1), x, y,mapOfPlayerHomes, mapOfBoard)) {
                if(calculatePlayerMove(players.at(currentPlayerIndedx-1), mapOfBoard, logg, p1Result, p2Result, p3Result, p4Result, playersDone)){
                    shouldPickPawn = false;
                    if(checkIfPawnsShouldBeEaten(players, players.at(currentPlayerIndedx-1), mapOfPlayerHomes, p1Result, p2Result, p3Result, p4Result, playersDone))
                        MessageBox(mainGameHwnd, "This Guy FUCKS","Ouuu", MB_OKCANCEL);
                    if (currentDiceNumber != 6) {
                        nextPlayer();
                    }
                    bool finish = true;
                    for(bool isFinish : playersDone) {
                        if(!isFinish) {
                            finish = false;
                        }
                    }
                    if(finish) {
                        endGame();
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
        endGame();
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

    case WM_PAINT: {
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
            ShowWindow(enterNamesHwnd,SW_SHOW);
            showEnterNames(); 
            ShowWindow(hwnd,SW_HIDE);
            
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
   
    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {   
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);  
       HWND button= CreateWindowA("Button", "CONTINUE", WS_CHILD | WS_VISIBLE |BS_CHECKBOX | BS_PUSHLIKE,
                     130, 570, 300, 60, hwnd, (HMENU)ID_CONTINUE, hinst, NULL); 
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER |ES_AUTOHSCROLL|ES_CENTER, 130,190 , 300, 30, hwnd, (HMENU)ID_PLAYER1, hinst, NULL);  
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER |ES_AUTOHSCROLL|ES_CENTER, 130,290 , 300, 30, hwnd, (HMENU)ID_PLAYER2, hinst, NULL);        
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER |ES_AUTOHSCROLL|ES_CENTER, 130,390 , 300, 30, hwnd, (HMENU)ID_PLAYER3, hinst, NULL);  
        CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER |ES_AUTOHSCROLL|ES_CENTER, 130,490 , 300, 30, hwnd, (HMENU)ID_PLAYER4, hinst, NULL); 
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
        if(GetDlgCtrlID((HWND)lParam) == ID_PLAYER1){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, yellow); 
        SetBkMode(hdcEdit, TRANSPARENT);
        return (LONG)hbrushEditBox; 
        } 
        if(GetDlgCtrlID((HWND)lParam) == ID_PLAYER2){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, blue); 
        return (LONG)hbrushEditBox; 
        }
        if(GetDlgCtrlID((HWND)lParam) == ID_PLAYER3){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, red); 
        return (LONG)hbrushEditBox; 
        }
        if(GetDlgCtrlID((HWND)lParam) == ID_PLAYER4){
        HDC hdcEdit = (HDC)wParam; 
        SetTextColor(hdcEdit, green); 
        return (LONG)hbrushEditBox; 
        }

        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_PLAYER1:
                if(HIWORD(wParam)==EN_CHANGE)
                {  
                   GetWindowText((HWND)lParam,Player1,40);
                   players.at(0).playerName = Player1;
                }
                break;
        
        case ID_PLAYER2:
               
                if(HIWORD(wParam)==EN_CHANGE)
                {
                    GetWindowText((HWND)lParam,Player2,40);
                    players.at(1).playerName = Player2;
                }
                break;
        case ID_PLAYER3:
                if(HIWORD(wParam)==EN_CHANGE)
                {   
                    GetWindowText((HWND)lParam,Player3,40);
                    players.at(2).playerName = Player3;
                }
                break;
        case ID_PLAYER4:
                if(HIWORD(wParam)==EN_CHANGE)
                {   
                    GetWindowText((HWND)lParam,Player4,40);   
                    players.at(3).playerName = Player4;
                }
                break;   
        case ID_CONTINUE:

        {
            HWND p1=GetDlgItem(hwnd,ID_PLAYER1);  
            HWND p2=GetDlgItem(hwnd,ID_PLAYER2);  
            HWND p3=GetDlgItem(hwnd,ID_PLAYER3);  
            HWND p4=GetDlgItem(hwnd,ID_PLAYER4);  
            int length1 =(int) SendMessage(p1,WM_GETTEXTLENGTH,0,0);
            int length2 =(int) SendMessage(p2,WM_GETTEXTLENGTH,0,0);
            int length3 =(int) SendMessage(p3,WM_GETTEXTLENGTH,0,0);
            int length4 =(int) SendMessage(p4,WM_GETTEXTLENGTH,0,0);
            if(length1!=0 && length2!=0 && length3!=0 && length4!=0)
            { 
            ShowWindow(mainGameHwnd, mCmdShow);
            createMainResult();
            ShowWindow(mainResultHwnd, SW_SHOW);
            showMainResult();
            ShowWindow(hwnd,SW_HIDE);
            break;
            }
            else{ 
            MessageBox(0,"Please enter all players",0,0); 
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
    {   HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        CreateWindowA("Button", "MUTE", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      140, 270, 300, 60, hwnd, (HMENU)ID_MUTE, hinst, NULL);
        CreateWindowA("Button", "RESTART", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      140, 380, 300, 60, hwnd, (HMENU)ID_RESTART, hinst, NULL);
        CreateWindowA("Button", "EXIT", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      140, 490, 300, 60, hwnd, (HMENU)ID_EXIT1, hinst, NULL);
        break;
    }

    case WM_PAINT: {
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
            //Mute 
            break;
        }
        case ID_EXIT1:
        {
            ShowWindow(hwnd,SW_HIDE);
        }
     }
        return 0;
        }
    case WM_DESTROY:
        logg.close();
         ShowWindow(gameMenuHwnd, SW_HIDE);
      //  PostQuitMessage(0); /* send a WM_QUIT to the message queue */
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
    switch (message) /* handle the messages */
    {
     
    case WM_CREATE:
    {  
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);  
        HWND p1=CreateWindow("static",Player1, WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,140,310 , 230, 30, hwnd, (HMENU)ID_P1, hinst, NULL);  
        p1Result=CreateWindow("static","0", WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,450,310 ,80,30, hwnd, (HMENU)ID_P1RESULT, hinst, NULL);    
        HWND p2=CreateWindow("static",Player2, WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,140,410 , 230, 30, hwnd, (HMENU)ID_P2, hinst, NULL);  
        p2Result=CreateWindow("static","0", WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,450,410 ,80, 30, hwnd, (HMENU)ID_P2RESULT, hinst, NULL);    
        HWND p3=CreateWindow("static",Player3, WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,140,510 , 230, 30, hwnd, (HMENU)ID_P3, hinst, NULL);  
        p3Result=CreateWindow("static","0", WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,450,510,80, 30, hwnd, (HMENU)ID_P3RESULT, hinst, NULL);    
        HWND p4=CreateWindow("static",Player4, WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,140,610 , 230, 30, hwnd, (HMENU)ID_P4, hinst, NULL);  
        p4Result=CreateWindow("static","0", WS_CHILD | WS_VISIBLE | WS_BORDER|ES_AUTOHSCROLL
        |ES_CENTER,450,610 ,80, 30, hwnd, (HMENU)ID_P4RESULT, hinst, NULL);    
        CreateWindow("button","DICE ROLLER", WS_CHILD | WS_VISIBLE | BS_CHECKBOX 
        | BS_PUSHLIKE,140,700 , 230, 80, hwnd, (HMENU)ID_DICE, hinst, NULL);  
        CreateWindow("button","SKIP", WS_CHILD | WS_VISIBLE | BS_CHECKBOX 
        | BS_PUSHLIKE,140,800 , 230, 80, hwnd, (HMENU)ID_SKIP, hinst, NULL); 
         CreateWindow("button","OPEN MENU", WS_CHILD | WS_VISIBLE | BS_CHECKBOX 
        | BS_PUSHLIKE,140,900 , 230, 80, hwnd, (HMENU)ID_GAMEMENU, hinst, NULL); 
        blue=COLORREF(RGB(0,0,255));

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
    case WM_PAINT:{
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForMainResult(hdc, &rect, hbmMainResultBackground);
        EndPaint(hwnd,&ps);
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
            diceRoller();
            break;
        }
        case ID_GAMEMENU:{
            createGameMenu();
            ShowWindow(gameMenuHwnd, SW_SHOW);
            showGameMenu();
            break;
        }
        case ID_SKIP:{
            shouldPickPawn = false;
            updatePoints(players.at(currentPlayerIndedx-1),-10, p1Result, p2Result, p3Result, p4Result); 
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

    switch (message) /* handle the messages */
    {
    case WM_CREATE:
    {
        HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        CreateWindowA("Button", "EXIT", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      80, 650, 250, 60, hwnd, (HMENU)ID_EXITFINAL, hinst, NULL);
        CreateWindowA("Button", "RESTART", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
                      80,720, 250, 60, hwnd, (HMENU)ID_RESTARTFINAL, hinst, NULL);
        p1=CreateWindow("static","Player1", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,50 , 230, 30, hwnd, (HMENU)ID_P1, hinst, NULL);  
        p1Points=CreateWindow("static","Points:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,90 , 100, 30, hwnd, (HMENU)1, hinst, NULL);  
        p1Pawns=CreateWindow("static","Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,130 , 100, 30, hwnd, (HMENU)2, hinst, NULL);  
        p2=CreateWindow("static","Player2", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,180 , 230, 30, hwnd, (HMENU)ID_P2, hinst, NULL);  
        p2Points=CreateWindow("static","Points:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,220 , 100, 30, hwnd, (HMENU)1, hinst, NULL);  
        p2Pawns=CreateWindow("static","Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,260 , 100, 30, hwnd, (HMENU)2, hinst, NULL);  
        p3=CreateWindow("static","Player3", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,300 , 230, 30, hwnd, (HMENU)ID_P3, hinst, NULL);  
        p3Points=CreateWindow("static","Points:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,340 , 100, 30, hwnd, (HMENU)1, hinst, NULL);  
        p3Pawns=CreateWindow("static","Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,380 , 100, 30, hwnd, (HMENU)2, hinst, NULL);  
        p4=CreateWindow("static","Player4", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,430 , 230, 30, hwnd, (HMENU)ID_P4, hinst, NULL);  
        p4Points=CreateWindow("static","Points:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,470 , 100, 30, hwnd, (HMENU)1, hinst, NULL);  
        p4Pawns=CreateWindow("static","Pawns eaten:", WS_CHILD | WS_VISIBLE | WS_BORDER
        |ES_CENTER,20,510 , 100, 30, hwnd, (HMENU)2, hinst, NULL);  
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
    case WM_PAINT: {
        hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        drawSceneForMainResult(hdc, &rect, hbmFinalResultBackground);
        EndPaint(hwnd, &ps);
        break;
    }
     case WM_CTLCOLOREDIT:
    { 
      if(GetDlgCtrlID((HWND)lParam) == ID_P4){
        HDC hdcStatic = (HDC)wParam; 
        SetBkMode(hdcStatic,TRANSPARENT);
         return (LONG) hbrushEditBox; 
        } 
    
        break;
    }
    
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_RESTARTFINAL:
        
        {   
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


bool createGameMenu(){
gameMenuHwnd = CreateWindowEx(
        0,
        gameMenuClassName,
        _T("Game Menu"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION|WS_VISIBLE|WS_GROUP | WS_TABSTOP | WS_POPUP | WS_SYSMENU,
        500,
        20,
        500,
        700,
        HWND_DESKTOP,
        NULL,
        (HINSTANCE)GetWindowLong(mainGameHwnd,GWLP_HINSTANCE),
        NULL);
}
bool createMainResult(){
        RECT clientrect;
        GetClientRect(mainGameHwnd,&clientrect);
        mainResultHwnd = CreateWindowEx(
        0,
        mainResultClassName,
        _T("Result"),
        DS_3DLOOK | DS_CENTER | DS_MODALFRAME | DS_SHELLFONT | WS_CAPTION|WS_VISIBLE|WS_GROUP | WS_TABSTOP | WS_POPUP ,
        clientrect.right+125,
        clientrect.top+10,
        600,
        1021,
        HWND_DESKTOP,
        NULL,
        (HINSTANCE)GetWindowLong(mainGameHwnd,GWLP_HINSTANCE),
        NULL);
        ShowWindow(mainResultHwnd,SW_HIDE);
}

void loadResources()
{
    hbmBackground = (HBITMAP)LoadImage(NULL, "resources/background2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoard = (HBITMAP)LoadImage(NULL, "resources/ludoboard_black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmBoardMask = (HBITMAP)LoadImage(NULL, "resources/ludoboard_white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmStartBackground = (HBITMAP)LoadImage(NULL, "resources/startbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmEnterNamesBackground = (HBITMAP)LoadImage(NULL, "resources/namesbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmGameMenuBackground = (HBITMAP)LoadImage(NULL, "resources/gamemenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmMainResultBackground= (HBITMAP)LoadImage(NULL, "resources/resultbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hbmFinalResultBackground = (HBITMAP)LoadImage(NULL, "resources/finalresultbackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

bool initialize()
{
    loadResources();
    mapOfPlayerHomes = generatePlayerHomes(busyFields);
    mapOfBoard = generateBoard();
    players = loadGame(pOnePawns, pTwoPawns, pThreePawns, pFourPawns, mapOfPlayerHomes);
    for(int i=0; i<4; ++i) {
        playersDone.push_back(false);
    }
    return true;
}

bool reinitialize() {
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
    SetWindowText(p1Result,"0");
    SetWindowText(p2Result,"0");
    SetWindowText(p3Result,"0");
    SetWindowText(p4Result,"0");

    playersDone.clear();
    for(int i=0; i<4; ++i) {
        playersDone.push_back(false);
    }

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
bool showEnterNames() {
    HDC hdcEnter = GetDC(enterNamesHwnd);
    RECT rect1;
    GetClientRect(enterNamesHwnd, &rect1);
    drawSceneForStart(hdcEnter, &rect1, hbmEnterNamesBackground);
    ReleaseDC(enterNamesHwnd, hdcEnter);
    ShowWindow(enterNamesHwnd, SW_SHOW);
}
bool showGameMenu() {
    HDC hdcGame = GetDC(gameMenuHwnd);
    RECT rect1;
    GetClientRect(gameMenuHwnd, &rect1);
    drawSceneForStart(hdcGame, &rect1,hbmGameMenuBackground);
    ReleaseDC(gameMenuHwnd, hdcGame);
  //  ShowWindow(gameMenuHwnd, mCmdShow);
}

bool showMainResult() {
    HDC hdcResult = GetDC(mainResultHwnd);
    RECT rect1;
    GetClientRect(mainResultHwnd, &rect1);
    drawSceneForMainResult(hdcResult, &rect1,hbmMainResultBackground);
    ReleaseDC(mainResultHwnd, hdcResult);
    ShowWindow(mainResultHwnd, SW_SHOW);
}

bool showFinalResult() {
    HDC hdcResult = GetDC(finalResultHwnd);
    RECT rect1;
    GetClientRect(finalResultHwnd, &rect1);
    drawSceneForMainResult(hdcResult, &rect1,hbmFinalResultBackground);
    ReleaseDC(finalResultHwnd, hdcResult);
    ShowWindow(finalResultHwnd, SW_SHOW);
}

void nextPlayer() {
    if(currentPlayerIndedx == 4) {
        currentPlayerIndedx = 1;
    } else {
        currentPlayerIndedx += 1;
    }
}

void diceRoller() {
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
            updatePoints(players.at(currentPlayerIndedx-1),-10, p1Result, p2Result, p3Result, p4Result); 
        }
    }    
}

bool comparePlayerPoints(const Player & p1, const Player & p2) {
    return (p1.playerPoints > p2.playerPoints);
}

void setEndText(Player player, HWND& p, HWND& pPoint, HWND& pPawn) {
    string player1Name = player.playerName;
    int player1Points = player.playerPoints;
    int player1PawnsEaten = player.pawnsEaten;
    char nameBuff1[player1Name.length() + 1];
    strcpy(nameBuff1, player1Name.c_str());
    char pointsBuff1[7];
    sprintf(pointsBuff1,"%d",player1Points);
    char pawnsBuff1[10];
    sprintf(pawnsBuff1,"%d",player1PawnsEaten);
    SetWindowText(p,nameBuff1);
    SetWindowText(pPoint, pointsBuff1);
    SetWindowText(pPawn, pawnsBuff1);
}

void endGame() {
    CloseWindow(mainGameHwnd);
    CloseWindow(mainResultHwnd);
    std::vector<Player> sortedPlayers = players;
    //std::sort(sortedPlayers.begin(), sortedPlayers.end(), comparePlayerPoints);
    setEndText(sortedPlayers.at(0),p1,p1Points,p1Pawns);
    setEndText(sortedPlayers.at(1),p2,p2Points,p2Pawns);
    setEndText(sortedPlayers.at(2),p3,p3Points,p3Pawns);
    setEndText(sortedPlayers.at(3),p4,p4Points,p4Pawns);
    ShowWindow(finalResultHwnd, SW_SHOW);
    showFinalResult();
}
