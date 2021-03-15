//
// Created by Suraj Lyons on 14/03/2021.
//

#include "ConfigValidator.h"
#include "../util/io.h"
#include <regex>

ConfigValidator::ConfigValidator(const std::string &configFilePath) : configFilePath(configFilePath) {
    configFileParser = ConfigFileParser(configFilePath);
    configFileParser.parseFile();

    configBoardDimensionsCache = computeAndCacheBoardDimensions();
    // Computing the ship inventory must happen AFTER computing the board dimensions
    shipInventoryCache = computeAndCacheShipInventory();
}

configBoardDimensions ConfigValidator::getBoardDimensions() {
    return configBoardDimensionsCache;
}

std::vector<configShipInventory> ConfigValidator::getShipInventory() {
    return shipInventoryCache;
}

std::map<std::string, int> ConfigValidator::getShipHealthMap() {
    return shipNameToHealth;
}

// Computers
configBoardDimensions ConfigValidator::computeAndCacheBoardDimensions() {
    std::regex boardNotation = std::regex("([0-9]{1,2})x([0-9]{1,2})");
    std::string boardDimensions = configFileParser.getProperty("Board");

    if (!regex_match(boardDimensions, boardNotation)){
        displayError("The board notation input: '" + boardDimensions + "' is not valid");
        exit(EXIT_FAILURE);
    }

    regexMatch match = getRegexMatchWithString(boardDimensions, boardNotation);

    if (match.match != boardDimensions){
        displayError(
                "The board notation input: '" + boardDimensions + "' contains additional erroneous input\n"
                + "Diff - matched: '" + match.match + "' - input: '" + boardDimensions + "'");
        exit(EXIT_FAILURE);
    }

    int width;
    int height;

    try {
        width = std::stoi(match.matches.at(0));
        height = std::stoi(match.matches.at(1));
    } catch (std::exception const & e) {
        throw std::runtime_error("Failed to convert the board dimensions to integers");
    }

    if (width < 5 || width > 80 || height < 5 || height > 80){
        displayError("The width or height of the board exceeds 80 or is less than 5");
        exit(EXIT_FAILURE);
    }

    return configBoardDimensions(width, height);
}

std::vector<configShipInventory> ConfigValidator::computeAndCacheShipInventory() {
    std::vector<std::string> shipInventory = configFileParser.getProperties("Ship");
    std::vector<configShipInventory> shipInventoryComputed = {};

    if (getBoardDimensions().height == -1 || getBoardDimensions().width == -1){
        throw std::runtime_error("Computing the ship inventory must happen AFTER computing the board dimensions");
    }

    for (auto&& shipNotation : shipInventory){
        configShipInventory shipData = matchShipInventory(shipNotation);
        if (shipData.amountOfShips > 0){
            shipInventoryComputed.emplace_back(shipData);
        }
    }

    return shipInventoryComputed;
}

// Utility - helper method
configShipInventory ConfigValidator::matchShipInventory(const std::string& shipNotation){
    std::regex shipNotationRegex = std::regex("(Carrier|Battleship|Destroyer|Submarine|Patrol Boat),\\s?(\\d+),\\s?(\\d+)");

    if (!regex_match(shipNotation, shipNotationRegex)){
        displayError("The ship notation input: '" + shipNotation + "' is not valid");
        exit(EXIT_FAILURE);
    }

    regexMatch match = getRegexMatchWithString(shipNotation, shipNotationRegex);

    std::string shipName = match.matches.at(0);
    int shipHealth;
    int amountOfShips;

    try {
        shipHealth = std::stoi(match.matches.at(1));
        amountOfShips = std::stoi(match.matches.at(2));
    } catch (std::exception const & e) {
        throw std::runtime_error("Failed to convert the ship health or amount of ships to integers");
    }

    if (shipHealth == 0){
        displayError("The ship health of the: " + shipName + " can not be 0");
        exit(EXIT_FAILURE);
    } else if (shipHealth > getBoardDimensions().width || shipHealth > getBoardDimensions().height){
        displayError(
                "The length of the: " + shipName
                + " (" + match.matches.at(1) + ")"
                + " exceeds the grid length or height");
        exit(EXIT_FAILURE);
    }

    if (shipNameToHealth.find(shipName) == shipNameToHealth.end()){
        shipNameToHealth.insert(std::pair<std::string, int>{ shipName, shipHealth });
    } else {
        displayError("The ship: " + shipName + " has duplicate entries in the configuration file");
        exit(EXIT_FAILURE);
    }

    return configShipInventory(shipName, shipHealth, amountOfShips);
}