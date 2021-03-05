//
// Created by Suraj Lyons on 01/03/2021.
//
#include <iostream>
#include <regex>
#include <algorithm>

#ifndef BATTLESHIPS_STRINGS_H
#define BATTLESHIPS_STRINGS_H

std::string convertIncrementingIntegerToAlpha(int counter);

int convertAlphaToIncrementingInteger(std::string alpha);

std::string convertToUpperCase(std::string input);

char convertToUpperCase(char &input);

std::string replaceStringOccurrences(std::string inputString, const std::string& source, const std::string& substitution);

std::string replaceStringOccurrencesFromVector(std::string inputString, const std::vector<std::string>& sourceVec, std::vector<std::string> substitutionVec);

#endif //BATTLESHIPS_STRINGS_H
