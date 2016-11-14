#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

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
#include <vector>
#include <time.h>
#include <cstring>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

#define THRESHOLD 0
//! demi-pente de la sigmoide à l'origine
#define PENTE 1
//! taux d'apprentissage 0<MU<1
#define MU 0.9
//! nombre de neurones de la première couche
#define FIRST_LAYER_SIZE 400
//! nombe de neurones de la dernière couche
#define LAST_LAYER_SIZE 1
//! momentum, quantité d'inertie
#define ALPHA 0.05
//! longueur maximale d'un nom de fichier
#define MAX_LENGTH_NAME_FILE 100
//! période temporelle d'affichage des informations sur l'apprentissage en cour
#define NB_LEARNING 10
//! valeur par defaut de la distance maximale sur les exemples que peut avoir un reseau en sortie d'un apprentissage productif, modife par NetworkArray::setOptions
#define MAXIMAL_DISTANCE 0.05
//! valeur par defaut du nombre maximal de boucles d'apprentissage a effectuer en un apprentissage, modife par  NetworkArray::setOptions
#define MAX_LIMIT_LOOP 500
//! valeur par defaut de la longueur de l'alphabet, modife par NetworkArray::setOptions
#define LENGTH_ALPHABET 52
//! valeur par defaut de la longueur de l'alphabet, modife par NetworkArray::setOptions
#define LENGTH_ALPHABET 52
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
//! Length of inputs in topology module
//* The image is of format TOPOLOGY_INPUT_SIZE*TOPOLOGY_INPUT_SIZE
#define TOPOLOGY_INPUT_SIZE 10

//! dossier contenant les polices
#define DOSSIERPOLICES "fonts/"
//! Le sous-dossier où il faudra placer les images
#define DOSSIERIMAGES "images/"
//! nom du fichier de configuratio
#define NAME_CONFIG_FILE "NeuronProject.cfg"
//! nom du dossier ou sont stockes les images de test
#define DOSSIERTEST "test/"
//! nom du dossier ou sont stockes les textes extraits des images de test
#define DOSSIERTESTTEXT "test_texts/"
//! liste des caracteres pour la database : TODO a supprimer
#define CHARSBIS { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" }
//! liste des caracteres
#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,?;.:!éàè'()+-\"="

//! Le sous-dossier où seront créés les textes
#define DOSSIERTEXTES "texts/"
//! Le dossier ou seront stockes les sauvegardes reseau
#define DOSSIER_SVG "svg_reseau/"
//! Le nom du fichier pointant vers la derniere sauvegarde
#define NOM_SVG "plus_recent_"
//! L'extension dediee aux sauvegardes reseau
#define EXTENSION_SVG ".svg_reseau"


#include "menu.h"
#include "system.h"
#include "NetworkArray.h"
#include "Neuron.h"
#include "Layer.h"
#include "filtres2.h"
#include "database.h"
#include "buttons.h"
#include "Network.h"
#include "Binding.h"
#include "script.h"
#include "topology.h"

using namespace std;

#endif  //CONFIG_H_INCLUDED
