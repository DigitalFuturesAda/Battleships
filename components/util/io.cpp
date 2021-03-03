//
// Created by Suraj Lyons on 02/03/2021.
//

#include "io.h"

std::string getInput() {
    std::string input;
    std::getline(std::cin, input);

    return input;
}

std::string getStringWithPrompt(const std::string& prompt) {
    std::cout << prompt;
    return getInput();
}

std::string getRegexInputWithPromptAsString(const std::string& prompt, const std::regex& regex) {
    std::string input = getStringWithPrompt(prompt);

    while (!regex_match(input, regex)) {
        input = getStringWithPrompt(prompt);
    }

    return input;
}

regexMatch getRegexInputWithPromptAsRegex(const std::string& prompt, const std::regex& regex) {
    std::string string = getRegexInputWithPromptAsString(prompt, regex);

    std::smatch match;
    regexMatch result;
    std::regex_search(string, match, regex);

    int i = 0;
    for (auto matches: match){
        if (i == 0){
            result.match = matches;
        } else {
            result.matches.push_back(matches);
        }
        i++;
    }

    return result;
}