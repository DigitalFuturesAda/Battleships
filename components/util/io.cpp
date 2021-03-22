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
    std::cout << "\033[1;33m" << "[INPUT]: " << "\033[0m";
    std::cout << prompt;
    return getInput();
}

std::string getRegexInputWithPromptAsString(const std::string& prompt, const std::regex& regex) {
    std::string input = getStringWithPrompt(prompt);

    while (!regex_match(input, regex)) {
        displayError("Invalid input! - ", 1);
        input = getStringWithPrompt(prompt);
    }

    return input;
}

regexMatch getRegexMatchWithString(const std::string& string, const std::regex& regex){
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

regexMatch getRegexInputWithPromptAsRegex(const std::string& prompt, const std::regex& regex) {
    std::string string = getRegexInputWithPromptAsString(prompt, regex);

    return getRegexMatchWithString(string, regex);
}

void displayError(const std::string error, int pruneMessagesAmount){
    for (int i = 0; i < pruneMessagesAmount; i++){
        std::cout << "\e[1A\e[K";
    }
    std::cout << "\033[1;31m" << error << "\033[0m";
}

void displayInformation(const std::string information, int pruneMessagesAmount){
    for (int i = 0; i < pruneMessagesAmount; i++){
        std::cout << "\e[1A\e[K";
    }
    std::cout << "\033[1;33m" << information << "\033[0m";
}

void clearConsole() {
    std::cout << "\x1B[2J\x1B[H";
}

void displayBlankInputDialog() {
    std::string input = getStringWithPrompt("Press enter to continue... ");
}

void displayContinueGameConfirmationDialogWithPrompt(GameFlowController *gameFlowController, const std::string& prompt) {
    std::string input = getRegexInputWithPromptAsString(prompt, std::regex("$^|^([Rr]{1}|[Qq]{1})"));

    if (input.empty()){
        return;
    }

    char letter = convertToUpperCase(input).at(0);
    if (letter == 'R'){
        gameFlowController->setUserRequestToRestart();
    } else if (letter == 'Q'){
        displayInformation("Quitting program. Thanks for using!\n", 1);
        exit (EXIT_SUCCESS);
    }
}

void displayContinueGameConfirmationDialog(GameFlowController *gameFlowController) {
    displayContinueGameConfirmationDialogWithPrompt(gameFlowController, "Press enter to continue \033[1;33m[Q to quit or R to reset]:\033[0m ");
}

void clearBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}