#include <iostream>
#include "components/grid/GameGrid.h"

int main() {
    GameGrid battleshipGameGrid;

    battleshipGameGrid.attemptPlacement(5, 5, DESTROYER, VERTICAL);
    battleshipGameGrid.attemptPlacement(6, 5, CARRIER, VERTICAL);

    battleshipGameGrid.printGrid();

    return 0;
}
