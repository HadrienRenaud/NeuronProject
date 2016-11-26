#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "config.h"

using namespace std;

void  database(bool sauvegarde, bool filtre, int length_alphabet = LENGTH_ALPHABET);
void  filterPngs(char* nameFont, bool sauvegarde, bool filtre, int **pixelsR, int **pixelsG, int **pixelsB, int length_alphabet = LENGTH_ALPHABET);
void  pathNames(char* pathPng, char* pathTxt, char* nameFont, const char* repertoryPng, const char* repertoryTxt, int i);

#endif  // DATABASE_H_INCLUDED
