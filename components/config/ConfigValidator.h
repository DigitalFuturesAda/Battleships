//
// Created by Suraj Lyons on 14/03/2021.
//

#ifndef BATTLESHIPS_CONFIGVALIDATOR_H
#define BATTLESHIPS_CONFIGVALIDATOR_H

#include <utility>

#include "ConfigFileParser.h"

struct configBoardDimensions {
    int width = 0;
    int height = 0;

    configBoardDimensions(int width, int height) : width(width), height(height) {}
};

struct configShipInventory {
    std::string shipType;
    int health;
    int amountOfShips;

    configShipInventory(std::string shipType, int health, int amountOfShips) : shipType(std::move(shipType)), health(health),
                                                                               amountOfShips(amountOfShips) {}
};

class ConfigValidator {
public:
    std::string configFilePath;

    ConfigValidator() = default;

    explicit ConfigValidator(const std::string &configFilePath);

    configBoardDimensions getBoardDimensions();

    std::vector<configShipInventory> getShipInventory();

    configShipInventory matchShipInventory(const std::string& shipNotation);

    std::map<std::string, int> getShipHealthMap();

private:
    ConfigFileParser configFileParser;

    std::map<std::string, int> shipNameToHealth = {};
};


#endif //BATTLESHIPS_CONFIGVALIDATOR_H
