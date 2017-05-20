#include "Layer.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

Layer::Layer()
	:       previousLayer_(0),      nextLayer_(0),  bindingsNumber_(0)
{
}

Layer::Layer(Network* network, int neurons, Layer* previousLayer, Layer* nextLayer, transfert trsf)
	:       m_network(network),     bindingsNumber_(0)
{
	/* neurons : nombre de neurones dans la couche */
	if (network)
	{
		if (network->getFirstLayer() == 0)
		{
			network->setFirstLayer(this); //on vérifie une première fois que la
			//fisrtlayer du réseau n'ait pas changé, puis une 2nde juste
			//en-dessous dans les fonctions setNextLayer/setPreviousLayer
		}
	}
	setNextLayer(nextLayer); //on fait le lien dans les deux sens
	setPreviousLayer(previousLayer);
	for (int i = 0; i < neurons; i++)
		addNeuron(RAND, trsf);  //on ajoute le nombre de neurones demandé
}

Layer::~Layer() //destructeur, inintéressant
{
	for (int i = 0; i < getSize(); i++)
	{
		delete m_neurons[i];
		m_neurons.pop_back();
	}
	if (!isLast())
		delete nextLayer_;
}

int Layer::getSize() const
{
	return m_neurons.size();
} //nombre de neurones
Layer* Layer::getNextLayer() const
{
	return nextLayer_;
}
bool Layer::setNextLayer(Layer* layer)
{
	nextLayer_ = layer;
	if (layer)
	{
		if (layer->getNetwork() != getNetwork())     //on ne relie pas si les couche
			return false;  //ne sont pas du mm réseau
		layer->previousLayer_ = this;
		if (layer == m_network->getFirstLayer())     //on vérifie que la firstlayer
			m_network->setFirstLayer(this);  //n'ait pas changé
		return true;
	}
	return false;
}
Layer* Layer::getPreviousLayer() const
{
	return previousLayer_;
}
bool Layer::setPreviousLayer(Layer* layer)
{
	previousLayer_ = layer;
	if (layer)
	{
		if (layer->getNetwork() != getNetwork())     //on ne relie pas si les couche
			return false;  //ne sont pas du mm réseau
		layer->nextLayer_ = this;
		if (this == m_network->getFirstLayer())     //on vérifie que la firstlayer n'ait
			m_network->setFirstLayer(layer);  //pas changé
		return true;
	}
	return false;
}
bool Layer::isLast() const
{
	return nextLayer_ == 0;
} //ne pas faire confiance à ces
bool Layer::isFirst() const
{
	return previousLayer_ == 0;
}  //deux fonctions dans le
//cas d'un réseau bouclé
Neuron* Layer::getNeuron(int n) const
{
	if (n >= 0 && n < getSize())
		return m_neurons[n];
	return 0;
}

void Layer::addNeuron(int option, transfert trsf) //NOUVEAU NEURONE !!!
{
	Neuron* neuron = new Neuron(this, trsf);
	double weight = 0.0 ;

	if (!isLast()) //on établit des liens avec les neurones de la couche d'après
	{ //que s'ils existent
		for (int i = 0; i < getNextLayer()->getSize(); i++)
		{
			if(option == RAND){
				weight = rand() % 1000;
				weight = (weight - 500) / 500;
			}
			if(option == ZERO)
				weight = 0.0;
			//weight est maintenant un nombre aléatoire entre -1 et 1
			//std::cout << weight << std::endl;
			Binding* bdg = new Binding(neuron, weight);
			getNextLayer()->getNeuron(i)->addBinding(bdg); //le neuron ajouté à
			//la couche n aura un tableau de weight de taille égale au nombre de
			//neurones de la couche n+1
		}
	}
	if (!isFirst()) //on établit des liens avec les neurones de a couche d'avant
	{ //que s'ils existent
		for (int i = 0; i < getPreviousLayer()->getSize(); i++)
		{
			if(option == RAND){
				weight = rand() % 1000;
				weight = (weight - 500) / 500;
			}
			if(option == ZERO)
				weight = 0.0;
			//weight est maintenant un nombre aléatoire entre -1 et 1
			//std::cout << weight << std::endl;
			Binding* bdg = new Binding(getPreviousLayer()->getNeuron(i), weight);
			neuron->addBinding(bdg);
		}
	}
	m_neurons.push_back(neuron);
}

void Layer::addNeurons(int n, int option, transfert trsf)
{
	for (int i = 0; i < n; i++)
		addNeuron(option, trsf);
}

void Layer::calculate() const //propagation normale
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
	{
		m_neurons[i]->receive();     //chaque neurone va chercher l'output de tous
		//les neurones de la couche précédente, en faire la somme pondérée par
		//les poids, puis calculer son propre output avec la fonction de transfert
	}
}

void Layer::calculateGradient() const //rétropropasgation du gradient
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->sendGradient();
}

int Layer::getBindingsNumber() const
{
	return bindingsNumber_;
}

Network* Layer::getNetwork() const
{
	return m_network;
}


bool Layer::learn()
{
	for (int i = 0; i < getSize(); i++)
		m_neurons[i]->learn();
	return true;
}

void Layer::resetNeurons() const
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->initNeuron(0);
}

void Layer::resetNeuronsGradient() const
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->initNeuronGradient(0);
}
