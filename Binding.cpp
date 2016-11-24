#include "Binding.h"
#include <iostream>
using namespace std;

// Useless default constructor
Binding::Binding() :
	neuron_(0),
	weight_(0)
{
	previousStep_ = 0.0;
}
// Useless default constructor
Binding::Binding(Neuron* neuron, double weight) :
	neuron_(neuron),
	weight_(weight)
{
	previousStep_ = 0.0;
}
Binding::~Binding()
{
}

Neuron* Binding::getNeuron() const
{
	return neuron_;
}

void Binding::setWeight(double weight)
{
	weight_ = weight;
}
double Binding::getWeight() const
{
	return weight_;
}

void Binding::addWeight(double weight)
{
	double temp = weight_;

	weight_  += (weight + getNeuron()->getLayer()->getNetwork()->getMomentum() * previousStep_);
	previousStep_ = weight_ - temp;
}
