//
// Created by Suraj Lyons on 02/03/2021.
//

#ifndef BATTLESHIPS_HITGRID_H
#define BATTLESHIPS_HITGRID_H

#include <utility>

#include "GameGrid.h"

class HitGrid : public GameGrid {
public:
    void markSuccessfulWarheadStrike(int x, int y);
    void markFailedWarheadStrike(int x, int y);
};


#endif //BATTLESHIPS_HITGRID_H
