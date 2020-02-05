#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

struct Pawn
{
    int playerIndex;
    int number;
    int currentPosition;
    int goalPosition;
    int xPos;
    int yPos;
    bool isFinished = false;
};

class Player
{
public:
    Player(int pInd, int startPoint, int homePoint_, int homeEnt, int homeEnd_, std::vector<Pawn> &pawns_) : pawns{pawns_}
    {
        playerIndex = pInd;
        startingPoint = startPoint;
        homePoint = homePoint_;
        homeEnterancePoint = homeEnt;
        homeEnd = homeEnd_;
    };
    std::string playerName = " ";
    int playerIndex;
    int startingPoint;
    int homePoint;
    int homeEnterancePoint;
    int homeEnd;
    int currentDiceRolled = 0;
    int pawnPickedIndex = 0;
    int playerPoints = 0;
    int finishCounter = 0;
    int pawnsEaten = 0;
    std::vector<Pawn> &pawns;
};

struct PlayerForSort
{
    std::string playerName = " ";
    int playerPoints;
    int pawnsEaten;
};

#endif