#ifndef NEURONJK_H
#define NEURONJK_H
//! fichier contenant la classe neurone et les fonctions de transfert utiles


#include "config.h"

//! pointeur de fonction
/*! On défini un format de pointeur de fonction, on l'invoquera avec le mot transfert
    Ce format est destiné à contenir les fonctions de transfert des neurones */
typedef double (*transfert)(double);

class Binding; //on indique que l'on va se servir de la class Binding
class Layer; //idem

//! Classe represantant un neurone
class Neuron {

public:
//!constructeur par défaut, contructeur de copie et constructeur que l'on utilise vraiment dans le programme
Neuron();

//! Le constructeur de copie, associe à la copie : la meme fonction de transfert, la meme couche et les memes liaisons vers les memes neurones,
Neuron(const Neuron &neuron);

//! Si aucune fonction de transfert n'est donné en paramètre, on prend une sigmoide
Neuron(Layer* layer, transfert trsf = 0);

//! Destructeur
~Neuron();

//METHODES

//! Changer la fonction de transfert
void setTransfert(double (*)(double));

//! Retourne la fonction de transfert
transfert getTransfert() const;

//! Retourne la couche parente du neurone
Layer*  getLayer() const;

//! Donne le nombre de liaisons avec les neurones des couches PRECEDENTES
int   getBindingsNumber() const;

//! Le neurone récupère la somme pondérée des outputs des neurones des couches d'avant auxquels il est connecte
void  receive();

//! Le neurone envoie son propre gradient aux neurones précedents, pondérée par le poids de chaque liaison
void  sendGradient();
//! nouvelle liaison
/*!
    \param binding ajoute binding à [bindings_](@bindings_)
 */
void  addBinding(Binding* binding);
//! nouvelle liaison
/*!
    \param binding ajoute binding à [bindings_](@bindings_)
 */
void  addBinding(Neuron* neuron, double weight = 1);

//! Retourne l'entree du neurone
double  getInput() const;

//! Retourne la sortie du neurone
double  getOutput() const;

//! Retourne le gradient du neurone
double  getGradient() const;
//! Remet à 0 l'input.
/*!
   Si le neuron est dans la première couche du réseau, input est pris en compte.
 */
void  initNeuron(double input);
//! Remet à zero l'output.
/*!
    Si le réseau est dans la dernière couche, assignation de expectedOutput est autorisée
 */
bool  initNeuronGradient(double expectedOutput);

//! Récupérer la liaison d'indice n
Binding* getBinding(int n);

//! Connaître la position du neuron dans sa couche
int   getIndexInLayer() const;

//! Algorithme d'apprentissage
void  learn();

private:

//! Somme pondérée des entrées
double input_;

//! Sortie = transfert(input_)
double output_;

//! Fonction de transfert du neuron
transfert trsf_;

//! Couche à laquelle il appartient
Layer* layer_;

//! L'ensemble des liaisons (neurones sources, et non successeurs)
std::vector<Binding*> bindings_;

//! Position du neuron dans sa couche
int indexInLayer_;

//! Gradient du neurone
double gradient_;
};

//2 couples (fonction,dérivée)
double threshold(double input); //! fonction de transfert 1
double threshold1(double input); //! fonction de transfert 2

double sigmo(double input);  //! Derivee de la fonction de transfert 1
double sigmo1(double input);  //! Derivee de la fonction de transfert 1

#endif
