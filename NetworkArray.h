#ifndef NETWORKARRAY_H_INCLUDED
#define NETWORKARRAY_H_INCLUDED

#include "Network.h"

class Network;

class NetworkArray{
    public :
        NetworkArray();//constructeur
        ~NetworkArray(); //destructeur
        //METHODES
        char testNetworks(double input[]);
        void learnAllNetworks();
        void getMostRecent();
        void getLettresTestees();

        char* getAlphabet();
        void setAlphabet(char* alphabet);
        double getMaximalDistance();
        void setMaximalDistance(double maximal_distance);

    private :
        Network** m_tablo_net; //le tableau des networks
        double m_maximal_distance;
        const char* m_alphabet;
};

template <class T>
void displayArray(T* data, int length);//affichage d'un tableau
bool readExemple(char* nom_fichier,double entrees[],int taille_entree);
bool readExemple(char* nom_fichier,double entrees[],int taille_entree,const char* directory);//idem
int countExemples();
int countExemples(const char* directory);//compte les exemples dans directory
void getArrayOfFileNames(char** tabloFichier); // lit le tableau correspondant à une image
void getArrayOfFileNames(char** tabloFichier, const char* directory); // idem
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples); // récupère tous les tablos d'exemples
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples,const char* directory); //idem
//void getAndLearn();
#endif //NETWORKARRAY_H_INCLUDED
