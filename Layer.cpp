#include "Layer.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

Layer::Layer() : m_previousLayer(0), m_nextLayer(0), m_bindingsNumber(0)
{
}

Layer::Layer(Network* network, int neurons, Layer* previousLayer, Layer* nextLayer, transfert trsf) : m_network(network), m_bindingsNumber(0)
{
	/* neurons : nombre de neurones dans la couche */
	if (network)
	{
		if (network->getFirstLayer() == 0)
		{
			//cout << "coucou" << endl;
			network->setFirstLayer(this);	//on vérifie une première fois que la
			//fisrtlayer du réseau n'ait pas changé, puis une 2nde juste
			//en-dessous dans les fonctions setNextLayer/setPreviousLayer
		}
	}
	setNextLayer(nextLayer);			//on fait le lien dans les deux sens
	setPreviousLayer(previousLayer);	//idem
	for (int i = 0; i < neurons; i++)
		addNeuron(trsf);				//on ajoute le nombre de neurones demandé
}

Layer::~Layer()	//destructeur, inintéressant
{
	for (int i = 0; i < getSize(); i++)
	{
		delete m_neurons[i];
		m_neurons.pop_back();
	}
	if (!isLast())
		delete m_nextLayer;
}

int Layer::getSize() const
{
	return m_neurons.size();
}													//nombre de neurones
Layer* Layer::getNextLayer() const
{
	return m_nextLayer;
}
bool Layer::setNextLayer(Layer* layer)
{
	m_nextLayer = layer;
	if (layer)
	{
		if (layer->getNetwork() != getNetwork())	//on ne relie pas si les couche
			return false;							//ne sont pas du mm réseau
		layer->m_previousLayer = this;
		if (layer == m_network->getFirstLayer())	//on vérifie que la firstlayer
			m_network->setFirstLayer(this);			//n'ait pas changé
		return true;
	}
	return false;
}
Layer* Layer::getPreviousLayer() const
{
	return m_previousLayer;
}
bool Layer::setPreviousLayer(Layer* layer)
{
	m_previousLayer = layer;
	if (layer)
	{
		if (layer->getNetwork() != getNetwork())	//on ne relie pas si les couche
			return false;							//ne sont pas du mm réseau
		layer->m_nextLayer = this;
		if (this == m_network->getFirstLayer())		//on vérifie que la firstlayer n'ait
			m_network->setFirstLayer(layer);		//pas changé
		return true;
	}
	return false;
}
bool Layer::isLast() const
{
	return m_nextLayer == 0;
}												//ne pas faire confiance à ces
bool Layer::isFirst() const
{
	return m_previousLayer == 0;
}														//deux fonctions dans le
//cas d'un réseau bouclé
Neuron* Layer::getNeuron(int n) const
{
	if (n >= 0 && n < getSize())
		return m_neurons[n];
	return 0;
}

void Layer::addNeuron(transfert trsf)	//NOUVEAU NEURONE !!!
{
	Neuron* neuron = new Neuron(this, trsf);
	double	weight;

	if (!isLast())		//on établit des liens avec les neurones de la couche d'après
	{					//que s'ils existent
		for (int i = 0; i < getNextLayer()->getSize(); i++)
		{
			weight	= rand() % 1000;
			weight	= (weight - 500) / 500;
			//weight est maintenant un nombre aléatoire entre -1 et 1
			//std::cout << weight << std::endl;
			Binding* bdg = new Binding(neuron, weight);
			getNextLayer()->getNeuron(i)->addBinding(bdg);	//le neuron ajouté à
			//la couche n aura un tableau de weight de taille égale au nombre de
			//neurones de la couche n+1
		}
	}
	if (!isFirst())		//on établit des liens avec les neurones de a couche d'avant
	{					//que s'ils existent
		for (int i = 0; i < getPreviousLayer()->getSize(); i++)
		{
			weight	= rand() % 1000;
			weight	= (weight - 500) / 500;
			//weight est maintenant un nombre aléatoire entre -1 et 1
			//std::cout << weight << std::endl;
			Binding* bdg = new Binding(getPreviousLayer()->getNeuron(i), weight);
			neuron->addBinding(bdg);//chaque neurone
			//de la couche n-1 augmente de 1 la taille de son tableau weight
		}
	}
	m_neurons.push_back(neuron);
}

void Layer::addNeurons(int n, transfert trsf)
{
	for (int i = 0; i < n; i++)
		addNeuron(trsf);
}

void Layer::calculate() const	//propagation normale
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
	{
		m_neurons[i]->receive();//chauqe neurone va chercher l'output de tous
		//les neurones de la couche précédente, en faire la somme pondérée par
		//les poids, puis calculer son propre output avec la fonction de transfert
	}
}

void Layer::calculateGradient() const	//rétropropasgation du gradient
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->sendGradient();
}

int Layer::getBindingsNumber() const
{
	return m_bindingsNumber;
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
