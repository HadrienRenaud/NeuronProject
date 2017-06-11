#include "Network.h"
using namespace std;

Network::Network()
{
	Network('_');
}
Network::Network(char lettre_testee, string nom_fichier, double maximal_distance, double momentum) :
	firstLayer_(0),
	totalBindingsNumber_(0),
	initialized_(false),
	gradientInitialized_(false),
	momentum_(momentum),
	maximal_distance_(maximal_distance),
	maxLimitLoop_(MAX_LIMIT_LOOP),
	testedLetter_(lettre_testee),
	nameFile_(new char[MAX_LENGTH_NAME_FILE])
{
	strcpy(nameFile_, nom_fichier.c_str());
}

Network::~Network()
{
	delete[] nameFile_;  //allocation dynamique
	delete firstLayer_;  //supprime TOUTES les couches
}

void Network::recuperateur()
{
	// On ouvre le fichier
	ifstream file(nameFile_);

	// Declarations
	int nbTotalLayer, lengthLayer, i, j;
	double weight;
	char*  pEnd(0);
	Neuron*  neurone;
	string ligne, weight_str, neurone_str;
	Layer *  layer(getFirstLayer());

	// On lit le nombre total de couches
	file >> nbTotalLayer;
	// On lit le nombre de neurones de la premiere couche, dont on a pas besoin de récupérer les liasons
	file >> lengthLayer;

	// pour chaque couche
	while (getline(file, ligne))
	{
		//objet crée pour le traitement de la ligne récupérée
		istringstream ligne_stream(ligne);

		// initialisation de l'indice du neurone recevant la liaison sur le couche layer
		i = 0;

		// si on est pas sur la première couche
		if ( layer->getPreviousLayer() != 0 )
		{
			// pour tout neurone de la couche
			while (getline(ligne_stream, neurone_str, ','))
			{
				// indice de la liaison arrivant au neurone considéré
				j  = 0;

				istringstream neurone_stream(neurone_str);   // objet créé pour le traitement du string récupéré
				neurone = layer->getNeuron(i);   // neurone considéré
				while (getline(neurone_stream, weight_str, ' '))   // pour toute liaison arrivant au neurone
				{
					if (weight_str.size())    // si ce n'est pas un faux poids vide
					{
						weight = strtod(weight_str.c_str(), &pEnd);    //on récupère le poids de la liaison
						neurone->getBinding(j)->setWeight(weight);    //on change le poids de la liaison
						j++;
					}
				}
				i++;
			}
		}
		layer = layer->getNextLayer();
		file >> lengthLayer;  // on lit la longueur de la prochaine couche
	}
}

void Network::setFirstLayer(Layer* layer)
{
	if (layer) //Si ce n'est pas un pointeur vide
		firstLayer_ = layer;
	else  //si c'est un pointeur vide
		firstLayer_ = new Layer(this);   // on crée une nouvelle couche
}

Layer* Network::getFirstLayer() const
{
	return firstLayer_;
}

Layer* Network::getLastLayer() const
{
	Layer* temp = firstLayer_; //initialisation à la première couche

	do
		temp = temp->getNextLayer();   //on parcourt les couches
	while (temp->getNextLayer() != firstLayer_ && temp->getNextLayer() != 0); //jusqu'à ce qu'il n'y en ai plus après
	return temp;         //on retourne la dernière
}

int Network::getTotalLayerNumber()
{
	Layer* temp = firstLayer_; //initialisation à la première couche
	int i  = 0;

	do
	{
		temp = temp->getNextLayer();  //on parcourt les couches
		i++;     //en incrémentant le compteur
	} while (temp->getNextLayer() != firstLayer_ && temp->getNextLayer() != 0); //jusqu'à ce qu'il n'y en ai plus
	return i;           // on retourne le compteur
}

int Network::getFirstLayerSize() const
{
	return firstLayer_->getSize();
}

void Network::initNetwork(double *inputs)
{
	//on initialise la premiere couche avec les inputs
	int fls = getFirstLayerSize();
	for (int i = 0; i < fls; i++)
		firstLayer_->getNeuron(i)->initNeuron(inputs[i]);

	//puis on réinitialise tous les aures neurones du réseau
	Layer* temp = firstLayer_;
	do
	{
		temp = temp->getNextLayer();
		temp->resetNeurons();
	} while (temp->getNextLayer() != firstLayer_ && temp->getNextLayer() != 0);
	initialized_ = true;
}

void Network::initNetworkGradient(double* expectedOutputs)
{
	//on initialise la dernière couche
	for (int i = 0; i < getLastLayer()->getSize(); i++)
		getLastLayer()->getNeuron(i)->initNeuronGradient(expectedOutputs[i]);
	//dans cette boucle, on parcourt toutes les layer (sauf la dernière), et on met les gradient à zéro

	Layer* last = getLastLayer();
	Layer* temp = last;
	do
	{
		temp = temp->getPreviousLayer();
		temp->resetNeuronsGradient();
	} while (temp->getPreviousLayer() != last && temp->getPreviousLayer() != 0);
	gradientInitialized_ = true;
}

void Network::launch(double outputs[])
{
	if (initialized_)
	{
		Layer* temp = firstLayer_;
		while (temp->getNextLayer() != 0)
		{
			temp = temp->getNextLayer();
			temp->calculate();
		}  //lorsque sort, la dernière couche a été stimulée : c'est bon.

		//maintenant on récupère la sortie dans un tableau
		for (int i = 0; i < temp->getSize(); i++)
			outputs[i] = temp->getNeuron(i)->getOutput();

		initialized_ = false;  //le réseau n'est plus utilisable pour l'instant
	}
}


bool Network::launchGradient()
{
	if (!gradientInitialized_) //on vérifie que le réseau a bien été intialisé
		return 0;   //pour les gradient
	Layer* temp = getLastLayer();
	do
	{
		temp->calculateGradient();  //on calcule le gradient de la couche d'avant
		temp = temp->getPreviousLayer();
	} while (temp != firstLayer_);  //jusqu'à la première
	gradientInitialized_ = false;  //le réseau n'est plus utilisable
	//pour le moment
	return true;
}

bool Network::learn()
{
	launchGradient();
	Layer* temp = firstLayer_;
	while (temp != 0)  //on parcourt une par une les couches, on dit à chacune de learn()
	{
		temp->learn();
		temp = temp->getNextLayer();
	}
	return true;
}

double Network::getMomentum()
{
	return momentum_;
}
void Network::setMomentum(double momentum)
{
	momentum_ = momentum;
}

double Network::getMaximalDistance()
{
	return maximal_distance_;
}
void Network::setMaximalDistance(double maximal_distance)
{
	maximal_distance_ = maximal_distance;
}

int Network::getMaxLimitLoop()
{
	return maxLimitLoop_;
}
void Network::setMaxLimitLoop(int maxLimitLoop)
{
	maxLimitLoop_ = maxLimitLoop;
}

double distance(double* data1, double* data2, int length)   //on fait la moyenne des carrés de chaque écart entre data1 et data2
{
	double res = 0;      //initialisation

	for (int i = 0; i < length; i++) //on parcourt la liste
		res += ((data1[i] - data2[i]) * (data1[i] - data2[i]));   //On augmente la moyenne du carré de la distance entre data1 et data2 ~ Variance
	res /= length;      // On moyenne

	return res;
}

double distanceMod(double* data1, double* data2, int length)
{
	double res = 0; //initialisation
	int j = -1; //initialisation

	for (int i = 0; i < length; i++)
	{
		res += ((data1[i] - data2[i]) * (data1[i] - data2[i]));  //carré de la distance
		j = (i + 26) % length;  //position de la majuscule
		res += ((data1[i] - data2[j]) * (data1[i] - data2[j]));  //carré de la distance à la majuscule
	}
	res /= length * 2;    // on moyenne

	return res;
}
