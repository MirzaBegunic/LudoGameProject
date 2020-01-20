#ifndef MAINFUNC_HPP
#define MAINFUNC_HPP

#include<windef.h>

void loadResources();
void drawScene(HDC, RECT *, HBITMAP, HBITMAP, HBITMAP);
void drawSceneForStart(HDC, RECT *, HBITMAP);
bool showStart();

bool initialize();
void CheckInput(RECT *rect);
void testFields(HDC hdc, std::map<int, board_field>& mapOfBoard, std::map<int, std::map<int, board_field>>& mapOfPlayerHomes);


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