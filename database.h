#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#define BACKGROUNDCOLOR 255,255,255
#define TEXTCOLOR 0,0,0
#define TEXTSIZE 280
#define CHARNUMBER 52
#define CHARS {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"}
#define DOSSIERPOLICES "fonts/"
#define DOSSIERIMAGES "images/"

void database(bool sauvegarde, bool filtre);
void filterPngs(char* file, bool sauvegarde, bool filtre, int **pixelsR, int **pixelsG, int **pixelsB);

#endif // DATABASE_H_INCLUDED
