//
// Created by Suraj Lyons on 14/03/2021.
//

#ifndef BATTLESHIPS_CONFIGSINGLETON_H
#define BATTLESHIPS_CONFIGSINGLETON_H


#include "ConfigValidator.h"

class ConfigSingleton {
public:
    static ConfigSingleton* getInstance();

    void setValidator(ConfigValidator validator);
    ConfigValidator getValidator();

protected:
    ConfigValidator configValidator;

private:
    static ConfigSingleton* instance;
    ConfigSingleton() = default;
    ConfigSingleton(const ConfigSingleton&);
    ConfigSingleton& operator=(const ConfigSingleton&);
};


#endif //BATTLESHIPS_CONFIGSINGLETON_H
