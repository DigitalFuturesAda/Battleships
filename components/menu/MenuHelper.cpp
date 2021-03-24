//
// Created by Suraj Lyons on 16/03/2021.
//

#include "MenuHelper.h"
#include "../util/io.h"
#include <iostream>
#include <map>
#include <vector>

MenuHelper::MenuHelper() = default;

void MenuHelper::renderMenu() {
    std::vector<std::pair<std::string, MenuGameConfiguration>> gameConfiguration = {
            {
                    "Player vs Computer",
                    MenuGameConfiguration(PLAYER, COMPUTER, false, false, false)
            },
            {
                    "Player vs Player",
                    MenuGameConfiguration(PLAYER, PLAYER, false, false, false)
            },
            {
                    "Player vs Computer Salvo game mode",
                    MenuGameConfiguration(PLAYER, COMPUTER, true, false, false)
            },
            {
                    "Player vs Player Salvo game mode",
                    MenuGameConfiguration(PLAYER, PLAYER, true, false, false)
            },
            {
                    "Player vs Computer Hidden Mines game mode",
                    MenuGameConfiguration(PLAYER, COMPUTER, false, true, false)
            },
            {
                    "Player vs Player Hidden Mines game mode",
                    MenuGameConfiguration(PLAYER, PLAYER, false, true, false)
            },
            {
                    "Computer simulation Hidden Mines game mode",
                    MenuGameConfiguration(COMPUTER, COMPUTER, false, true, false)
            },
            {
                    "Player vs Computer with enhanced algorithm",
                    MenuGameConfiguration(PLAYER, COMPUTER, false, false, true)
            },
            {
                    "Computer simulation with enhanced algorithm",
                    MenuGameConfiguration(COMPUTER, COMPUTER, false, false, true)
            },
            {
                    "Quit", MenuGameConfiguration::ofEmpty()
            },
    };

    clearConsole();

    std::cout << "\033[1;31m__________         __    __  .__                .__    .__              \n"
                 "\\______   \\_____ _/  |__/  |_|  |   ____   _____|  |__ |__|_____  ______\n"
                 " |    |  _/\\__  \\\\   __\\   __\\  | _/ __ \\ /  ___/  |  \\|  \\____ \\/  ___/\n"
                 " |    |   \\ / __ \\|  |  |  | |  |_\\  ___/ \\___ \\|   Y  \\  |  |_> >___ \\ \n"
                 " |______  /(____  /__|  |__| |____/\\___  >____  >___|  /__|   __/____  >\n"
                 "        \\/      \\/                     \\/     \\/     \\/   |__|       \\/\033[0m"
                 << std::endl
                 << std::endl;

    std::cout << "\033[1;31m···\033[0m Welcome to Battleships \033[1;31m···\033[0m" << std::endl;
    std::cout << "\033[1;31m···\033[0m Please select one of the following options by entering the corresponding number:" << std::endl;

    int counter = 1;
    for (auto&& configuration : gameConfiguration){
        std::cout << "    \033[1;31m[" << counter << "]:\033[0m " << configuration.first << std::endl;
        counter ++;
    }

    std::string numberInput;

    std::cout << std::endl;
    while (true){
        numberInput = getStringWithPrompt("Select one of the options: ");
        int intInput;

        try {
            intInput = std::stoi(numberInput);
        } catch (std::exception const & e) {
            displayError("Invalid option - ", 1);
        }

        if (intInput >= 1 && intInput <= gameConfiguration.size()){
            gameConfigurationCache = gameConfiguration.at(intInput - 1).second;
            return;
        } else {
            displayError("Invalid number - ", 1);
        }
    }
}

MenuGameConfiguration MenuHelper::getGameConfiguration() {
    return gameConfigurationCache;
}
