#ifndef NETWORKARRAY_H_INCLUDED
#define NETWORKARRAY_H_INCLUDED

#include "config.h"

using namespace std;

class Network;

//! Classe comprenant tous les réseaux avec des méthodes groupées.
class NetworkArray {
public:

//! Constructeur : crée les réseaux, et charge les options.
NetworkArray(int length_alphabet = LENGTH_ALPHABET);

//! Destructeur : détruit les réseaux.
~NetworkArray();

//METHODES

//! Teste l'entree sur les reseaux.
/*!	Fait passer l'entree dans les reseaux et retourne la lettre du reseau maximal.
    \param input entree du reseau
    \param verbose bool (optionnal : default = true ) si true : affiche les infos de sortie
    \return la sortie la plus probable, ou '_' si aucun reseau n'a plus de LOWER_BOUND_DISTINCTION
 */
char testNetworks(double input[], bool verbose = true);

//! Teste toutes les entrees du repertoire de teste
/*! Fait passer toutes les entrees dans les reseaux
    \param directory string (optionnal : default = DOSSIERTESTTEXT ) dossier de test
    \return stat double ratio of succes
 */
double testAll(string directory = DOSSIERTESTTEXT);

//! Lance l'apprentissage des reseaux.
/*!	Chaque reseau apprend l'un après l'autre sur les exemples donnes dans DOSSIERTEXTES,
    l'apprentissage d'un reseau s'arrete quand sa distance_maximale sur les exemples est inferieure à maximal_distance
    ou quand il a deja effectue max_limit_loop apprentissages .
 */
void learnAllNetworks();

//! Actualise les reseaux.
/*!
    Chaque reseau reprend les poids enregistres dans le fichier specifie dans mostRecent.txt
 */
void getMostRecent();

//! Actualise les options des reseaux.
void setOptions();

//! Retourne le moment d'inertie des reseaux.
/*!
   \return m_momentum : le moment d'inertie du reseau */
double getMomentum();

//! Fixe le moment d'inertie des reseaux
/*! selon le moment :
   \param momentum nouveau moment d'inertie
 */
void setMomentum(double momentum);

//! Retourne l'alphabet des reseaux
char* getAlphabet();

//! Fixe des reseaux
void setAlphabet(char* alphabet);

//! Retourne la distance maximale apres apprentissage productif
double getMaximalDistance();

//! Fixe la distance maximale apres apprentissage productif des reseaux
void setMaximalDistance(double maximal_distance);

//! Retourne le nombre maximal de boucles d'apprentissage a effectuer a chaque apprentissage
int  getMaxLimitLoop();

//! Fixe le nombre maximal de boucles d'apprentissage a effectuer a chaque apprentissage
void setMaxLimitLoop(int maxLimitLoop);

void save();

private:
//! Le tableau des networks
Network** m_tablo_net;
//! La distance maximale après un apprentissage producti
double m_maximal_distance;
//! La limite du nombre de boucles d'apprentissag
int m_maxLimitLoop;
//! L'alphabet concerne
const char* m_alphabet;
//! La longueur de l'alphabet concern
int m_length_alphabet;
//! Le moment d'inertie de tous les réseau
double m_momentum;
};

//! affichage d'un tableau
template <class T>
void displayArray(T* data, int length);
//! Lit un exemple :
/*
   L'exemple est donne comme son nom de fichier `nom_fichier` et l'entrée recuperee sera sur `entrees`
 */
bool readExemple(char* nom_fichier, double entrees[], int taille_entree, string directory = DOSSIERTEXTES);

//! Compte les exemples dans directory
int  countExemples(string directory = DOSSIERTEXTES);

//! Recupere les noms des fichiers dans un dossier, les met dans le `tabloFichiers` passe en argument
void getArrayOfFileNames(char** tabloFichier, string directory = DOSSIERTEXTES);

//! Récupère tous les tableaux d'exemples
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, string directory = DOSSIERTEXTES);

#endif  //NETWORKARRAY_H_INCLUDED
