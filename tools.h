#ifndef TOOLS_H
#define TOOLS_H

#include "config.h"


using namespace std;




//! Compte les exemples dans directory
int  countExemples(std::string directory = DOSSIERTEXTES);

//! Recupere les noms des fichiers dans un dossier, les met dans le `tabloFichiers` passe en argument
int getArrayOfFileNames(char** tabloFichier, std::string directory = DOSSIERTEXTES);

//! Récupère tous les tableaux d'exemples
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, std::string directory = DOSSIERTEXTES);


int findChar(char, const char*,int);

double abso(double d);

#endif
