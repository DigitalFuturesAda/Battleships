//
// Created by Suraj Lyons on 02/03/2021.
//

#include "HitGrid.h"

void HitGrid::markSuccessfulWarheadStrike(int x, int y) {
    this->attemptPlacement(x, y, VALID_HIT, VERTICAL);
}

void HitGrid::markFailedWarheadStrike(int x, int y) {
    this->attemptPlacement(x, y, INVALID_HIT, VERTICAL);
}
