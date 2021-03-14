//
// Created by Suraj Lyons on 14/03/2021.
//

#ifndef BATTLESHIPS_CONFIGFILEPARSER_H
#define BATTLESHIPS_CONFIGFILEPARSER_H

#include <string>
#include <map>
#include <vector>
#include <regex>

class ConfigFileParser {
public:
    std::string configFilePath;

    ConfigFileParser() = default;

    explicit ConfigFileParser(std::string configFilePath);

    ConfigFileParser parseFile();

    std::vector<std::string> getProperties(const std::string& property);

    std::string getProperty(const std::string& property);

private:
    std::map<std::string, std::vector<std::string>> associativePropertiesMap = {};
};


#endif //BATTLESHIPS_CONFIGFILEPARSER_H
