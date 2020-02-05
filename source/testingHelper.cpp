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
#include "../headers/player.hpp"

void testDice(HDC hdc, std::map<int, BoardField> dice)
{
    for (int i = 1; i <= 4; ++i)
    {
        BoardField bf = dice[i];
        Rectangle(hdc, bf.xPos - 5, bf.yPos - 5, bf.xPos + 5, bf.yPos + 5);
    }
}

void testFields(HDC hdc, std::map<int, BoardField> &mapOfBoard, std::map<int, std::map<int, BoardField>> &mapOfPlayerHomes)
{
    for (int i = 1; i <= 4; ++i)
    {
        std::map<int, BoardField> tmp = mapOfPlayerHomes[i];

        for (int j = 1; j <= 4; ++j)
        {
            BoardField bf = tmp[j];
            Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
        }
    }
    for (int i = 0; i <= 81; ++i)
    {
        BoardField bf = mapOfBoard[i];
        Rectangle(hdc, bf.left, bf.top, bf.right, bf.bottom);
    }
}

void testLoadGame(Player playerOne,
                  Player playerTwo,
                  Player playerThree,
                  Player playerFour,
                  std::ofstream &logg)
{
    logg << playerOne.playerIndex << " " << playerOne.startingPoint << " " << playerOne.homePoint << " " << playerOne.homeEnterancePoint << "\n";

    for (Pawn pawn : playerOne.pawns)
    {
        logg << pawn.playerIndex << " " << pawn.number << " " << pawn.xPos << " " << pawn.yPos << "\n";
    }
    logg << "\n";

    logg << playerTwo.playerIndex << " " << playerTwo.startingPoint << " " << playerTwo.homePoint << " " << playerTwo.homeEnterancePoint << "\n";

    for (Pawn pawn : playerTwo.pawns)
    {
        logg << pawn.playerIndex << " " << pawn.number << " " << pawn.xPos << " " << pawn.yPos << "\n";
    }
    logg << "\n";

    logg << playerThree.playerIndex << " " << playerThree.startingPoint << " " << playerThree.homePoint << " " << playerThree.homeEnterancePoint << "\n";

    for (Pawn pawn : playerThree.pawns)
    {
        logg << pawn.playerIndex << " " << pawn.number << " " << pawn.xPos << " " << pawn.yPos << "\n";
    }
    logg << "\n";

    logg << playerFour.playerIndex << " " << playerFour.startingPoint << " " << playerFour.homePoint << " " << playerFour.homeEnterancePoint << "\n";

    for (Pawn pawn : playerFour.pawns)
    {
        logg << pawn.playerIndex << " " << pawn.number << " " << pawn.xPos << " " << pawn.yPos << "\n";
    }
    logg << "\n";
}

void testMove(Player player, std::ofstream &logg)
{
    logg << "player index " << player.playerIndex << "rolled " << player.currentDiceRolled << "Pawn ended up at" << player.pawns.at(player.pawnPickedIndex - 1).currentPosition << "\n";
}

void testPawn(HDC hdc, std::vector<Player> players)
{
    for (Player player : players)
    {
        switch (player.playerIndex)
        {
        case 1:
        {
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
            SelectObject(hdc, brush);
            break;
        }

        case 2:
        {
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
            SelectObject(hdc, brush);
            break;
        }

        case 3:
        {
            HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, brush);
            break;
        }

        case 4:
        {
            HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
            SelectObject(hdc, brush);
            break;
        }

        default:
            break;
        }
        for (Pawn pawn : player.pawns)
        {
            if (!pawn.isFinished)
                Rectangle(hdc, pawn.xPos - 5, pawn.yPos - 5, pawn.xPos + 5, pawn.yPos + 5);
        }
    }
}