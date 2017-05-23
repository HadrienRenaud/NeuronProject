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
/*void Network::save()
{
	// On trouve le nom du fichier
	time_t t = time(0);   // l'heure
	ostringstream temps;   //flux pour traitement

	temps << asctime(localtime(&t));  //on récupère le flux correspondant au temps
	string str_nom_fichier(temps.str());  // on crée un string correspondant au flux
	for (int i(0); i < (int)str_nom_fichier.size(); i++)  // on parcourt le nom du fichier
		if (!isalnum(str_nom_fichier[i]))  // et on remplace tout ce qui ne va pas dans un nom de fichier par '_'
			str_nom_fichier[i] = '_';
	str_nom_fichier = string(DOSSIER_SVG) + str_nom_fichier + "_" + testedLetter_ + string(EXTENSION_SVG);
	// adresse complete
	strcpy(nameFile_, str_nom_fichier.c_str());

	//on écrit dans le fichier
	ofstream file(nameFile_);    // flux sortant dans le fichier
	file << getTotalLayerNumber() << ' ';  // on entre le nombre total de couches
	Layer *  layer(getFirstLayer());   // on initialise la premiere couche
	file << layer->getSize() << ' ';   // en donnant sa longueur
	Neuron * neurone;
	while (layer->getNextLayer() != 0)   //pour toute couche
	{
		layer = layer->getNextLayer();  // on prend la suivante
		file << endl << layer->getSize() << ' '; // on donne sa taille
		for (int i(0); i < layer->getSize(); i++) // pour tout neurone de la couche
		{
			neurone = layer->getNeuron(i);  // on récupère le neurone
			for (int j(0); j < neurone->getBindingsNumber(); j++) // pour toute liaison de la couche précédente vers ce neurone
				file << neurone->getBinding(j)->getWeight() << ' ';    // on ajoute au fichier le poids de la liaison
			file << ',';  //séparateur
		}
	}

	// on sauvegarde le dernier fichier enregistré dans mostRecent.txt :
	string mostRecent;
	mostRecent = DOSSIER_SVG + string(NOM_SVG) + testedLetter_ + string(EXTENSION_SVG);
	ofstream fichier_recent(mostRecent.c_str());
	fichier_recent << nameFile_;
}*/

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

bool Network::isALoop() const //on regarde si le réseau se mord la queue
{
	Layer* temp = firstLayer_;

	do
		temp = temp->getNextLayer();
	while (temp != firstLayer_ && temp != 0);
	if (temp) //si la dernière couche est la premiere ...
		return true;
	else
		return false;
}

void Network::launch(double outputs[])
{
	if (initialized_ && !isALoop()) //si on est dans le cas normal : le réseau n'est pas une boucle
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
	if (!isALoop()) //si on est dans le cas normal : le réseau n'est pas une boucle
	{
		Layer* temp = getLastLayer();
		do
		{
			temp->calculateGradient();  //on calcule le gradient de la couche d'avant
			temp = temp->getPreviousLayer();
		} while (temp != firstLayer_);  //jusqu'à la première
		gradientInitialized_ = false;  //le réseau n'est plus utilisable
		//pour le moment
		return true;
	}  //pas de else, flemme de faire le cas de la boucle
	return 0;
}

bool Network::learn()
{
	launchGradient();
	if (!isALoop()) //si on est dans le cas normal : le réseau n'est pas une boucle
	{
		Layer* temp = firstLayer_;
		while (temp != 0)  //on parcourt une par une les couches, on dit à chacune de learn()
		{
			temp->learn();
			temp = temp->getNextLayer();
		}
		return true;
	}
	return false;
}

double Network::getMomentum()
{
	return momentum_;
}
void Network::setMomentum(double momentum)
{
	momentum_ = momentum;
}

/*void Network::getMostRecent()
{
	//initialisations
	string str_nom_fichier;
	string nom_db;

	//on reconstitue le nom du fichier :
	nom_db = string(DOSSIER_SVG) + NOM_SVG + testedLetter_ + string(EXTENSION_SVG);

	ifstream file(nom_db.c_str()); //on ouvre le fichier getMostRecent.txt
	file >> str_nom_fichier; //on lit son contenu
	strcpy(nameFile_, str_nom_fichier.c_str());
	cout << "Reseau " << testedLetter_ << " - recuperation de : " << str_nom_fichier << endl;
	recuperateur(); // On récupère le réseau stocké dans le fichier de svg le plus récent
}*/

/*void Network::writeReport(bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires)
{
	//initialisations
	ofstream base_donnes;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time(&rawtime);
	Layer* temp;

	//Inscription des premières données
	base_donnes.open("Compte_rendu_test_toutes.csv", ios::out | ios::app);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%d/%m/%y", timeinfo);
	base_donnes << buffer << ',';
	strftime(buffer, 80, "%H:%M:%S", timeinfo);
	base_donnes << buffer << "," << testedLetter_ << "," << getTotalLayerNumber() + 1 << ',';

	//inscription des couches
	temp = firstLayer_;
	do
	{
		base_donnes << temp->getSize() << ',';
		temp = temp->getNextLayer();
	} while (temp != 0);
	for (int i(0); i < (4 - (getTotalLayerNumber())); i++)
		base_donnes << ',';

	//fin de l'inscription des données
	base_donnes << count << ',' << distance_moyenne << ',' << maximal_distance_ << ',';
	base_donnes << resultat << ',' << temps_mis << ',' << commentaires << ',' << nameFile_ << endl;
}*/

/*void Network::learnNetwork(const int nbExemples, char** fileArray, double** inputs)
{
	clock_t t0(clock()); //temps de départ du programme

	cout << endl << "Apprentissage de la lettre " << testedLetter_ << " ..." << endl << endl;

	// Initialisations
	int exemple   = 0; //exemple actuellement selectionné pour l'apprentissage, cette variable est incrémenté à chaque fois qu'il réussi un exemple
	int successes  = 0; //le réseau doit enchainer nbExemples succès pour avoir fini l'apprentissage, cela ne devra pas être le cas pour les caractères manuscrits, parce qu'il y un risque de surapprentissage
	int successesInLoop = 0;
	int count   = 0; //nombre de passage dans la boucle
	double dist   = 0; //pour stocker la moyenne des écarts quadratiques entre les valeurs attendues et les valeurs retournées
	double totalDistance = 0;
	double maxDist   = 0;
	double outputExpected[LAST_LAYER_SIZE]; //valeur attendue en sortie du réseau
	double outputExperimental[LAST_LAYER_SIZE]; //valeur prototypale en sortie du réseau


	clock_t t1(clock());

	//APPRENTISSAGE
	while ((successes < nbExemples) && (count < maxLimitLoop_ * nbExemples)) //tant qu'on a pas enchaîné nbExemples succès
	{
		//incrémentation
		exemple++;
		exemple %= nbExemples;  //On ne dépasse pas nbExemples
		count++;

		// Affichages de temps en temps ...
		if (count % (NB_LEARNING * nbExemples) == nbExemples - 1 )  //de temps en temps, on affiche dist et un poids(ça sert à rien le poids mais bon)
		{
			//cout << "count = " << count << " soit " << count / nbExemples << " boucles : ";
			//cout << " ( " << NB_LEARNING << " boucles  en : " << ((float)(clock() - t1) / CLOCKS_PER_SEC) << " s. ) ." << endl;
			cout << "Progression " << min(int(100 * maximal_distance_ / maxDist), 100) << "% - " << count / nbExemples << " boucles." << endl;
			cout << "Distance moyenne : " << (totalDistance / nbExemples) << " - distance maximale : " << maxDist << endl;
			t1 = clock();
		}

		//Remise à zéro des paramètres de boucles
		if (count % nbExemples == 0)
		{
			successesInLoop = 0;
			totalDistance = 0;
			maxDist   = 0;
		}

		//Résultat attendu
		if (fileArray[exemple][0] == testedLetter_)
			outputExpected[0] = 1;
		else
			outputExpected[0] = 0;

		//Calcul de la réponse du réseau
		initNetwork(inputs[exemple]);  //on initialise avec les valeurs inputs
		launch(outputExperimental);  //on lance et on récupère les outputs

		//Calcul de l'écart
		if (count < nbExemples * MAX_LIMIT_CASE && false)
			dist = distance(outputExperimental, outputExpected, LAST_LAYER_SIZE );   //on calcule l'écart
		else
			dist = distanceMod(outputExperimental, outputExpected, LAST_LAYER_SIZE );   //on calcule l'écart

		//changement de maxDist et de totalDistance
		if (dist > maxDist)
			maxDist = dist;
		totalDistance += dist;

		//On apprend, ou pas en fonction du résultat
		if (dist < maximal_distance_)  //si c'est assez petit, c'est un succès
			successes++;
		else  //sinon c'est un echec et le réseau recalcule les poids des liaisons
		{
			initNetworkGradient(outputExpected);
			learn();
			successes = 0;  //on réinitialise aussi les nombre de succès enchaînés
		}

	}

	//On sauvegarde le réseau
	save();

	//Affichages ...
	cout << endl;
	if (count >= maxLimitLoop_ * nbExemples)
		cout << "Apprentissage INFRUCTUEUX sur " << count << " exemples lus.";
	else
		cout << "Apprentissage productif : " << count << " exemples lus sur " << maxLimitLoop_ * nbExemples;

	cout << " avec " << successes << " succes, effectue en " << ((float)(clock() - t0) / CLOCKS_PER_SEC) << " secondes." << endl;

	//Calcul de la distance moyenne
	for (exemple = 0; exemple < nbExemples; exemple++)
	{
		//Réponse attendue
		if (fileArray[exemple][0] == testedLetter_)
			outputExpected[0] = 1;
		else
			outputExpected[0] = 0;
		//Réponse du réseau
		initNetwork(inputs[exemple]);
		launch(outputExperimental);

		//Calcul de la distance
		totalDistance += distance(outputExperimental, outputExpected, LAST_LAYER_SIZE );
	}
	cout << "Distance moyenne sur les exemples : " << totalDistance / nbExemples << endl;

	//Calcul du temps mis
	double temps_mis(((float)(clock() - t0) / CLOCKS_PER_SEC));
	cout << "Apprentissage effectue en " << temps_mis << " secondes." << endl;

	// On met à jour les données dans le fichier
	writeReport((count < maxLimitLoop_ * nbExemples), count / nbExemples,
	            totalDistance / nbExemples, temps_mis, " ");
}*/

/*char Network::getLettreTestee()
{
	return testedLetter_; //récupère la lettre testee.
}*/

/*void Network::setLettreTestee(char lettre_testee) //change la lettre
{
	cout << "Network " << testedLetter_ << " change de lettre et devient ";
	testedLetter_ = lettre_testee;
	cout << testedLetter_ << endl;
}*/

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

/*template <class T>
void displayArray(T* data, int length) //afficher un tableau de valeur
{
	cout << "[";
	for ( int i = 0; i < length - 1; i++)
		cout << (data[i] >= 0 ? "+" : "") << data[i] << ",";
	cout << (data[length - 1] >= 0 ? "+" : "") << data[length - 1] << "]";
}*/

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
