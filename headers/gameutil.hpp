#ifndef UTIL_HPP
#define UTIL_HPP

#include "../headers/player.hpp"
#include "../headers/diceNumberGenerator.hpp"

#include <map>
#include <vector>
#include <fstream>

int rollTheDice(Player & player);
bool calculatePlayerMove(Player & player, std::map<int, BoardField> & mapOfBoard, std::ofstream & logg);
std::vector<Player> loadGame(
              std::vector<Pawn>& pwOne,
              std::vector<Pawn>& pwTwo,
              std::vector<Pawn>& pwThree,
              std::vector<Pawn>& pwFour,
              std::map<int, std::map<int, BoardField>>& homes);

bool pawnSelection(std::ofstream & logg,Player & player, int sxPos, int syPos, std::map<int, std::map<int, BoardField>>& mapOfPlayerHomes,std::map<int, BoardField>& mapOfBoard );
bool checkIfPawnsShouldBeEaten(std::vector<Player> & players, Player & currentPlayer, std::map<int, std::map<int, BoardField>>& mapOfPlayerHomes);
void updatePoints(Player & player, int amount);
#endif