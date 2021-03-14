//
// Created by Suraj Lyons on 14/03/2021.
//

#include "ConfigFileParser.h"
#include "../util/io.h"

#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <absl/strings/ascii.h>
#include <map>

ConfigFileParser ConfigFileParser::parseFile() {
    std::regex iniNotation = std::regex("([^\\s]+)[ ]?=\\s?(.*[^=])");

    std::ifstream configurationFile;
    configurationFile.open(configFilePath);

    std::string line;

    while (!configurationFile.eof()) {
        getline(configurationFile, line);
        absl::string_view whiteSpaceStripped = absl::StripLeadingAsciiWhitespace(line);

        // Restart the loop if the leading line if a comment or empty (new line)
        if (whiteSpaceStripped.empty() || whiteSpaceStripped.at(0) == ';'){
            continue;
        }

        if (!regex_match(line, iniNotation)) {
            displayError("Failed to parse file - line: '" + line + "' does not conform to the ini format specification");
            exit(EXIT_FAILURE);
        }

        regexMatch match = getRegexMatchWithString(line, iniNotation);

        std::string property = match.matches.at(0);
        std::string value = match.matches.at(1);

        if (absl::StripAsciiWhitespace(value).empty()){
            displayError("Failed to parse file - The property: '" + match.matches.at(0) + "' has an empty value");
            exit(EXIT_FAILURE);
        }

        if (associativePropertiesMap.find(property) == associativePropertiesMap.end()){
            associativePropertiesMap.insert(std::pair<std::string, std::vector<std::string>>{
                property, {value}
            });
        } else {
            associativePropertiesMap.at(property).emplace_back(value);
        }
    }
    configurationFile.close();

    return *this;
}

std::map<std::string, std::vector<std::string>> ConfigFileParser::getAssociativeProperties() {
    return associativePropertiesMap;
}

ConfigFileParser::ConfigFileParser(std::string configFilePath) : configFilePath(std::move(configFilePath)) {}
