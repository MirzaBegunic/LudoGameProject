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

void testFields(HDC hdc, std::map<int, board_field>& mapOfBoard, std::map<int, std::map<int, board_field>>& mapOfPlayerHomes)
{
    for (int i = 1; i <= 4; ++i)
    {
        std::map<int, board_field> tmp = mapOfPlayerHomes[i];

        for (int j = 1; j <= 4; ++j)
        {
            board_field bf = tmp[j];
            Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
        }
    }
    for (int i = 0; i <= 81; ++i)
    {
        board_field bf = mapOfBoard[i];
        Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
    }
}