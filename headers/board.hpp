#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>



struct board_field
{
    double xPos;
    double yPos;
    int top;
    int left;
    int right;
    int bottom;
};


std::map<int,board_field> generateBoard();

#endif
