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

#define THRESHOLD						0
#define PENTE							1		//demi-pente de la sigmoide à l'origine
#define MU								0.9		//taux d'apprentissage 0<MU<1
#define FIRST_LAYER_SIZE				400		//nombre de neurones de la première couche
#define LAST_LAYER_SIZE					1		//nombe de neurones de la dernière couche
#define ALPHA							0.05	//momentum, quantité d'inertie
#define MAX_LENGTH_NAME_FILE			100
#define NB_LEARNING						10
#define MAXIMAL_DISTANCE				0.05
#define MAX_LIMIT_LOOP					500
#define LENGTH_ALPHABET					52
#define LOWER_BOUND_DISTINCTION			0.05
#define MAX_LIMIT_CASE					10000
#define TESTING_BACKGROUND_DIMENSIONS	300
#define REFRESH_TIME					0.2
#define TAILLE							20		//La taille du fichier .txt créé pour chaque image : actuellement, 20 x 20 = 400 nombres entre 0 et 1
#define MANUAL_RESIZE_MULTIPLIER		4
#define TAILLEMAX						10000
#define BACKGROUNDCOLOR					255, 255, 255
#define TEXTCOLOR						0, 0, 0
#define TEXTSIZE						280
#define DOSSIERPOLICES					"fonts/"
#define DOSSIERIMAGES					"images/"	//Le sous-dossier où il faudra placer les images
#define NAME_CONFIG_FILE				"NeuronProject.cfg"
#define DOSSIERTEST						"test/"
#define DOSSIERTESTTEXT					"test_texts/"
#define CHARSBIS						{ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" }
#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,?;.:!éàè'()+-\"="
#define DOSSIERTEXTES					"texts/"//Le sous-dossier où seront créés les textes
#define DOSSIER_SVG						"svg_reseau/"
#define NOM_SVG							"plus_recent_"
#define EXTENSION_SVG					".svg_reseau"


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

using namespace std;

#endif	//CONFIG_H_INCLUDED
