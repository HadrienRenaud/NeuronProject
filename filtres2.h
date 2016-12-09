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
 */

#ifndef FILTRES2_H_INCLUDED
#define FILTRES2_H_INCLUDED

#include "config.h"

using namespace std;


// FONCTIONS NON UTILISÉES
//void filtres(); //Fonction à appeler pour filtrer tout le dossier d'images
//void filtres(const char* repertory); //Fonction à appeler pour filtrer un dossier spécifique
//void filtres(const char* repertory, const char* repertory_arr); //Fonction à appeler pour filtrer un dossier spécifique
//void pixelsfinal(int **pixelsR, int pixelsRfinal[20][20], int Largeur, int Hauteur, int marges[]);  //Transforme le tableau correspondant à l'image rognée au maximum (en conservant une forme carrée) en un tableau 20x20 (moyenne pondérée)


/*! Filtre un dossier.
    \param repertory_dep le dossier à filtrer, contient les images.
    \param repertory_arr le dossier où seront laissés les fichiers txt créés.
    \param selectif indique si l'on souhaite vérifier qu'une image n'a pas déjà été filtrée avant de la filtrer. Permet d'éviter d'écraser des images déja filtrées.
*/
void  filtres(const char* repertory_dep = DOSSIERIMAGES, const char* repertory_arr = DOSSIERTEXTES, bool selectif = false);

/*! Filtre une image.
    \param lettre la surface chargée de l'image à filtrer.
    \param pathTxt le chemin correspondant à l'emplacement où sera créé le fichier text.
    \param pixels trois tableaux 10000x10000 qui vont contenir les composantes rouge, verte, bleue de la lettre.
*/
void  filtres_indiv(SDL_Surface *lettre, char* pathTxt, int **pixelsR, int **pixelsG, int **pixelsB);


//! Écris les noms des chemins où aller chercher la police, l'image à créer, le txt à créer.
void  pathNames(char* pathPng, char* pathTxt, char* namePng, const char* repertory_dep, const char* repertory_arr);


/*! Extrait un pixel.
    \param surface la surface que l'on considère.
    \param x,y les coordonnées du pixel relatives à la surface.
    \return un unsigned int sur 32 bits qui correspond aux composantes rouge, verte, bleue et éventuellement l'alpha.
*/
Uint32  getPixel(SDL_Surface * surface, int x, int y);

//! Modifie une surface en y insérant un pixel donné.
void  setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);


/*! Détermine les marges que l'on peut rogner à gauche et à droite sur l'image, c'est-à-dire les premières colonnes ne contenant que des pixels de la couleur de référence.
    \param reference les couleurs du pixel de référence, par défaut le pixel tout en haut à gauche de l'image.
    \param marges les dimensions des marges.
*/
void margeLigne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[]);

//! Détermine les marges que l'on peut rogner en haut et en bas sur l'image.
void margeColonne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[]);

/*! Finalise les marges.
    \param marges les dimensions des marges.
    \return true si l'on a pu rogner l'image en une image carrée, centrée sur la lettre ; false si la lettre ou si les dimensions sont incompatibles avec la rognure.
*/
bool margeSynthese(int imageWidth, int imageHeight, int marges[]);

//! Détermine si une image a déja été filtrée, c'est-à-dire si il y a déjà un fichier de nom associé à l'image dans le dossier de destination.
bool  dejaFiltree(const char* repertory_arr, const char* imageName);

//! Extrait tous les pixels de l'image et stocke les composantes dans les tableaux pixels.
void  analysePixel(SDL_Surface *image, int **pixelsR, int **pixelsG, int **pixelsB);


//! Détermine la distance chromatique maximale au sein de l'image : quelle est la couleur du pixel le plus éloigné par sa couleur du pixel de référence ?
void distanceChro(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int distancemax[]);

//! Réduit la taille de l'image dans la surface resized. Utilise l'algorithme fourni par la SDL, qualité médiocre (réduit les nuances) mais efficace.
void resize(SDL_Surface * lettre, SDL_Surface * resized, int reference[], int marges[]);


/*! Réduit MANUELLEMENT la taille de l'image et écrit les résultats dans le fichier destination.
    \param fichier un flux ouvert sur le fichier sur lequel il faut écrire.
    \param distancemax les composantes du pixel le plus éloigné du pixel de référence.
*/
void manualResizePrinting(FILE * fichier, SDL_Surface * image, int reference[], int marges[], int distancemax[], bool notEmpty);

//! À partir de l'image réduite, calcule les rapports des distances chromatiques et écrit le résultat dans le fichier destination.
void printingTxt(FILE * fichier, SDL_Surface * resized, int reference[], int marges[], int distancemax[], bool notEmpty);

#endif
