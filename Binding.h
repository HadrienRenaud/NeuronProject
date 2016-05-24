#ifndef BLINDING_H
#define BLINDING_H

#include "config.h"

using namespace std;

class Neuron;	//on dit qu'on va utiliser la classe Neuron

/* la classe liaison est composé du neurone source et d'un poids, un neurone a autant de Bindings que de neurones prédecesseurs. */
class Binding {
public:
	Binding();
	Binding(Neuron* neuron, double weight = 1);
	~Binding();

	Neuron* getNeuron() const;	//on ne met surtout pas de méthode setNeuron(), ça n'aurait aucun sens
	void	setWeight(double weight = 1);
	double	getWeight() const;

	/*raccourci pour setWeight( weight + getWeight())*/
	void	addWeight(double weight);

private:
	Neuron* m_neuron;
	double	m_weight;
	double	m_previousStep;
};

#endif
