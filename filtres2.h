/*
--- PROGRAMME DE FILTRES --- VERSION 2.0 ---

Appeller la fonction filtres() : parcourt tout le dossier d'images et filtre chaque image
Appeller la fonction filtres_indiv(nomImage) : filtre uniquement l'image qui a pour nom nomImage

Placer toutes les images dans le sous répertoire "images", et les fichiers textes correspondants seront créés dans le sous répertoire "texte"
Temps de calcul : environ 8 mn pour 100 images en 3000x3000 sur ma machine

Transforme une image dont la largeur/hauteur est inférieure à 10000 pixels en un fichier texte contenant des réels de [-1, 1]
Dans le fichier texte : une ligne correspond à une ligne de pixels de l'image synthétisée. Chaque nombre est séparé par un espace et possède deux décimales après la virgule (ajustable)
On revient à la ligne avant d'écrire une nouvelle ligne correspondant à la ligne suivante de pixels

Le programme peut traiter des images de n'importe quelle couleur
Les -1 correspondent à la couleur de référence, qui correspond à celle du pixel de coordonnées (0,0)
Les 1 correspondent aux pixels dont la couleur est la plus "éloignée" de la couleur de référence, généralement les pixels qui composent la lettre
Les nombres entre -1 et 1 correspondent à des nuances entre la couleur de référence et la couleur la plus éloignée

Pour compiler sur Linux, ajouter `pkg-config --cflags --libs sdl` -lSDL_image (testé pour g++)

*/
#ifndef FILTRES2_H_INCLUDED
#define FILTRES2_H_INCLUDED

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

#define TAILLE 20                //La taille du fichier .txt créé pour chaque image : actuellement, 20 x 20 = 400 nombres entre 0 et 1
#define DOSSIERIMAGES "images/"  //Le sous-dossier où il faudra placer les images
#define DOSSIERTEXTES "texts/"  //Le sous-dossier où seront créés les textes
#define TAILLEMAX 10000

using namespace std;

//void filtres(); //Fonction à appeler pour filtrer tout le dossier d'images
//void filtres(const char* repertory); //Fonction à appeler pour filtrer un dossier spécifique
//void filtres(const char* repertory, const char* repertory_arr); //Fonction à appeler pour filtrer un dossier spécifique
void filtres(const char* repertory_dep = DOSSIERIMAGES, const char* repertory_arr = DOSSIERTEXTES, bool selectif = false); //Fonction à appeler pour filtrer un dossier spécifique avec sélection des images non déja filtrées ou non
void filtres_indiv(SDL_Surface *lettre, char* pathTxt, int **pixelsR, int **pixelsG, int **pixelsB); //Fonction appellée par filtres pour chaque image, correspond à la quasi totalité de l'ancienne fonction main

void pathNames(char* pathPng, char* pathTxt, char* namePng, const char* repertory_dep, const char* repertory_arr);
Uint32 getPixel(SDL_Surface * surface, int x, int y);   //Renvoie un Uint32 contenant les composantes en rouge, vert, bleu du pixel de coordonnées x, y de l'image chargée sur surface
void margeLigne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[]);  //Délimite l'espace que l'on peut rogner en haut et en droite
void margeColonne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[]);    // Idem, en gauche et à droite
bool margeSynthese(int imageWidth, int imageHeight, int marges[]);
bool dejaFiltree(const char* repertory_arr, const char* imageName);
void analysePixel(SDL_Surface *image, int **pixelsR, int **pixelsG, int **pixelsB); // Réalise une analyse pixel par pixel de image et stocke les composantes dans les 3 tableaux donnés
void distanceChro(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int distancemax[]);
void resize(SDL_Surface *lettre, SDL_Surface *resized, int reference[], int marges[]);
void ecritureTxt(FILE* fichier, SDL_Surface *resized, int reference[], int marges[], int distancemax[], bool notEmpty);

//void pixelsfinal(int **pixelsR, int pixelsRfinal[20][20], int Largeur, int Hauteur, int marges[]);  //Transforme le tableau correspondant à l'image rognée au maximum (en conservant une forme carrée) en un tableau 20x20 (moyenne pondérée)

#endif
