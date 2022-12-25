#ifndef DISPLAY_H
#define DISPLAY_H
    #include <stdlib.h>
    #include <unistd.h>
    #include "InitGame.h"
    #include "Constant.h"
    #include "Miscellanous.h"
    
    /// @brief Modifie le terminal pour l'adapter au jeu 
    void EditTerminal(); 

    /// @brief Menu lorsqu'on lance le programme, permet de lancer une nouvelle game, charger la précedente, ou voir l'historique.
    int MenuStartGame();

    int MenuGameHistory();

    /// @brief Menu pour choisir la partie à visionner
    int MenuHistory();


    int MenuConfirmClearHistory();

    void ConfirmClearHistory();

    

    /// @brief Menu pour choisir la difficulté 
    int MenuDifficulty();

    /// @brief Menu pour choisir la taille de la carte
    int MenuMapSize();

    /// @brief Menu lors de la fermeture de la partie
    int MenuSave();

    ///@brief attend que l'user appuis sur entrer pour continuer le process/affcihage 
    void MenuNext();

    /// @brief Affiche les touches pour les coups dispos (et leur coup en distance)
    void ShowKeyAvailable();

    /// @brief Fonction d'affichage de la Map dans le terminal
    void DisplayMap(int** matrice_Map, int int_mapSize);

    /// @brief affiche la map avec le chemin suivis par une liste passer en parametre
    /// @param p_list liste contenant le chemin à afficher
    void DisplayPathInMap(int** matric_Map, int int_mapSize, List* p_list);

    /// @brief affiche la map avec le chemin suivis par une liste passer en parametre avec des fleches correspondant a la direction
    /// @param p_list liste contenant le chemin à afficher
    void DisplayPathInMapArrow(int** matric_Map, int int_mapSize, List* p_list);

    /// @brief Affiche les informations de victoire ou de défaite
    void DisplayEndGame(int bool_victory, PlayerInfo *p_playerInfo);

    void DisplayList(List* p_list);

    /////////////////////////////////////////////////////////////////////////////
    //Fonctions de DEBUG pour dev, ne serons pas utiliser dans le process final//
    /////////////////////////////////////////////////////////////////////////////

    void DebugDisplayMap(int** matrice_Map, int int_mapSize);

    void DebugDisplayMatriceDistance(int int_mapSize, int*** matrice_Distance);

    /// @brief Affiche toutes les informations à propos du personnage
    void DebugInfoPlayer(PlayerInfo s_playerInfo);

    void PrintList(List* p_list);

    void PrintFirstList(List* p_list);

#endif