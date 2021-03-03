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

std::string getRegexInputWithPrompt(const std::string& prompt, const std::regex& regex) {
    std::string input = getStringWithPrompt(prompt);

    while (!regex_match(input, regex)) {
        input = getStringWithPrompt(prompt);
    }

    return input;
}
