#ifndef READNETWORK_H_INCLUDED
#define READNETWORK_H_INCLUDED

#include "config.h"





//! affichage d'un tableau
template <class T>
void displayArray(T* data, int length);

class Network;
class ReadNetwork: public Network{
    public :
        ReadNetwork();
        //ReadNetwork(const ReadNetwork rdnk);//constructeur de copie
        ReadNetwork(int layerNb, int* layerSizes, char* alphabet, transfert trsf, double maximal_distance = MAXIMAL_DISTANCE, double momentum = ALPHA, double mu = MU);
        ~ReadNetwork();

        int getLenghtAlphabet();
        void setLengthAlphabet(int);

        char* getAlphabet();
        void setAlphabet(char*);

        void train(double maximal_distance = MAXIMAL_DISTANCE);
        void save(string name);
        char test(char* name, string directory);
        //! Teste toutes les entrees du repertoire de teste
        /*! Fait passer toutes les entrees dans les reseaux
            \param directory string (optionnal : default = DOSSIERTESTTEXT ) dossier de test
            \return stat double ratio of succes
         */
        double testAllExamples(std::string directory = DOSSIERTESTTEXT);


    protected :
        //nombre max de boucle d'apprentissage
        int m_maxLimitLoop;
        //! L'alphabet concerne
        char* m_alphabet;
        //! La longueur de l'alphabet concerné
        int m_length_alphabet;


};

ReadNetwork* load(string name, bool treat_error);

bool isSuccess(double* tab1, double* tab2, int length, double dist);



//! Lit un exemple :
//! L'exemple est donne comme son nom de fichier `nom_fichier` et l'entrée recuperee sera sur `entrees`
bool readExemple(char* nom_fichier, double entrees[], int taille_entree, std::string directory = DOSSIERTEXTES);

#endif
