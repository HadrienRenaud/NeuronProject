#ifndef NETWORK_H
#define NETWORK_H

#include "Layer.h"
#include <string>
#include <cstring>
using namespace std;

#define g_dir_svg "svg_reseau/"
#define g_nom_svg "plus_recent_"
#define g_extension_svg ".svg_reseau"
#define FIRST_LAYER_SIZE 400//nombre de neurones de la première couche
#define LAST_LAYER_SIZE 1//nombe de neurones de la dernière couche
#define ALPHA 0.05 //momentum, quantité d'inertie
#define MAX_LENGTH_NAME_FILE 100

class Layer;//on dit qu'on va utiliser la classe layer
class Network{
public :
    Network();//rien a mettre de-dans
    Network( string );
    ~Network();//destructeur

    void setFirstLayer(Layer* layer=0);//changer la première couche
    Layer* getFirstLayer()const;
    Layer* getLastLayer()const;
    int getFirstLayerSize()const;
    void initNetwork(double* inputs);//initialiser pour la propagation normale
    void initNetworkGradient(double* expectedOutputs);//initialiser pour la
                                                        //rétropropagation
    int getTotalBindingsNumber()const;//nombre de liaisons dans le réseau
    int getTotalLayerNumber();//nombre de couches dans le réseau
    void increaseTotalBindingsNumber(int n = 1);//agmenter
    bool isALoop()const;//tester si le réseau boucle sur lui mm
    void launch(double output[]);//lancer la propagation, le pointeur length est
                                //destiné à stocké la taille du tableau retourné
    bool launchGradient();//lancer la rétropropagation du gradient
    bool learn();//;)
    char* save(char* char_nom_fichier);
    char* save(char lettre_testee);
    char* save(char lettre_testee,char* char_nom_fichier); //sauver l'etat du réseau
    void recup(string nom_fichier);// récupérer le réseau depuis le fichier
    //ATTENTION, supprime toutes les donnes ! A n'utiliser qu'avec un nouvel objet
    double getMomentum();

private :
    void recuperateur(string nom_fichier);// fonction appelée pour récupérer à partir d'un fichier
    Layer* m_firstLayer;//seule la première couche suffit
    int m_totalBindingsNumber;
    bool m_initialized;//on procede à la propagation seulement si m_initialized
                        //est vrai
    bool m_gradientInitialized;//pareil pour la rétro
    double m_momentum;//facteur d'inertie, define -> ALPHA
};


template <class T>
void displayArray(T* data, int length);//affichage d'un tableau


#endif
