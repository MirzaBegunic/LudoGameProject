#include "headers/board.hpp"
#include <map>


void generateBounds(board_field& bf) {
    bf.bottom = bf.yPos + 27;
    bf.left = bf.xPos - 27;
    bf.top = bf.yPos - 27;
    bf.right = bf.xPos + 27;
}

std::map<int,board_field> generateBoard() {
    std::map<int, board_field> positionMap;
    // first vertical row
    board_field lastField;
    lastField.xPos = 450;
    lastField.yPos = 945;
    generateBounds(lastField);
    positionMap[1] = lastField;

    for (int i=2; i<= 7; ++i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }
    lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
    positionMap[68] = lastField;

    for (int i=21; i<= 27; ++i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }
    //second vertical row
    lastField.xPos = 512;
    lastField.yPos = 945;
        generateBounds(lastField);
    positionMap[56] = lastField;

    for (int i=57; i<= 62; ++i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
    positionMap[81] = lastField;

    for (int i=74; i>= 69; --i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
    positionMap[28] = lastField;

    //third vertical row
    lastField.xPos = 574;
    lastField.yPos = 945;
        generateBounds(lastField);
    positionMap[55] = lastField;

    for (int i=54; i>= 49; --i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
    positionMap[75] = lastField;

    for (int i=35; i>= 29; --i) {
        lastField.yPos = lastField.yPos - 54.6 - 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    // first horizontal row
    lastField.yPos = 450;
    lastField.xPos = 80;
        generateBounds(lastField);
    positionMap[15] = lastField;

    for(int i=16; i<=20; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    for (int i=0; i<3; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
    }

    for(int i=36; i<=41; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    //second horizontal row
    lastField.xPos = 80;
    lastField.yPos = 513;
        generateBounds(lastField);
    positionMap[14] = lastField;

    for(int i=63; i<=67; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    for (int i=0; i<3; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
    }

    for(int i=76; i<=80; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
    positionMap[42] = lastField;

    // thrid horizontal row
    lastField.yPos = 576;
    lastField.xPos = 80;
        generateBounds(lastField);
    positionMap[13] = lastField;

    for(int i=12; i>=8; --i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    for (int i=0; i<3; ++i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
    }

    for(int i=48; i>=43; --i) {
        lastField.xPos = lastField.xPos + 54.6 + 7;
        generateBounds(lastField);
        positionMap[i] = lastField;
    }

    return positionMap;
}
