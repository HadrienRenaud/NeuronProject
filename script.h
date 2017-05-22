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

//! Fonction récupérant les valeurs par défaut, stockées dans le fichier de config
int* getConfigValue(int* length_alphabet, double* mu, double* maximal_distance, double* momentum, int* layerNb, int sizes[]);

//! Lit un exemple :
//! L'exemple est donne comme son nom de fichier `nom_fichier` et l'entrée recuperee sera sur `entrees`
bool readExemple(char* nom_fichier, double entrees[], int taille_entree, std::string directory = DOSSIERTEXTES);

//! Compte les exemples dans directory
int  countExemples(std::string directory = DOSSIERTEXTES);

//! Recupere les noms des fichiers dans un dossier, les met dans le `tabloFichiers` passe en argument
int getArrayOfFileNames(char** tabloFichier, std::string directory = DOSSIERTEXTES);

//! Récupère tous les tableaux d'exemples
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, std::string directory = DOSSIERTEXTES);


#endif // SCRIPT_H_INCLUDED
