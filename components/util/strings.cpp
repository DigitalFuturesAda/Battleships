//
// Created by Suraj Lyons on 01/03/2021.
//

#include "strings.h"
#include <iostream>

/**
 * Converts a counter to it's numerical unicode equivalent.
 * @author: https://www.techiedelight.com/convert-given-number-corresponding-excel-column-name/
 */
std::string convertIncrementingIntegerToAlpha(int counter){
    std::string result;

    while (counter > 0){
        result += char((counter - 1) % 26 + 'A') + result;
        counter = (counter - 1) / 26;
    }

    return result;
}
