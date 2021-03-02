#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    // Testing logic
    Player humanPlayer;
    Player computerPlayer;

    humanPlayer.setOpposingPlayer(&computerPlayer);
    computerPlayer.setOpposingPlayer(&humanPlayer);

//    do {
        humanPlayer.deployShip(CARRIER, "E", 4, VERTICAL);
        computerPlayer.executeWarheadStrike("E", 5);

        computerPlayer.deployShip(CARRIER, "E", 4, VERTICAL);
        computerPlayer.deployShip(DESTROYER, "A", 3, VERTICAL);
        computerPlayer.deployShip(CARRIER, "D", 6, VERTICAL);

        computerPlayer.executeWarheadStrike("E", 5);
        computerPlayer.executeWarheadStrike("A", 3);
        computerPlayer.executeWarheadStrike("G", 5);

        // Tabulate definitions - any logic after this will not be rendered.
        tabulate::Table battleshipGameTable;
        battleshipGameTable.format()
                .border_color(tabulate::Color::white)
                .corner("⋅")
                .corner_color(tabulate::Color::red);
        battleshipGameTable.add_row({"Game board", "Hit board"});

        tabulate::Table statisticsBoard;
        statisticsBoard.format()
                .border_color(tabulate::Color::white)
                .corner("⋅")
                .corner_color(tabulate::Color::red);

        battleshipGameTable.add_row({humanPlayer.getGameGrid()->renderGrid(), humanPlayer.getHitGrid()->renderGrid()});
        battleshipGameTable.add_row({computerPlayer.getGameGrid()->renderGrid(), computerPlayer.getHitGrid()->renderGrid()});

        battleshipGameTable.column(0).format().width(GameGrid::getObservableGridWidth());
        battleshipGameTable.column(1).format().width(HitGrid::getObservableGridWidth());

        // Statistics board
        statisticsBoard.add_row({"Boat", "Status", "Remaining lives"});
        statisticsBoard.add_row({
                                        humanPlayer.getShipInformation().at(0),
                                        humanPlayer.getShipInformation().at(1),
                                        humanPlayer.getShipInformation().at(2)});

        std::cout << battleshipGameTable << std::endl;
        std::cout << statisticsBoard << std::endl;

//        std::cout << "Enter where you want to move (eg. A1 or H8): "; // [A-Z](?:[A-B])?[1-9](?:[1-10])?
//
//        std::string userInput;
//        std::cin >> userInput;
//
//        if (userInput == "exit"){
//            break;
//        }
//
//        std::cout << "\x1B[2J\x1B[H";
//    } while (true);
}
