#ifndef MAINFUNC_HPP
#define MAINFUNC_HPP

#include <windef.h>
#include <vector>
#include <map>
#include "player.hpp"

void loadResources();
void drawSceneForStart(HDC, RECT *, HBITMAP);
void drawSceneForMainResult(HDC, RECT *, HBITMAP);
bool createMainResult();
bool createGameMenu();
bool showStart();
bool showFinalResult();
bool showEnterNames();
bool showGameMenu();
bool showMainResult();
bool initialize();
void nextPlayer();
void diceRoller(int num);
bool reinitialize();
void endGame();
void drawScene(HDC hdc, RECT *rect);

LRESULT CALLBACK WindowProcedureFinalResult(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedureMainResult(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedureGameMenu(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedureEnterNames(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WindowProcedureStart(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int registerWindowWithCustomIcon(TCHAR szClassName[],
                                 HINSTANCE hThisInstance,
                                 HINSTANCE hPrevInstance,
                                 LPSTR lpszArgument,
                                 WNDCLASSEX &wincl,
                                 int HICON,
                                 int HICONSM,
                                 LPCSTR HCURSOR,
                                 void *winClass);

int registerWindow(TCHAR szClassName[],
                   HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   WNDCLASSEX &wincl,
                   LPCSTR HICON,
                   LPCSTR HICONSM,
                   LPCSTR HCURSOR,
                   void *winClass);

#endif
