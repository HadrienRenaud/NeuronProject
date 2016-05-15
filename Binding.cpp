#include "Binding.h"
#include <iostream>
using namespace std;

Binding::Binding() :
	m_neuron(0),
	m_weight(0)
{
	m_previousStep = 0.0;
}																//constructeur par défaut inutile
Binding::Binding(Neuron* neuron, double weight) :
	m_neuron(neuron),
	m_weight(weight)
{
	m_previousStep = 0.0;
}																										//constructeur vraiment utilisé
Binding::~Binding()
{
	//coucou
}

Neuron* Binding::getNeuron() const
{
	return m_neuron;
}

void Binding::setWeight(double weight)
{
	m_weight = weight;
}
double Binding::getWeight() const
{
	return m_weight;
}

void Binding::addWeight(double weight)
{
	double temp = m_weight;

	m_weight		+= (weight + getNeuron()->getLayer()->getNetwork()->getMomentum() * m_previousStep);
	m_previousStep	= m_weight - temp;
}
