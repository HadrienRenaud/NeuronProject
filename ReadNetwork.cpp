#include "ReadNetwork.h"
#include <fstream>
using namespace std;

template <class T>
void displayArray(T* data, int length) //afficher un tableau de valeur
{
	cout << "[";
	for ( int i = 0; i < length - 1; i++)
		cout << (data[i] >= 0 ? "+" : "") << data[i] << ",";
	cout << (data[length - 1] >= 0 ? "+" : "") << data[length - 1] << "]";
}


ReadNetwork::ReadNetwork():Network('b', "", MAXIMAL_DISTANCE){
	m_alphabet = CHARS;
	m_length_alphabet = LENGTH_ALPHABET;
}

//ReadNetwork::ReadNetwork(const ReadNetwork rdnk){}//constructeur de copie

ReadNetwork::ReadNetwork(int layerNb, int* layerSizes, char* alphabet, transfert trsf, double maximal_distance):Network('b', "", maximal_distance),m_maxLimitLoop(0),m_alphabet(alphabet),m_length_alphabet(layerSizes[layerNb - 1]){
	Layer* layer = NULL;
	for(int i = 0; i<layerNb; i++){
		layer = new Layer(this, layerSizes[i], layer, 0, trsf);
	}
	maximal_distance_ = maximal_distance;
}

ReadNetwork::~ReadNetwork(){
	delete[] nameFile_;  //allocation dynamique
	delete firstLayer_;  //supprime TOUTES les couches
}


int ReadNetwork::getLenghtAlphabet(){
	return m_length_alphabet;
}
void ReadNetwork::setLengthAlphabet(int length){
	m_length_alphabet = length;
}

char* ReadNetwork::getAlphabet(){
	return m_alphabet;
}
void ReadNetwork::setAlphabet(char* alphabet){
	if(alphabet)
		m_alphabet = alphabet;
	else
		err("ReadNetwork::setAlphabet : alphabet NULL",1);
}

void ReadNetwork::train(){

	short lastLayerSize = getLastLayer()->getSize();
	if(m_length_alphabet!=lastLayerSize)
		err("ReadNetwork::train : certaines lettre ne seront pas prises en compte car m_length_alphabet!=lastLayerSize",1);
	short firstLayerSize = firstLayer_->getSize();
	short const nb_exemples(countExemples());
	short ignoredExemples = 0;
	char**  tabloFichiers = new char*[nb_exemples];
	double** inputs   = new double*[nb_exemples];
	double** outputExpected = new double*[nb_exemples];
	char* lettres = new char[nb_exemples];
	double outputExperimental[lastLayerSize]; //valeur prototypale en sortie du réseau

	for (int i = 0; i < nb_exemples; ++i)
	{
		tabloFichiers[i] = new char[MAX_LENGTH_NAME_FILE];
		inputs[i]   = new double[firstLayerSize];
		outputExpected[i] = new double[lastLayerSize];
		for(int j = 0; j<lastLayerSize; j++)
			outputExpected[i][j] = 0;
	}
	//Récupération des données des fichiers
	if(nb_exemples != getArrayOfFileNames(tabloFichiers))
		err("ReadNetwork::train() : compteur!=nb_exemples",1);

	for(int i = 0; i<nb_exemples; i++){
		// cout << tabloFichiers[i] << endl;
		lettres[i] = tabloFichiers[i][0];
	}
	// cout << "#####################################";
	getArrayOfExemples(tabloFichiers, inputs, nb_exemples);

	for(int i = 0; i<nb_exemples;i++){
		// cout << tabloFichiers[i] << " : " << lettres[i] << endl;
		int ca = findChar(lettres[i],m_alphabet,m_length_alphabet);
		if(ca==-1){//échec
			// delete outputExpected[i];
			outputExpected[i] = NULL;
			ignoredExemples++;
		}
		else
			outputExpected[i][ca] = 1;
	}
	// Initialisations
	short exemple   = 0; //exemple actuellement selectionné pour l'apprentissage, cette variable est incrémenté à chaque fois qu'il réussi un exemple
	short successes  = 0; //le réseau doit enchainer nb_exemples - ignoredExemples succès pour avoir fini l'apprentissage, cela ne devra pas être le cas pour les caractères manuscrits, parce qu'il y un risque de surapprentissage
	short successesInLoop = 0;
	short maxSuccesses = 0;
	short count   = 0; //nombre de passage dans la boucle
	double dist   = 0; //pour stocker la moyenne des écarts quadratiques entre les valeurs attendues et les valeurs retournées
	double totalDistance = 0;
	double maxDist   = 0;

	clock_t t0(clock()); //temps de départ du programme
	short trueNb_exemples = nb_exemples - ignoredExemples;
	//APPRENTISSAGE
	while ((successes < trueNb_exemples) /*&& (count < maxLimitLoop_ * nb_exemples)*/) //tant qu'on a pas enchaîné nb_exemples succès
	{
		exemple++;
		exemple %= nb_exemples;  //On ne dépasse pas nb_exemples
		if(outputExpected[exemple]!=NULL){//
			count++;
			initNetwork(inputs[exemple]);  //on initialise avec les valeurs inputs
			launch(outputExperimental);  //on lance et on récupère les outputs
			//On apprend, ou pas en fonction du résultat
			if (isSuccess(outputExperimental, outputExpected[exemple], lastLayerSize, maximal_distance_)){  //si c'est assez petit, c'est un succès
				successes++;
			}
			else  //sinon c'est un echec et le réseau recalcule les poids des liaisons
			{
				initNetworkGradient(outputExpected[exemple]);
				learn();
				if(successes > maxSuccesses){
					maxSuccesses = successes;
					cout << 100*maxSuccesses/trueNb_exemples << "%\n";
				}
				successes = 0;  //on réinitialise aussi les nombre de succès enchaînés
			}
		}
	}
	cout << "100%\n";
	cout << "Temps : " << ((float)(clock() - t0) / CLOCKS_PER_SEC) << " secondes." << endl;

	//On sauvegarde le réseau
	// save();

	//Affichages ...
	// cout << endl;
	// if (count >= maxLimitLoop_ * nb_exemples)
	// 	cout << "Apprentissage INFRUCTUEUX sur " << count << " exemples lus.";
	// else
	// 	cout << "Apprentissage productif : " << count << " exemples lus sur " << maxLimitLoop_ * nb_exemples << endl;

	// cout << " avec " << successes << " succes, effectue en " << ((float)(clock() - t0) / CLOCKS_PER_SEC) << " secondes." << endl;

	//Calcul de la distance moyenne
	// for (exemple = 0; exemple < nb_exemples; exemple++){
	// 	//Réponse du réseau
	// 	initNetwork(inputs[exemple]);
	// 	launch(outputExperimental);
	//
	// 	//Calcul de la distance
	// 	totalDistance += distance(outputExperimental, outputExpected[exemple], lastLayerSize );
	// }
	// cout << "Distance moyenne sur les exemples : " << totalDistance / nb_exemples << endl;

	//Calcul du temps mis
	// double temps_mis(((float)(clock() - t0) / CLOCKS_PER_SEC));
	// cout << "Apprentissage effectue en " << temps_mis << " secondes." << endl;

	// On met à jour les données dans le fichier
	// writeReport((count < maxLimitLoop_ * nb_exemples), count / nb_exemples,
	// 			totalDistance / nb_exemples, temps_mis, " ");
	// cout <<"lastone\n";
	// scanf("%s",d);
	for(int i = 0; i<nb_exemples; i++){
		delete tabloFichiers[i];
		delete inputs[i];
		if(outputExpected[i])
			delete outputExpected[i];
	}
	delete tabloFichiers;
	delete inputs;
	delete outputExpected;
	delete lettres;
}

void ReadNetwork::save(string name)
{
	//on écrit dans le fichier
	ofstream file(name);    // flux sortant dans le fichier
	file << maximal_distance_ << ' ';
	file << m_alphabet << '\n';
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
}

char ReadNetwork::test(double* input){
	double* output = new double[m_length_alphabet];
	initNetwork(input);
	launch(output);
	short imax = 0;
	for(short k = 0; k < m_length_alphabet; k ++)
		if(output[k] > output [imax])
			imax = k;
	return m_alphabet[imax];
}

ReadNetwork* load(string name)
{
	// On ouvre le fichier
	ifstream file(name);
	if(!file){
		err("Impossible d'ouvrir le fichier" + name,1);
		return 0;
	}

	// Declarations
	int nbTotalLayer, lengthLayer, lengthPreviousLayer,lab;
	double weight, mdist;
	char*  pEnd(0);
	char tamp;
	Neuron*  neurone;
	string ligne, weight_str, neurone_str,alpha;
	Layer *  layer;
	ReadNetwork* rdnk = new ReadNetwork();

	file >> mdist;
	file >> alpha;

	rdnk->setMaximalDistance(mdist);
	lab = alpha.length();
	char* alphab = new char[lab];//on ne peut pas le delete à la fin de la fonction car le réseau doit garder son alphabet, mais on ne peut pas le delete dans le constructeur non plus, donc c'est une fuite de mémoire
	strncpy(alphab,alpha.c_str(),lab);

	rdnk->setAlphabet(alphab);

	file >> nbTotalLayer;
	file >> lengthLayer;

	layer = new Layer(rdnk,lengthLayer,0,0,0);

	// pour chaque couche
	for(int k = 1; k<=nbTotalLayer; k++)
	{
		lengthPreviousLayer = lengthLayer;
		file >> lengthLayer;  // on lit la longueur de la prochaine couche
		// cout << "lengthLayer = " << lengthLayer << endl;
		layer = new Layer(rdnk,lengthLayer,layer,0,0);
		// pour tout neurone de la couche
		for(int i = 0; i<lengthLayer;i++)
		{
			for(int j = 0; j<lengthPreviousLayer;j++){
				file >> weight;  //on récupère le poids de la liaison
				// cout << "weight[" << k << "][" << i << "][" << j << "] : " << weight <<endl;
				layer->getNeuron(i)->getBinding(j)->setWeight(weight);    //on change le poids de la liaison
			}
			if(i!=lengthPreviousLayer-1)
				file.seekg(2,ios::cur);//on saute l'espace et la virgule à la fin d'un neurone
		}
	}
	rdnk->setLengthAlphabet(lengthLayer);

	return rdnk;
}

int findChar(const char c, const char* str,const int length){
	int res = -1;
	for(int i = 0; i< length; i++){
		if(str[i] == c)
			res = i;
	}
	if(res==-1){
		char* msg = new char[2];
		msg[0] = c;
		msg[1] = '\0';
		err("findChar : lettre non trouvée : " + string(msg),0);
		delete msg;
	}
	return res;
}
double abso(double d){
	if(d<0)
		return -d;
	return d;
}

bool isSuccess(double* tab1, double* tab2, int length, double dist){
	for(int i = 0; i<length; i++){
		if(abso(tab1[i]-tab2[i])>dist)
			return false;
	}
	return true;
}
