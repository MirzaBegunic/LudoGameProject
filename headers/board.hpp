#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <list>

struct BoardField
{
    int index;
    double xPos;
    double yPos;
    int top;
    int left;
    int right;
    int bottom;
};

struct Board{
    std::map<int, BoardField> boardPositions;
    std::map<int,std::map<int,BoardField>> homeBoardPositions;
};

std::map<int,BoardField> generateBoard();
std::map<int,std::map<int,BoardField>> generatePlayerHomes();


#endif
