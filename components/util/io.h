//
// Created by Suraj Lyons on 02/03/2021.
//

#ifndef BATTLESHIPS_IO_H
#define BATTLESHIPS_IO_H

#include <iostream>
#include <regex>
#include "strings.h"

#include <vector>

struct regexMatch {
    std::string match;
    std::vector<std::string> matches;
};

std::string getInput();

std::string getStringWithPrompt(const std::string& prompt);

std::string getRegexInputWithPromptAsString(const std::string& prompt, const std::regex& regex);

regexMatch getRegexInputWithPromptAsRegex(const std::string& prompt, const std::regex& regex);

void clearConsole();

void displayError(const std::string error, int pruneMessagesAmount);

void displayInformation(const std::string information, int pruneMessagesAmount);

void awaitBlankInput();

#endif //BATTLESHIPS_IO_H
