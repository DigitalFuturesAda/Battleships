#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"
#include "components/player/HostController.h"
#include "components/player/GameFlowController.h"
#include "components/util/rand.h"
#include "components/config/ConfigFileParser.h"
#include <thread>
#include <chrono>

int main2() {
    srand( time( nullptr ) );

    GameFlowController gameFlowController;

    Player hostPlayer("Jarvis", gameFlowController);
    Player secondaryPlayer("Ultron", gameFlowController);

    secondaryPlayer.setOpposingPlayer(&hostPlayer);
    hostPlayer.setOpposingPlayer(&secondaryPlayer);

    HostController hostController(&hostPlayer, &secondaryPlayer);

    hostPlayer.deployWarshipsAutomatically();
    secondaryPlayer.deployWarshipsAutomatically();

    // Render the ship deployment UI with a sample of the board, this will be no-op if all ships are deployed.
    hostPlayer.showShipDeploymentInterface();

    // We call this afterwards, as this augments the computer board onto the player board
    hostPlayer.setPlayingAgainstComputer();
    secondaryPlayer.setPlayingAgainstComputer();

    // These calls should happen AFTER all ships have been deployed
    hostPlayer.deployMultipleRandomlyPositionedMines();
    secondaryPlayer.deployMultipleRandomlyPositionedMines();

    while (true){
        if (gameFlowController.hasUserRequestedToRestart()){
            clearConsole();
            displayInformation("Resetting board. Stand by...\n", 1);
            break;
        }

        if (hostController.hasEitherPlayerLost()){
            hostController.renderWinConditionInterface();
            break;
        } else {
            hostPlayer.renderPlayerUserInterface();
//            hostPlayer.renderSalvoWarheadStrikeInterface();

            hostPlayer.deployWarheadStrikesAutomatically();
            secondaryPlayer.deployWarheadStrikesAutomatically();
        };
    }
}

int main() {
    ConfigFileParser fileParser("config.ini");
    fileParser.parseFile();

    std::map<std::string, std::vector<std::string>> properties = fileParser.getAssociativeProperties();
    std::cout << properties.at("Board").at(0) << std::endl;

    std::cout << properties.at("Ship").at(0) << std::endl;
    std::cout << properties.at("Ship").at(1) << std::endl;
    std::cout << properties.at("Ship").at(2) << std::endl;
    std::cout << properties.at("Ship").at(3) << std::endl;

}