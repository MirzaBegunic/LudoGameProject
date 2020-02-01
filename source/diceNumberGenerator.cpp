#include <iostream>
#include <stdlib.h>
#include <time.h>

int generateDiceNumber() {
    int resultSet[19] = {1,1,1,2,2,3,3,3,4,4,5,5,5,6,6,6,6,6,6};
    srand(time(0));
    int index = rand() %19;
    return resultSet[index];
}