//
// Created by Suraj Lyons on 04/03/2021.
//

#ifndef BATTLESHIPS_GAMEFLOWCONTROLLER_H
#define BATTLESHIPS_GAMEFLOWCONTROLLER_H


class GameFlowController {
private:
    bool hasUserRequestedToRestartBool = false;

public:
    bool hasUserRequestedToRestart();

    void setUserRequestToRestart();
};


#endif //BATTLESHIPS_GAMEFLOWCONTROLLER_H
