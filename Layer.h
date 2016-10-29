#ifndef LAYERJK_H
#define LAYERJK_H

#include "config.h"

typedef double (*transfert)(double);

class Neuron; //on va utliser la classe neurone
class Network; //network

//! Classe représentant une couche du reseau
/*!
   classe stockant les reseaux de la couche, ayant des methodes generales sur l'ensemble des neurones de la couche
 */
class Layer {

public:
								//! Constructeur par défaut (vide)
								Layer();

								//! Constructeur utilisé
								/*!
								    initialise la couche precedente, la couche suivante, et les neurones qu'il y a dedans.
								    \param network le reseau dans lequel est la couche
								    \param neurons le nombre de neurones de la couche
								    \param previousLayer la couche precedente
								    \param nextLayer la couche suivante
								    \param trsf pointeur de type [transfert](@transfert) vers la fonction de transfert de chacun des neurones
								 */
								Layer(Network* network, int neurons = 0,  Layer* previousLayer = 0, Layer* nextLayer = 0, transfert trsf = 0);

								//! destructeur
								~Layer();

								//! nombre de neurones dans la couche
								int  getSize() const;

								//! Retourne la couche suivante
								Layer* getNextLayer() const;

								//! Fixe la couche suivante
								bool setNextLayer(Layer* layer);

								//! Retourne la couche precedente
								Layer* getPreviousLayer() const;

								//! Fixe la couche precedente
								bool setPreviousLayer(Layer* layer);

								//! S'agit-il de la couche d'entrée ?
								bool isFirst() const;

								//! S'agit-il de la couche d sortie ?
								bool isLast() const;

								//! On récupère le n-ième neurone
								Neuron* getNeuron(int n) const;

								//! Ajouter un neurone dont on spécifie (ou pas) la fonction de transfert
								/*!
								    \param trsf (optional)  pointeur de type [transfert](@transfert) vers la fonction de transfert du neurone
								 */
								void addNeuron(transfert trsf = 0);

								//! Ajouter plusieurs neurones
								/*!
								    \param n nombre de neurones a ajouter
								    \param trsf (optional)  pointeur de type [transfert](@transfert) vers la fonction de transfert du neurone
								 */
								void  addNeurons(int n, transfert trsf = 0);

								//! Demander à chaque neurone de calculer sa sortie
								void  calculate() const;

								//! Demander à chaque neurone d'envoyer le gradient
								void  calculateGradient() const;

								//! Renvoi le nombre de liaisons partant de cette couche
								int   getBindingsNumber() const;

								//! Réseau parent
								Network* getNetwork() const;

								//! Dit a chaque neurone de Neuron::learn()
								bool  learn();

								//! Préparer les neurones à propager
								void  resetNeurons() const;

								//! Préparer les neurones à rétropropager
								void  resetNeuronsGradient() const;

private:

								//! Le réseau auquel la couche appartient
								Network* m_network;

								//! L'ensemble de ses neurones
								std::vector<Neuron*> m_neurons;

								//! Couche precedente
								Layer* m_previousLayer;

								//! Couche suivante
								Layer* m_nextLayer;

								//! nombre de [Bindings](@Bindings), s'incrémente à chaque nouvelle liaison
								int m_bindingsNumber;
};

#endif
