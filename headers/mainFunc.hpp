#ifndef MAINFUNC_HPP
#define MAINFUNC_HPP

#include<windef.h>
#include<vector>
#include<map>
#include"player.hpp"

void loadResources();
void drawScene(HDC, RECT *, HBITMAP, HBITMAP, HBITMAP);
void drawSceneForStart(HDC, RECT *, HBITMAP);
bool showStart();

bool initialize();
void nextPlayer();

void testFields(HDC hdc, std::map<int, BoardField>& mapOfBoard, std::map<int, std::map<int, BoardField>>& mapOfPlayerHomes);
void testPawn(HDC hdc, std::vector<Player> players);

LRESULT CALLBACK WindowProcedureStart(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int registerWindowWithCustomIcon(TCHAR szClassName[],
                   HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   WNDCLASSEX& wincl,
                   int HICON,
                   int HICONSM,
                   LPCSTR HCURSOR,
                   void * winClass);


int registerWindow(TCHAR szClassName[],
                   HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   WNDCLASSEX& wincl,
                   LPCSTR HICON,
                   LPCSTR HICONSM,
                   LPCSTR HCURSOR,
                   void * winClass);

#endif