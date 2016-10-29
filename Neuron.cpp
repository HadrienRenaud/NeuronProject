#include "config.h"
#include <iostream>
using namespace std;

Neuron::Neuron() :
								m_input(0),
								m_output(0),
								m_trsf(0),
								m_layer(0),
								m_indexInLayer(0),
								m_gradient(0)
{
}         //constructeur par défaut inutile
Neuron::Neuron(const Neuron& neuron) :
								m_input(0),
								m_output(0),
								m_trsf(neuron.m_trsf),
								m_layer(neuron.m_layer),
								m_bindings(neuron.m_bindings),
								m_indexInLayer(neuron.m_indexInLayer),
								m_gradient(0)
{
}              //jamais utilisé
Neuron::Neuron(Layer* layer, transfert trsf) :
								m_input(0),
								m_output(0),
								m_trsf(trsf),
								m_layer(layer),
								m_indexInLayer(layer->getSize()),
								m_gradient(0)
{
								if (trsf == 0)
																m_trsf = sigmo;
} //utlisé lors de la création d'une couche, il ne faut pas invoquer ce constructeur lorsque'on veut ajouter un neurone à une couche après avoir créé la couche
//il faut utiliser la fonction addNeuron() sur la couche concernée.

Neuron::~Neuron() //destructeur, inintéressant
{
								for (int i = 0; i < (int)m_bindings.size(); i++)
								{
																delete m_bindings[i];
																m_bindings.pop_back();
								}
}

void Neuron::setTransfert(double (*trsf)(double))
{
								m_trsf = trsf;
}
transfert Neuron::getTransfert() const
{
								return m_trsf;
}
Layer* Neuron::getLayer() const
{
								return m_layer;
}
int Neuron::getBindingsNumber() const
{
								return m_bindings.size();
}

void Neuron::receive() //le neurone calcule son input en récupérant la somme pondérée de neurones d'avant
{
								for (int i = 0; i < (int)m_bindings.size(); i++)
																m_input += (m_bindings[i]->getNeuron()->getOutput()) * (m_bindings[i]->getWeight());
								//enfin, calcul de l'output avec la fonction de transfert
								m_output = m_trsf(m_input);
}

void Neuron::sendGradient() //rétrop propager le gradient aux neurones des couches d'avant
{
								for (int i = 0; i < (int)m_bindings.size(); i++)
																m_bindings[i]->getNeuron()->m_gradient += (m_gradient * (m_bindings[i]->getWeight()) /*trsf[1]*/ * sigmo1(m_bindings[i]->getNeuron()->m_input));
								// gradien_du_neurone_précédent+=(gradient_actuel*poids_de_la_liaison*dérivée_de_sigmo_en(m_input));
}

void Neuron::addBinding(Binding* binding)
{
								m_bindings.push_back(binding);
}

void Neuron::addBinding(Neuron* neuron, double weight)
{
								Binding* binding = new Binding(neuron, weight);

								addBinding(binding);
}

double Neuron::getInput() const
{
								return m_input;
}

double Neuron::getOutput() const
{
								return m_output;
}

double Neuron::getGradient() const
{
								return m_gradient;
}

bool Neuron::initNeuron(double input) //on autorise de regler l'input si et seulement si il s'agit d'un neurone de la première couche
{
								if (getLayer()->getNetwork()->getFirstLayer() == getLayer() || input == 0)
								{
																m_input = input;
																if (input != input)
																								cout << "Neuron : initNeuron : if_firstlayer : input : " << input << endl;
																m_output = m_trsf(m_input);
																return true;
								}
								else //si echec
																return false;
}

bool Neuron::initNeuronGradient(double expectedOutput)
{
								//on initialise le gradient du neurone à une valeur différente de 0, seulement si le neurone est dans la dernière couche.
								if (getLayer()->getNetwork()->getLastLayer() == getLayer())
								{
																m_gradient = /* 2 * */ sigmo1(m_input) * (expectedOutput - m_output); //le x2 est dans la poly mais pas sur wiki
																return true;
								}
								//on peut initialiser le gradient de n'impote quel neurone à 0.
								else if (expectedOutput == 0)
								{
																m_gradient = 0;
																return true;
								}
								else //si echec, le retour n'est jamais utilisé
																return false;
}


Binding* Neuron::getBinding(int n) //liaison d'indice n
{
								Binding *b;

								if (n >= 0 && n < (int)m_bindings.size())
																b = m_bindings[n];
								return m_bindings[n];
}

int Neuron::getIndexInLayer() const
{
								return m_indexInLayer;
}

void Neuron::learn() //Le neuron recalcule le poids de toutes ces liaisons avec les neurones précédents
{      //la fonction utilise MU, le gradient du neuron présent et l'output du neuron qui est à l'autre bout de la laison
								for (int i = 0; i < (int)m_bindings.size(); i++)
																m_bindings[i]->addWeight((m_gradient) * MU * (m_bindings[i]->getNeuron()->m_output));
}

//////////////HORS NEURON////////////////////////////
double threshold(double input)
{
								if (input > THRESHOLD)
																return 1;
								return -1;
}
double threshold1(double input) //dérivée de threshold
{
								return 0;
}

inline double sigmo(double input)
{
								double result((exp(PENTE * input) - 1) / (exp(PENTE * input) + 1));

								if (result != result)
																cout << "sigmo probleme avec input = " << input << endl;
								return result;
}
inline double sigmo1(double input) //dérivée de sigmo
{
								return 2 * PENTE * exp(PENTE * input) / ((exp(PENTE * input) + 1) * (exp(PENTE * input) + 1));
}
