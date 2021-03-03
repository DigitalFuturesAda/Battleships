//
// Created by Suraj Lyons on 02/03/2021.
//

#ifndef BATTLESHIPS_IO_H
#define BATTLESHIPS_IO_H

#include <iostream>
#include <regex>
#include "strings.h"

std::string getInput();

std::string getStringWithPrompt(const std::string& prompt);

std::string getRegexInputWithPrompt(const std::string& prompt, const std::regex& regex);

#endif //BATTLESHIPS_IO_H
