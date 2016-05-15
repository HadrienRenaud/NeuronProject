#ifndef BLINDING_H
#define BLINDING_H

#include "Neuron.h"

class Neuron;	//on dit qu'on va utiliser la classe Neuron
class Binding {	//la classe liaison est composé du neurone source et d'un poids, un neurone a autant de Bindings que de neurones prédecesseurs.
public:
	Binding();
	Binding(Neuron* neuron, double weight = 1);
	~Binding();

	Neuron* getNeuron() const;	//on ne met surtout pas de méthode setNeuron(), ça n'aurait aucun sens
	void	setWeight(double weight = 1);
	double	getWeight() const;
	void	addWeight(double weight);	//raccourci

private:
	Neuron* m_neuron;
	double	m_weight;
	double	m_previousStep;
};

#endif
