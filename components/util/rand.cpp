//
// Created by Suraj Lyons on 03/03/2021.
//

#include "rand.h"
#include <iostream>

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

//    std::cout << "Generating random between: " << min << " -> " << max << " : " << min + x % n << std::endl;
    return min + x % n;
}

/**
 * Random number between using std::mt19937
 *
 * @author Cornstalks - https://stackoverflow.com/a/13445752/14937517
 */
int randomBetween19937(int low, int high){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(low, high);

    return dist6(rng);
}