#ifndef BLINDING_H
#define BLINDING_H

#include "config.h"

using namespace std;

class Neuron; //on dit qu'on va utiliser la classe Neuron

//! Classe liaison
/*! la classe liaison est composé du neurone source et d'un poids, un neurone a autant de Bindings que de neurones prédecesseurs. */
class Binding {
public:
//! Constructeur par défaut (vide)
Binding();
//! Constructeur utilise
/*!
   \param neuron Neurone source
   \param weight poids de la liaison
 */
Binding(Neuron* neuron, double weight = 1);

//! Destructeur
~Binding();


//! Retourne le Neurone source
Neuron* getNeuron() const;

//! Fixe le poids de la liaison
void setWeight(double weight = 1);

//! Retourne le poids de la liaison
double getWeight() const;

//! Raccourci pour setWeight( weight + getWeight())
void addWeight(double weight);

private:

//! Neurone source
Neuron* m_neuron;

//! Poids de la liason
double m_weight;

//! Poids a l'etape d'apprentissage precedent : utile pour l'inertie
double m_previousStep;
};

#endif
