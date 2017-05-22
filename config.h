#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <cstring>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <stdexcept>


#ifndef NO_GRAPHIC
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif // NO_GRAPHIC

#define THRESHOLD 0
//! demi-pente de la sigmoide à l'origine
#define PENTE 0.67
//! taux d'apprentissage 0<MU<1
#define MU 0.5
//! momentum, quantité d'inertie
#define ALPHA 0.27

//! valeur par defaut de la longueur de l'alphabet, modife par NetworkArray::setOptions
#define LENGTH_ALPHABET 26

//! nombre de neurones de la première couche
#define FIRST_LAYER_SIZE 400
//! nombe de neurones de la dernière couche : a priori il y aura toujours égalité avec le nombre de lettres utilisées dans l'alphabet, voir plus haut.
#define LAST_LAYER_SIZE LENGTH_ALPHABET

//! longueur maximale d'un nom de fichier
#define MAX_LENGTH_NAME_FILE 100
//! période temporelle d'affichage des informations sur l'apprentissage en cours
#define NB_LEARNING 10
//! valeur par defaut de la distance maximale sur les exemples que peut avoir un reseau en sortie d'un apprentissage productif, modife par NetworkArray::setOptions
#define MAXIMAL_DISTANCE 0.15
//! valeur par defaut du nombre maximal de boucles d'apprentissage a effectuer en un apprentissage, modife par  NetworkArray::setOptions
#define MAX_LIMIT_LOOP 500
//! flotant sous lequel, lors d'un test, le reseau est considere comme repondant negativement
#define LOWER_BOUND_DISTINCTION 0.05
//! nombre de boucles d'apprentissage a partir duquel on commence a ne plus considerer la casse
#define MAX_LIMIT_CASE          10000
#define TESTING_BACKGROUND_DIMENSIONS 300
#define REFRESH_TIME          0.2
//! La taille du fichier .txt créé pour chaque image : actuellement, 20 x 20 = nombres entre 0 et 1
#define TAILLE            20
#define MANUAL_RESIZE_MULTIPLIER  4
#define TAILLEMAX         10000
#define BACKGROUNDCOLOR       255, 255, 255
#define TEXTCOLOR         0, 0, 0
#define TEXTSIZE          280

///////
#define RAND 0
#define ZERO 1
///////

//! dossier contenant les polices
#define DOSSIERPOLICES "fonts/"
//! nom du fichier de configuration
#define NAME_CONFIG_FILE "NeuronProject.cfg"
//! nom du script
#define NAME_SCRIPT_FILE "Script.txt"
//! nom du fichier contenant les erreurs
#define ERROR "error.txt"

//! Le sous-dossier où il faudra placer les images (base d'entraînement)
#define DOSSIERIMAGES "images/"
//! Le sous-dossier où seront créés les textes extraits des images (base d'entraînement)
#define DOSSIERTEXTES "texts/"
//! nom du dossier ou sont stockes les images (test)
#define DOSSIERTEST "test/"
//! nom du dossier ou sont stockes les textes extraits des images (test)
#define DOSSIERTESTTEXT "test_texts/"

//! Le dossier ou est stocké la sauvegarde du réseau la plus récente
#define DOSSIERBACKUP "backup/"
//! Le nom du dossier correspondant à la sauvegarde du réseau
#define NOMBACKUP "network_backup.svg_reseau"
//Remarque : chaque nouvel apprentissage efface la sauvegarde du précédent réseau ; une seule sauvegarde à la fois : à améliorer si possible ...

//Anciennes méthodes de sauvegarde / chargement
//! Le dossier ou seront stockes les sauvegardes reseau
#define DOSSIER_SVG "svg_reseau/"
//! Le nom du fichier pointant vers la derniere sauvegarde
#define NOM_SVG "plus_recent_"
//! L'extension dediee aux sauvegardes reseau
#define EXTENSION_SVG ".svg_reseau"

//! liste des caracteres
#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,?;.:!éàè'()+-\"="
//! liste des caracteres qui ne se ressemblent pas (ex u et U)
#define CHARS2 "abcdefghijklmnopqrstuvwxyzABDEFGHIJKLMNQRTY0123456789,?;.:!éàè'()+-\"="


#define MAX_NUMBER_LAYER 10


#include "Neuron.h"
#include "Layer.h"
#include "Network.h"
#include "Binding.h"
#include "NetworkArray.h"
//#include "ReadNetwork.h"
#include "script.h"
#include "error.h"

#ifndef NO_GRAPHIC
#include "system.h"
#include "filtres2.h"
#include "database.h"
#include "buttons.h"
#include "menu.h"
#endif // NO_GRAPHIC

using namespace std;

#endif  //CONFIG_H_INCLUDED
