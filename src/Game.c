#include "Game.h"

void SetupGame()
{
    EditTerminal();
    InitRNG();
    SupprFile(CURRENT_GAME_CSV);
}

void BeforeTurn(int** matrice_Map, int*** matrice_Distance, int int_mapSize, PlayerInfo* p_playerInfo, List* p_list)
{
    ClearTerm();
    if(p_list == NULL){
        DisplayMap(matrice_Map, int_mapSize);
    } else {
        DisplayPathInMap(matrice_Map, int_mapSize, p_list);
    }
    printf("Votre energie : %d\n",p_playerInfo->energy);
    ShowKeyAvailable(p_playerInfo, matrice_Distance);
}

void ErrorStepBack(int int_error)
{
    if(int_error == 1){
        printf("\nVous n'avez plus de retour en arrière possible.\n");
    } else if(int_error == 2){
        // PrintList(p_list);
        printf("\nVous ne pouvez pas revenir plus en arrière.\n");
    }
    return;
}

int BeforeStepBack(PlayerInfo* p_playerInfo, List* p_list)
{
    if(p_playerInfo->backward <= 0){
        return (1);
    } else if(LengthList(p_list) <= 1){
        return (2);
    } else {
        return (0);
    }
}

void Game()
{
    int key_pressed, int_wanted_x, int_wanted_y, int_error;
    int bool_victory = 0;
    int choice;
    RemoveNode(GameInfo.p_listpath);
    AddNode(GameInfo.p_listpath,  GameInfo.s_playerInfo.coordonnees, 0);

    BeforeTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.int_mapSize, &GameInfo.s_playerInfo, GameInfo.p_listpath);
    while( !bool_victory && GameInfo.s_playerInfo.energy > 0){
        StockCurrentTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.p_listpath, GameInfo.int_mapSize, &GameInfo.s_playerInfo);
        key_pressed = ListenKeyboard();
        ChangePosition(key_pressed, &int_wanted_x, &int_wanted_y, &GameInfo.s_playerInfo); 
        if(key_pressed == STEP_BACK){
            int_error = BeforeStepBack(&GameInfo.s_playerInfo, GameInfo.p_listpath);
            if(int_error != 0){
                ErrorStepBack(int_error);
            }else{
                StepBack(GameInfo.p_listpath, GameInfo.matrice_Map, &GameInfo.s_playerInfo, GameInfo.matrice_Distance);
                BeforeTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.int_mapSize, &GameInfo.s_playerInfo, GameInfo.p_listpath);
            }
        } else if(key_pressed == LEAVE){
            choice = MenuSave();
            if (choice == 0) 
                Quit(); //l'utilisateur a choisi de partir sans sauvegarder.
            else if (choice == 1)
                SaveAndQuit();//fonction Fabien sauvegarde; 
            else //choice ==2 l'utilisateur veut revenir jouer sur sa game
                BeforeTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.int_mapSize, &GameInfo.s_playerInfo, GameInfo.p_listpath);
       } else {
            GameInfo.matrice_Map = AfterMovement(GameInfo.matrice_Map, int_wanted_x, int_wanted_y, &GameInfo.s_playerInfo, GameInfo.int_mapSize, &bool_victory, GameInfo.p_listpath, GameInfo.matrice_Distance);
            printf("\n"); 
            BeforeTurn(GameInfo.matrice_Map, GameInfo.matrice_Distance, GameInfo.int_mapSize, &GameInfo.s_playerInfo, GameInfo.p_listpath);
        }
    }
    GameInfo.bool_victory=bool_victory;
    return;
}

void FreeGame()
{
    FreeList(GameInfo.p_listpath);
    FreeList(GameInfo.p_listBestWay);
    UnallocMatriceMap(GameInfo.matrice_Map, GameInfo.int_mapSize);
    UnallocMatriceDistance(GameInfo.matrice_Distance, GameInfo.int_mapSize);
}


void Quit()
{
    FreeGame();
    exit(EXIT_SUCCESS);
}

void SaveAndQuit()
{
    Save(GameInfo.int_mapSize);
    Quit();
}

void QuitFail(const char* str_errMsg){
    puts(str_errMsg);
    FreeGame();
    exit(EXIT_FAILURE);
}

void NewGame()
{
     //Choix des parametre de la game
    GameInfo.float_diffRate = ChooseDifficulty();
    GameInfo.int_mapSize = ChooseMapSize();

    //Initialisation de la structure Joueur et du stockage du chemin
    GameInfo.s_playerInfo = SetupPlayer(); 
    InitEnergy(&GameInfo.s_playerInfo, GameInfo.int_mapSize);
    GameInfo.p_listpath = InitList(GameInfo.s_playerInfo.coordonnees, 0);

    //Creation des structures contenant les infos de la carte
    GameInfo.matrice_Map = InitMap(GameInfo.int_mapSize, GameInfo.float_diffRate, &GameInfo.s_playerInfo);
    GameInfo.matrice_Distance = InitDistance(GameInfo.int_mapSize);
}

void ReloadSave(int*** matrice_Map, int**** matrice_Distance, PlayerInfo* s_playerInfo, int int_mapSize)
{
    SaveToCurrentGame();
    RestoreTurn(SavedTurnsCount(int_mapSize, CURRENT_GAME_CSV)+1, matrice_Map, matrice_Distance, s_playerInfo, int_mapSize, SAVE_CSV );
}

void ResumeGame()
{
    if(!ExistFile(SAVE_CSV)){ //Ajouter une verif si le fichier est vide
        puts("File Save.csv not found or empty");
        exit(EXIT_FAILURE);
    }   
    GameInfo.int_mapSize = RestoreMapSize();
    GameInfo.matrice_Map = AllocMatriceMap(GameInfo.int_mapSize);
    GameInfo.matrice_Distance = AllocMatriceDistance(GameInfo.int_mapSize);
    ReloadSave(
                &GameInfo.matrice_Map,
                &GameInfo.matrice_Distance,
                &GameInfo.s_playerInfo,
                GameInfo.int_mapSize
                );
    GameInfo.p_listpath = RestoreList(GameInfo.int_mapSize, SavedTurnsCount(GameInfo.int_mapSize, CURRENT_GAME_CSV)+1);
}

void EndGame()
{
    ClearTerm();
    DisplayEndGame(GameInfo.bool_victory, &GameInfo.s_playerInfo);
    printf("The Path you followed: ");
    DisplayList(InvertList(GameInfo.p_listpath));
    DisplayPathInMapArrow(GameInfo.matrice_Map, GameInfo.int_mapSize, InvertList(GameInfo.p_listpath));
    puts("");
    //recherche et affichage du meilleur chemin en terme de distance
    RestoreTurn(1 ,&GameInfo.matrice_Map, &GameInfo.matrice_Distance, &GameInfo.s_playerInfo, GameInfo.int_mapSize, CURRENT_GAME_CSV);
    GameInfo.matrice_Map = RestoreMap(GameInfo.matrice_Map, GameInfo.int_mapSize,1, CURRENT_GAME_CSV);
    GameInfo.p_listBestWay = AlgoDijkstra(GameInfo.matrice_Map,
                                          GameInfo.matrice_Distance,
                                          &GameInfo.s_playerInfo, 
                                          GameInfo.int_mapSize
                                          );
    // RestoreDistance(GameInfo.matrice_Distance, GameInfo.int_mapSize,1,CURRENT_GAME_CSV)
    printf("The shortest Way: ");
    DisplayList(GameInfo.p_listBestWay);
    printf("Total distance: %d\n", GameInfo.p_listBestWay->firstnode->is_bonus);
    DisplayPathInMapArrow(GameInfo.matrice_Map,GameInfo.int_mapSize,InvertList(GameInfo.p_listBestWay));
    
}

void InitGame()
{
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
}