#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include "config.h"

using namespace std;

class NetworkArray;

//! Fonction executant un script
/*!
    \param file fichier a executer
 */
void scriptFile(ifstream &input);

//! Fonction executant une liste de commandes
void commands(int nbCmds, string cmds[]);

//! Fonction récupérant la longueur de l'alphabet, stocké dans le fichier de config
int   getLenghtAlphabet();

#endif // SCRIPT_H_INCLUDED
