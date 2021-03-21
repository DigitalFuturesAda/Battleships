//
// Created by Suraj Lyons on 16/03/2021.
//

#ifndef BATTLESHIPS_MENUHELPER_H
#define BATTLESHIPS_MENUHELPER_H

enum PlayerType {
    PLAYER, COMPUTER
};

struct MenuGameConfiguration {
    PlayerType playerOneType = PLAYER;
    PlayerType playerTwoType = PLAYER;

    bool salvoGameMode = false;
    bool hiddenMinesGameMode = false;
    bool enhancedAlgorithm = false;

    bool requestToQuit = false;

    MenuGameConfiguration(PlayerType playerOneType, PlayerType playerTwoType, bool salvoGameMode,
                          bool hiddenMinesGameMode, bool enhancedAlgorithm):
                            playerOneType(playerOneType), playerTwoType(playerTwoType),
                            salvoGameMode(salvoGameMode), hiddenMinesGameMode(hiddenMinesGameMode),
                            enhancedAlgorithm(enhancedAlgorithm) {}

    static MenuGameConfiguration ofEmpty(){
        return MenuGameConfiguration();
    };

    MenuGameConfiguration() : requestToQuit(true) {};
};

class MenuHelper {
public:
    MenuHelper();

    void renderMenu();

    MenuGameConfiguration getGameConfiguration();

private:
    MenuGameConfiguration gameConfigurationCache;
};


#endif //BATTLESHIPS_MENUHELPER_H
