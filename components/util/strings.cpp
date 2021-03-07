//
// Created by Suraj Lyons on 01/03/2021.
//

#include "strings.h"
#include <string>

std::regex BATTLESHIP_INPUT_NOTATION = std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)");

/**
 * Converts a counter to it's numerical unicode equivalent.
 * @author: https://www.techiedelight.com/convert-given-number-corresponding-excel-column-name/
 */
std::string convertIncrementingIntegerToAlpha(int counter){
    if (counter == 0){
        counter ++;
    }

    std::string result;

    while (counter > 0) {
        result = (char)(65 + (counter - 1) % 26) + result;
        counter = (counter - 1) / 26;
    }

    return result;
}

int convertAlphaToIncrementingInteger(std::string alpha){
    int c = 0;
    for (int i = 0; i < alpha.length(); i ++) {
        c = c * 26 + alpha[i] - 64;
    }
    return c;
}

std::string convertToUpperCase(std::string input){
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return input;
}

char convertToUpperCase(char &input){
    std::toupper(input);
    return input;
}

/**
 * Replaces a string with another
 *
 * @author Gauthier - https://stackoverflow.com/a/24315631/14937517
 */
std::string replaceStringOccurrences(std::string inputString, const std::string& source, const std::string& substitution){
    size_t start_pos = 0;
    while((start_pos = inputString.find(source, start_pos)) != std::string::npos) {
        inputString.replace(start_pos, source.length(), substitution);
        start_pos += substitution.length();
    }
    return inputString;
}

std::string replaceStringOccurrencesFromVector(std::string inputString, const std::vector<std::string>& sourceVec, std::vector<std::string> substitutionVec){
    std::string stringCache = std::move(inputString);
    int c = 0;

    for (auto&& a1: sourceVec){
        stringCache = replaceStringOccurrences(stringCache, a1, substitutionVec.at(c));
        c++;
    }

    return stringCache;
};

/**
 * Splits a string at it's given token.
 *
 * @author wcochran - https://stackoverflow.com/a/60782724/14937517
 */
std::vector<std::string> splitAtCharacterIntoVector(const std::string& input, char token){
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = input.find_first_not_of(token, end)) != std::string::npos) {
        end = input.find(token, start);
        strings.push_back(input.substr(start, end - start));
    }
    return strings;
}