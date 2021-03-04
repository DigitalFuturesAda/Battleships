//
// Created by Suraj Lyons on 04/03/2021.
//

#include "GameFlowController.h"
#include <iostream>

bool GameFlowController::hasUserRequestedToRestart() {
    return hasUserRequestedToRestartBool;
}

void GameFlowController::setUserRequestToRestart() {
    hasUserRequestedToRestartBool = true;
}
