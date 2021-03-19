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
                MenuGameConfiguration(PLAYER, COMPUTER, false, false)
            },
            {
                "Player vs Player",
                MenuGameConfiguration(PLAYER, PLAYER, false, false)
            },
            {
                "Player vs Computer Salvo game mode",
                MenuGameConfiguration(PLAYER, COMPUTER, true, false)
            },
            {
                "Player vs Player Salvo game mode",
                MenuGameConfiguration(PLAYER, PLAYER, true, false)
            },
            {
                "Player vs Computer Hidden Mines game mode",
                MenuGameConfiguration(PLAYER, COMPUTER, false, true)
            },
            {
                "Player vs Player Hidden Mines game mode",
                MenuGameConfiguration(PLAYER, PLAYER, false, true)
            },
            {
                "Computer vs Computer simulation Hidden Mines game mode",
                MenuGameConfiguration(COMPUTER, COMPUTER, false, true)
            },
            {
                "Quit", MenuGameConfiguration::ofEmpty()
            }
    };

    clearConsole();

    std::cout << "\033[1;31m···\033[0m Welcome to Battleships \033[1;31m···\033[0m" << std::endl;
    std::cout << "\033[1;31m···\033[0m Please select one of the following options by entering the corresponding number:" << std::endl;

    int counter = 1;
    for (auto&& configuration : gameConfiguration){
        std::cout << "    \033[1;31m[" << counter << "]:\033[0m " << configuration.first << std::endl;
        counter ++;
    }

    std::cout << std::endl;
    regexMatch numberInput = getRegexInputWithPromptAsRegex("Select one of the options: ",
                                                            std::regex("^([1-" + std::to_string(gameConfiguration.size()) + "])"));

    gameConfigurationCache = gameConfiguration.at(stoi(numberInput.match) - 1).second;
}

MenuGameConfiguration MenuHelper::getGameConfiguration() {
    return gameConfigurationCache;
}
