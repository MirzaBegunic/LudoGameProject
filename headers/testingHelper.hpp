#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <vector>
#include <windows.h>
#include "board.hpp"
#include "player.hpp"

void testDice(HDC hdc, std::map<int, BoardField> dice);
void testFields(HDC hdc, std::map<int, BoardField> &mapOfBoard, std::map<int, std::map<int, BoardField>> &mapOfPlayerHomes);
void testLoadGame(Player playerOne,
                  Player playerTwo,
                  Player playerThree,
                  Player playerFour,
                  std::ofstream &logg);
void testMove(Player player, std::ofstream &logg);
void testPawn(HDC hdc, std::vector<Player> players);

#endif