#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    Player humanPlayer;

    tabulate::Table battleshipGameTable;

    battleshipGameTable.add_row({"Game board", "Hit board"});


//    battleshipGameTable.add_row({humanPlayer.battleshipGameGrid.getGrid(), humanPlayer.battleshipHitGrid.getGrid()});
//
//    battleshipGameTable.add_row({humanPlayer.getStationaryShips(), humanPlayer.getDeployedShips()}).format().border_color(tabulate::Color::white);
//
//    battleshipGameTable.column(0).format().width(humanPlayer.battleshipGameGrid.getObservableGridWidth());
//    battleshipGameTable.column(1).format().width(humanPlayer.battleshipHitGrid.getObservableGridWidth());

    std::cout << battleshipGameTable << std::endl;
}
