#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include "config.h"

using namespace std;

//class NetworkArray;

//! Fonction executant un script
/*!
    \param file fichier a executer
 */
void scriptFile(ifstream &input);

//! Fonction executant une liste de commandes
void commands(int nbCmds, string cmds[]);

//! Fonction récupérant les valeurs par défaut, stockées dans le fichier de config
void getConfigValue(int* length_alphabet, double* mu, double* maximal_distance, double* momentum, int* layerNb, int sizes[]);


#endif // SCRIPT_H_INCLUDED
