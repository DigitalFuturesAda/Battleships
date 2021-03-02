#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    tabulate::Table battleshipGameTable;
    battleshipGameTable.add_row({"Game board", "Hit board"});

    Player humanPlayer;
    Player computerPlayer;

    humanPlayer.setOpposingPlayer(&computerPlayer);
    computerPlayer.setOpposingPlayer(&humanPlayer);

    attemptPlacementResponse placeShipRequest = humanPlayer.attemptToDeployShip(CARRIER, "E", 5, VERTICAL);

//    // Computer: Deploy some ships
//    computerPlayer.getGameGrid()->attemptPlacement("E", 5, CARRIER, VERTICAL);
//    computerPlayer.getGameGrid()->attemptPlacement("A", 2, BATTLESHIP, HORIZONTAL);
//    computerPlayer.getGameGrid()->attemptPlacement("H", 7, PATROL, HORIZONTAL);
//
//    // Human: Fire warheads at ships
//    humanPlayer.fireWarheadStrikeAtOpposingPlayer("E", 5); // Hit the Aircraft Carrier
//    humanPlayer.fireWarheadStrikeAtOpposingPlayer("E", 6); // ^
//
//    humanPlayer.fireWarheadStrikeAtOpposingPlayer("B", 2); // Missed warhead strikes
//    humanPlayer.fireWarheadStrikeAtOpposingPlayer("B", 3); // ^
//    humanPlayer.fireWarheadStrikeAtOpposingPlayer("C", 5); // ^
//
//    // Any logic called after this will not be displayed in the board.
    battleshipGameTable.add_row({humanPlayer.getGameGrid()->renderGrid(), humanPlayer.getHitGrid()->renderGrid()});
    battleshipGameTable.add_row({humanPlayer.getStationaryShips(), humanPlayer.getDeployedShips()});
    battleshipGameTable.column(0).format().width(GameGrid::getObservableGridWidth());
    battleshipGameTable.column(1).format().width(HitGrid::getObservableGridWidth());
//
//    // Output Computer board
//    battleshipGameTable.add_row({computerPlayer.getGameGrid()->renderGrid(), computerPlayer.getHitGrid()->renderGrid()});

    std::cout << battleshipGameTable << std::endl;
}
