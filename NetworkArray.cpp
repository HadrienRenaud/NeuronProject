#include "NetworkArray.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <dirent.h>     //Pour la gestion du répertoire d'images
#include <cstring>
#include <time.h>
using namespace std;


const char* g_dir_exemples("texts/"  ); //dossier ou sont contenus tous les fichiers d'exemples
const char* g_dir_test("test/");
const char g_alphabet[LENGTH_ALPHABET] {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};



NetworkArray::NetworkArray():
    m_tablo_net(new Network*[LENGTH_ALPHABET]),
    m_maximal_distance(DISTANCE_MAXIMALE), m_maxLimitLoop(MAX_LIMIT_LOOP*NB_APPRENTISSAGE), m_alphabet(g_alphabet)
    {
    cout << "Creation des reseaux ... " << flush ;
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
       m_tablo_net[i] = new Network(g_alphabet[i]);//Le réseau
       //Layer* l = new Layer(adresse du réseau, nombre de neurones dans la couche, adresse de la couche précédente, adresse de la couche suivante, fonction de transfert des neuronres de la couche);
       Layer* l1= new Layer(m_tablo_net[i], FIRST_LAYER_SIZE,  0,  0); //première couche
       Layer* l2= new Layer(m_tablo_net[i], 100, l1, 0); //seconde
       Layer* l3= new Layer(m_tablo_net[i], 10, l2, 0); //troisieme
       Layer* lend= new Layer(m_tablo_net[i],LAST_LAYER_SIZE,l3, 0); //couche de fin
    }
    cout << "Reseaux crees !" << endl;
}

NetworkArray::~NetworkArray(){
    cout << "Sauvegarde et destruction des reseaux ..." << flush;
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        m_tablo_net[i]->save();
        delete m_tablo_net[i];
    }
    delete m_tablo_net;
    cout << "Reseaux detruits !" << endl;
}

void NetworkArray::learnAllNetworks(){
    cout << "Bienvenue dans le gestionnaire d'apprentissage du reseau de neurones" << endl << endl;

    cout << "Initialisation des parametres." << endl;
    clock_t t0(clock());//temps de départ du programme

    //nombre d'exemples à traiter
    int const nb_exemples(countExemples());

    // Initialisation des tableaux contenant les donnees des exemples
    char** tabloFichiers = new char*[nb_exemples];
    double** inputs = new double*[nb_exemples];
    for (int i = 0; i < nb_exemples; ++i)
    {
        tabloFichiers[i] = new char[MAX_LENGTH_NAME_FILE];
        inputs[i] = new double[FIRST_LAYER_SIZE];
    }

    //Récupération des données des fichiers
    getArrayOfFileNames(tabloFichiers); //on récupère les noms des ficheirs d'exemples
    getArrayOfExemples(tabloFichiers,inputs,nb_exemples); //on récupère les donnees des exemples
    cout << "Initialisation effectuee en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes, l'apprentissage commence." <<endl <<endl;

    //Apprentissage
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        m_tablo_net[i]->learnNetwork(nb_exemples,tabloFichiers,inputs);
        cout << endl << endl;
    }
}

char NetworkArray::testNetworks(double input[]){
    //initialisation
    char lettre_trouvee('_');
    double maxi(BORNE_INF_DISTINCTION);
    double exp_output[LENGTH_ALPHABET][LAST_LAYER_SIZE];

    //tests
    cout << "Results : ";
    for (int i = 0; i < LENGTH_ALPHABET ; ++i) //pour chaque réseau
    {
        //on récupère la réponse
        m_tablo_net[i]->initNetwork(input);
        m_tablo_net[i]->launch(exp_output[i]);

 //on l'affiche si elle est suffisement grande
        if (exp_output[i][0]>(BORNE_INF_DISTINCTION/10))
            cout << g_alphabet[i] << " : " << exp_output[i][0]*100 << "% -- " ;

        //on trouve le maximum des réponses aux premiers réseaux
        if (maxi<exp_output[i][0])
        { //On modifie maxi si elle est supérieure
            maxi=exp_output[i][0];
            lettre_trouvee=g_alphabet[i];
        }
    }
    cout << endl;
    return lettre_trouvee;
}

void NetworkArray::getMostRecent(){
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        m_tablo_net[i]->getMostRecent();
}

void NetworkArray::getLettresTestees(){
    cout << "NetworkArray : " << this << " tablo_net : "<< m_tablo_net << endl;
    for (int j = 0; j < LENGTH_ALPHABET; ++j)
        m_tablo_net[j]->getLettreTestee();
    cout << endl;
}

double NetworkArray::getMaximalDistance(){
    return m_maximal_distance;
}
void NetworkArray::setMaximalDistance(double maximal_distance){
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        m_tablo_net[i]->setMaximalDistance(maximal_distance);
    m_maximal_distance=maximal_distance;
}

int NetworkArray::getMaxLimitLoop(){
    return m_maxLimitLoop;
}
void NetworkArray::setMaxLimitLoop(int maxLimitLoop) {
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        m_tablo_net[i]->setMaxLimitLoop(maxLimitLoop);
    m_maxLimitLoop=maxLimitLoop;
}

// ######################### Hors de NetworkArray ##############################




template <class T>
void displayArray(T* data, int length){//afficher un tableau de valeur
    cout << "["; //début de l'affichage
    for(int i = 0; i<length-1; i++) // pour chaque valeur du tableau,
        cout << (data[i]>=0 ? "+" : "") << data[i] << "," ; // on l'affiche avec son signe
    cout << (data[length-1]>=0 ? "+" : "") << data[length-1] << "]"; //fin de l'affichage
}

bool readExemple(char* nom_fichier, double entrees[], int taille_entree){ //surcharge, appelle readExemple
    return readExemple(nom_fichier,entrees,taille_entree,g_dir_exemples); //par défaut on lit les exemples dans g_dir_exemples
}
bool readExemple(char* nom_fichier,double entrees[], int taille_entree,const char* directory){
    ifstream file((string(directory)+string(nom_fichier)).c_str()); //ouverture du fichier
    if (!file){
        //ERREUR -> message dans erreur.txt, affiché dans la console
        ofstream file("erreur.txt");
        file << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        cout << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        return false;
    }
    else{
        // On lit le fichier
        for (int i(0);i<taille_entree;i++)
            file >> entrees[i]; // on insère les valeurs dans entrees
        return true;
    }
}

int countExemples(){ //surcharge
    return countExemples(g_dir_exemples); //par défaut ...
}
int countExemples(const char* directory){
    int nombre_fichiers(0);     //Initialisation
    DIR* dp;
    struct dirent* ep;
    dp = opendir(directory);    //On ouvre le répertoire contenant les images avant de le parcourir
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
            if (strlen(ep->d_name) >= 4) //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
                nombre_fichiers++;
        closedir (dp);   //Fermeture du répertoire d'images
    }
    return nombre_fichiers;
}

void getArrayOfFileNames(char** tabloFichiers){ //surcharge
    getArrayOfFileNames(tabloFichiers,g_dir_exemples); //par déa
}
void getArrayOfFileNames(char** tabloFichiers,const char* directory){
    int compteur(0);
    DIR* dp;
    struct dirent* ep;
    dp = opendir(directory);    //On ouvre le répertoire contenant les images avant de le parcourir
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
        {
            if (strlen(ep->d_name) >= 4){ //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
                strcpy(tabloFichiers[compteur],ep->d_name); //on inscrit le nom du fichier dans le tablo
                compteur++;
            }
        }
        closedir (dp);   //Fermeture du répertoire d'images
    }
}

void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples){ //Surcharge
    getArrayOfExemples(tabloFichiers, tabloExemple, nb_exemples , g_dir_exemples); // Par défaut, directory = g_dir_exemples
}
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples,const char* directory){
    cout << "Lecture des exemples ... " << flush;
    for (int i(0); i< nb_exemples;i++)
        readExemple(tabloFichiers[i],tabloExemple[i],FIRST_LAYER_SIZE,directory); //on lit chacun des exemples
    cout << "Lecture terminee." <<endl;
}
