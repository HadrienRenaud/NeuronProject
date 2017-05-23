#include "config.h"
#include <iostream>
using namespace std;

Neuron::Neuron() :
	input_(0),
	output_(0),
	trsf_(0),
	layer_(0),
	indexInLayer_(0),
	gradient_(0),
	mu_(MU)
{
}         //constructeur par défaut inutile
Neuron::Neuron(const Neuron& neuron) :
	input_(0),
	output_(0),
	trsf_(neuron.trsf_),
	layer_(neuron.layer_),
	bindings_(neuron.bindings_),
	indexInLayer_(neuron.indexInLayer_),
	gradient_(0),
	mu_(neuron.mu_)
{
}              //jamais utilisé
Neuron::Neuron(Layer* layer, transfert trsf, double mu) :
	input_(0),
	output_(0),
	trsf_(trsf),
	layer_(layer),
	indexInLayer_(layer->getSize()),
	gradient_(0),
	mu_(mu)
{
	if (trsf == 0)
		trsf_ = sigmo;
} //utlisé lors de la création d'une couche, il ne faut pas invoquer ce constructeur lorsque'on veut ajouter un neurone à une couche après avoir créé la couche
//il faut utiliser la fonction addNeuron() sur la couche concernée.

Neuron::~Neuron() //destructeur, inintéressant
{
	for (int i = 0; i < (int)bindings_.size(); i++)
	{
		delete bindings_[i];
		bindings_.pop_back();
	}
}

void Neuron::setTransfert(double (*trsf)(double))
{
	trsf_ = trsf;
}
transfert Neuron::getTransfert() const
{
	return trsf_;
}
Layer* Neuron::getLayer() const
{
	return layer_;
}
int Neuron::getBindingsNumber() const
{
	return bindings_.size();
}

void Neuron::receive() //le neurone calcule son input en récupérant la somme pondérée des neurones d'avant
{
	input_ = 0;
	for (int i = 0; i < (int)bindings_.size(); i++){
		input_ += (bindings_[i]->getNeuron()->getOutput()) * (bindings_[i]->getWeight());
	}
	//enfin, calcul de l'output avec la fonction de transfert
	output_ = trsf_(input_);
}

void Neuron::sendGradient() //rétrop propager le gradient aux neurones des couches d'avant
{
	for (int i = 0; i < (int)bindings_.size(); i++)
		bindings_[i]->getNeuron()->gradient_ += (gradient_ * (bindings_[i]->getWeight()) /*trsf[1]*/ * sigmo1(bindings_[i]->getNeuron()->input_));
	// gradien_du_neurone_précédent+=(gradient_actuel*poids_de_la_liaison*dérivée_de_sigmo_en(input_));
}

void Neuron::addBinding(Binding* binding)
{
	bindings_.push_back(binding);
}

void Neuron::addBinding(Neuron* neuron, double weight)
{
	Binding* binding = new Binding(neuron, weight);

	addBinding(binding);
}

double Neuron::getInput() const
{
	return input_;
}

double Neuron::getOutput() const
{
	return output_;
}

double Neuron::getGradient() const
{
	return gradient_;
}

void Neuron::initNeuron(double input) //on autorise de regler l'input si et seulement si il s'agit d'un neurone de la première couche
{
	// if (getLayer()->getNetwork()->getFirstLayer() == getLayer() || input == 0)
	// {
		input_ = input;
		// if (input != input)
		// 	cout << "Neuron : initNeuron : if_firstlayer : input : " << input << endl;
		output_ = trsf_(input_);
	// }
}

bool Neuron::initNeuronGradient(double expectedOutput)
{
	//on initialise le gradient du neurone à une valeur différente de 0, seulement si le neurone est dans la dernière couche.
	if (getLayer()->getNetwork()->getLastLayer() == getLayer())
	{
		gradient_ = /* 2 * */ sigmo1(input_) * (expectedOutput - output_);                                                                                                                 //le x2 est dans la poly mais pas sur wiki
		return true;
	}
	//on peut initialiser le gradient de n'impote quel neurone à 0.
	else if (expectedOutput == 0)
	{
		gradient_ = 0;
		return true;
	}
	else                                                         //si echec, le retour n'est jamais utilisé
		return false;
}


Binding* Neuron::getBinding(int n) //liaison d'indice n
{
	Binding *b;

	if (n >= 0 && n < (int)bindings_.size())
		b = bindings_[n];
	return bindings_[n];
}

int Neuron::getIndexInLayer() const
{
	return indexInLayer_;
}

void Neuron::learn() //Le neuron recalcule le poids de toutes ces liaisons avec les neurones précédents
{      //la fonction utilise MU, le gradient du neuron présent et l'output du neuron qui est à l'autre bout de la laison
	// cout << "hello\n";
	for (int i = 0; i < (int)bindings_.size(); i++)
		bindings_[i]->addWeight((gradient_) * mu_ * (bindings_[i]->getNeuron()->output_));
}

double Neuron::getMu()
{
	return mu_;
}

void Neuron::setMu(double mu)
{
	mu_ = mu;
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

	if (result != result){
		cout << "sigmo probleme avec input = " << input << endl;
		return 0;
	}
	return result;
}
inline double sigmo1(double input) //dérivée de sigmo
{
	return 2 * PENTE * exp(PENTE * input) / ((exp(PENTE * input) + 1) * (exp(PENTE * input) + 1));
}
