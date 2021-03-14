//
// Created by Suraj Lyons on 14/03/2021.
//

#include "ConfigSingleton.h"

#include <utility>

ConfigSingleton* ConfigSingleton::instance = nullptr;

ConfigSingleton* ConfigSingleton::getInstance() {
    if (instance == nullptr) {
        instance = new ConfigSingleton();
    }
    return instance;
}

void ConfigSingleton::setValidator(ConfigValidator validator) {
    configValidator = std::move(validator);
}

ConfigValidator ConfigSingleton::getValidator() {
    return configValidator;
}
