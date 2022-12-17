#include "Game.h"

GameInformation GameInfo;

int main(void)
{   
    SetupGame();
    switch(MenuStartGame()){
        case 1:
            ResumeGame();
            break;
        case 2:
            NewGame();
            break;
        case 3:
            break;
        case 4:
            exit(EXIT_SUCCESS);
            break;
        default:
            exit(EXIT_FAILURE);
            break;
    }

    GameInfo.p_listBestWay = AlgoDijkstra(GameInfo.matrice_Map, DuplicateMatriceDistance(GameInfo.matrice_Distance, GameInfo.int_mapSize),&GameInfo.s_playerInfo, GameInfo.int_mapSize); 
   
    //Affichage d'avant Tour
    BeforeTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.int_mapSize, &GameInfo.s_playerInfo, GameInfo.p_listpath);

    //Tours du joueur
    int bool_victory = Game(GameInfo.int_mapSize, GameInfo.matrice_Map, GameInfo.matrice_Distance, &GameInfo.s_playerInfo, GameInfo.p_listpath);

    //Affichage de fin de jeu
    DisplayPathInMapArrow(GameInfo.matrice_Map, GameInfo.int_mapSize, GameInfo.p_listpath);
    DisplayEndGame(bool_victory, &GameInfo.s_playerInfo);
    //affichage du best chemin trouver
    DisplayPathInMapArrow(GameInfo.matrice_Map,GameInfo.int_mapSize,GameInfo.p_listBestWay);
    PrintList(GameInfo.p_listBestWay);

    

    //liberation des donnée allouer
    FreeGame();    
    return 0;
}
