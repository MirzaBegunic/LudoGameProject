#include "../headers/player.hpp"
#include "../headers/diceNumberGenerator.hpp"
#include "../headers/board.hpp"

#include <map>
#include <vector>
#include <fstream>
#include <windows.h>


int rollTheDice(Player & player) {
    int diceNum = generateDiceNumber();
    player.currentDiceRolled = diceNum;
    return diceNum;
}

void updatePoints(Player & player, int amount, HWND& p1Result, HWND& p2Result, HWND& p3Result, HWND& p4Result) {
    player.playerPoints += amount;
    switch (player.playerIndex)
    {
    case 1: {
        char buf[6];
        sprintf(buf, "%d", player.playerPoints);
        SetWindowText(p1Result, buf);
        break;
    }
    case 2: {
        char buf[6];
        sprintf(buf, "%d", player.playerPoints);
        SetWindowText(p2Result, buf);
        break;
    }
    case 3: {
        char buf[6];
        sprintf(buf, "%d", player.playerPoints);
        SetWindowText(p3Result, buf);
        break;
    }

    case 4: {
        char buf[6];
        sprintf(buf, "%d", player.playerPoints);
        SetWindowText(p4Result, buf);
        break;
    }
    
    default:
        break;
    }
} 

bool calculatePlayerMove(Player & player, std::map<int, BoardField> & mapOfBoard, std::ofstream & logg, HWND& p1Result, HWND& p2Result, HWND& p3Result, HWND& p4Result) {
    if(player.playerIndex == 4) {
        int startPoint = player.startingPoint;
        int curPos = player.pawns.at(player.pawnPickedIndex-1).currentPosition;
        logg<<"curr pos "<<curPos<<"\n";
        if(curPos == 0) {
            curPos = startPoint;
        } else if(curPos+player.currentDiceRolled <= 63) {
            curPos = curPos + player.currentDiceRolled;
            if(curPos == 63) 
                player.pawns.at(player.pawnPickedIndex-1).isFinished = true;
                updatePoints(player, 100, p1Result, p2Result, p3Result, p4Result);
        } else {
            return false;
        }
        player.pawns.at(player.pawnPickedIndex-1).currentPosition = curPos;
        logg<<"Updated pos "<<player.pawns.at(player.pawnPickedIndex-1).currentPosition<<"\n";
        player.pawns.at(player.pawnPickedIndex-1).xPos = mapOfBoard[curPos].xPos;
        player.pawns.at(player.pawnPickedIndex-1).yPos = mapOfBoard[curPos].yPos;
        logg<<"x and ylon "<<player.pawns.at(player.pawnPickedIndex-1).xPos<< ":"<<player.pawns.at(player.pawnPickedIndex-1).yPos<<"\n";
        return true;
    } else {
        int curPos = player.pawns.at(player.pawnPickedIndex-1).currentPosition;
        logg<<"curr pos "<<curPos<<"\n";
        int startPoint = player.startingPoint;
        int home = player.homePoint;
        int homeEnter = player.homeEnterancePoint;
        int homeEnd = player.homeEnd;
        if(curPos == 0) {
            curPos = startPoint;

        } else if(curPos>=startPoint && curPos<home) {
            curPos = (curPos+player.currentDiceRolled)%56;
        }else if(curPos <= homeEnter) {
            if((curPos+player.currentDiceRolled) <= homeEnter) {
                curPos = curPos + player.currentDiceRolled;
            } else {
                int num = (curPos + player.currentDiceRolled) % homeEnter;
                curPos = home-1+num;
            }
        } else {
            if ((curPos + player.currentDiceRolled) <= homeEnd) {
                curPos = curPos + player.currentDiceRolled;
                if(curPos == homeEnd) {
                    player.pawns.at(player.pawnPickedIndex-1).isFinished = true;
                    updatePoints(player, 10, p1Result, p2Result, p3Result, p4Result);
                }
            } else {
                return false;
            }
        }
        logg<<"Updated pos "<<curPos<<"\n";
        player.pawns.at(player.pawnPickedIndex-1).currentPosition = curPos;
        logg<<"Updated pos in player"<<player.pawns.at(player.pawnPickedIndex-1).currentPosition<<"\n";
        player.pawns.at(player.pawnPickedIndex-1).xPos = mapOfBoard[curPos].xPos;
        player.pawns.at(player.pawnPickedIndex-1).yPos = mapOfBoard[curPos].yPos;
        logg<<"x and ylon "<<player.pawns.at(player.pawnPickedIndex-1).xPos<< ":"<<player.pawns.at(player.pawnPickedIndex-1).yPos<<"\n";
        return true;
    }

}

std::vector<Player> loadGame(
              std::vector<Pawn>& pwOne,
              std::vector<Pawn>& pwTwo,
              std::vector<Pawn>& pwThree,
              std::vector<Pawn>& pwFour,
              std::map<int, std::map<int, BoardField>>& homes) {
    for (int i=0; i<4; ++i) {
        Pawn pawn;
        pwOne.push_back(pawn);
        pwOne.at(i).currentPosition = 0;
        pwOne.at(i).number = i+1;
        pwOne.at(i).playerIndex = 1;
        pwOne.at(i).xPos = homes[1].at(i+1).xPos;
        pwOne.at(i).yPos = homes[1].at(i+1).yPos;
    }
    for (int i=0; i<4; ++i) {
        Pawn pawn;
        pwTwo.push_back(pawn);
        pwTwo.at(i).currentPosition = 0;
        pwTwo.at(i).number = i+1;
        pwTwo.at(i).playerIndex = 2;
        pwTwo.at(i).xPos = homes[2].at(i+1).xPos;
        pwTwo.at(i).yPos = homes[2].at(i+1).yPos;
    }
    for (int i=0; i<4; ++i) {
        Pawn pawn;
        pwThree.push_back(pawn);
        pwThree.at(i).currentPosition = 0;
        pwThree.at(i).number = i+1;
        pwThree.at(i).playerIndex = 3;
        pwThree.at(i).xPos = homes[3].at(i+1).xPos;
        pwThree.at(i).yPos = homes[3].at(i+1).yPos;
    }
    for (int i=0; i<4; ++i) {
        Pawn pawn;
        pwFour.push_back(pawn);
        pwFour.at(i).currentPosition = 0;
        pwFour.at(i).number = i+1;
        pwFour.at(i).playerIndex = 4;
        pwFour.at(i).xPos = homes[4].at(i+1).xPos;
        pwFour.at(i).yPos = homes[4].at(i+1).yPos;
    }

    std::vector<Player> players;
    players.push_back(Player(1,15,63,14,69,pwOne));
    players.push_back(Player(2,29,69,28,75,pwTwo));
    players.push_back(Player(3,43,75,42,81,pwThree));
    players.push_back(Player(4,1,57,56,63,pwFour));
    return players;
}

bool pawnSelection(std::ofstream& loggg, Player & player, int sxPos, int syPos, std::map<int, std::map<int, BoardField>>& mapOfPlayerHomes,std::map<int, BoardField>& mapOfBoard ) {
    for (Pawn pawn: player.pawns) {
        if(pawn.currentPosition != 0) {
            BoardField bf = mapOfBoard[pawn.currentPosition];
            if (sxPos >= bf.left && sxPos <= bf.right && syPos >= bf.top && syPos<= bf.bottom && !pawn.isFinished) {
                player.pawnPickedIndex = pawn.number;
                return true;
            }
        } 
    }
    if (player.currentDiceRolled == 6) {
        std::map<int, BoardField> home = mapOfPlayerHomes[player.playerIndex];
        for(int i=1;i<=4;++i) {
            BoardField bf = home[i];
            loggg<<bf.xPos<<" "<<bf.yPos<<"\n";
            if (sxPos >= bf.left && sxPos <= bf.right && syPos >= bf.top && syPos<= bf.bottom && player.pawns.at(i-1).currentPosition == 0 && !player.pawns.at(i-1).isFinished) {
                player.pawnPickedIndex = i;
                return true;
            }
        }
    }

    return false;
}

bool checkIfPawnsShouldBeEaten(std::vector<Player> & players, Player & currentPlayer, std::map<int, std::map<int, BoardField>>& mapOfPlayerHomes, HWND& p1Result, HWND& p2Result, HWND& p3Result, HWND& p4Result) {
    int lastPawnPosition = currentPlayer.pawns.at(currentPlayer.pawnPickedIndex-1).currentPosition;
    int pointsCounter = 0;
    
    for(int i=0; i<4; ++i) {
        if(i+1 != currentPlayer.playerIndex) {
                for (int j=0; j<4; ++j) {
                    if(players.at(i).pawns.at(j).currentPosition == lastPawnPosition) {
                        pointsCounter++;
                        players.at(i).pawns.at(j).currentPosition = 0;
                        std::map<int, BoardField> map = mapOfPlayerHomes[players.at(i).playerIndex];
                        players.at(i).pawns.at(j).xPos = map[j+1].xPos;
                        players.at(i).pawns.at(j).yPos = map[j+1].yPos;
                        break;
                    }
            }
        }
    }
    updatePoints(currentPlayer, pointsCounter * 200, p1Result, p2Result, p3Result, p4Result);
    if(pointsCounter>0) return true;
    else return false;    
}