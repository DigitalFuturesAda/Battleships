//
// Created by Suraj Lyons on 03/03/2021.
//

#include "rand.h"

bool randomBool(){
    static const int shift = static_cast<int>(std::log2(RAND_MAX));
    return (rand() >> shift) & 1;
}

/**
 * Random number between.
 *
 * @author Mark Ransom - https://stackoverflow.com/a/11766794/6084329
 */
int randomBetween(int min, int max){
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do {
        x = rand();
    } while (x >= RAND_MAX - remainder);
    return min + x % n;
}