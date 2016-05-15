#ifndef NETWORK_H
#define NETWORK_H

#include "Layer.h"
#include <string>
#include <cstring>
using namespace std;

#define DIR_EXEMPLE				"texts/" //dossier ou sont contenus tous les fichiers d'exemples
#define g_dir_test				"test/"
#define g_dir_svg				"svg_reseau/"
#define g_nom_svg				"plus_recent_"
#define g_extension_svg			".svg_reseau"
#define FIRST_LAYER_SIZE		400		//nombre de neurones de la première couche
#define LAST_LAYER_SIZE			1		//nombe de neurones de la dernière couche
#define ALPHA					0.05	//momentum, quantité d'inertie
#define MAX_LENGTH_NAME_FILE	100
#define NB_LEARNING				10
#define MAXIMAL_DISTANCE		0.1
#define MAX_LIMIT_LOOP			100
#define LENGTH_ALPHABET			52
#define LOWER_BOUND_DISTINCTION	0.05
#define MAX_LIMIT_CASE			1000


class Layer;//on dit qu'on va utiliser la classe layer
class Network {
public:
	Network();									//rien a mettre dedans
	Network(char lettre_testee, string nom_fichier = "", double maximal_distance = MAXIMAL_DISTANCE);
	~Network();									//destructeur

	void	setFirstLayer(Layer* layer = 0);	//changer la première couche
	Layer*	getFirstLayer() const;
	Layer*	getLastLayer() const;
	int		getFirstLayerSize() const;
	void	initNetwork(double* inputs);					//initialiser pour la propagation normale
	void	initNetworkGradient(double* expectedOutputs);	//initialiser pour la rétropropagation
	int		getTotalBindingsNumber() const;					//nombre de liaisons dans le réseau
	int		getTotalLayerNumber();							//nombre de couches dans le réseau
	void	increaseTotalBindingsNumber(int n = 1);			//agmenter
	bool	isALoop() const;								//tester si le réseau boucle sur lui mm
	void launch(double output[]);							//lancer la propagation, le pointeur length est
															//destiné à stocké la taille du tableau retourné
	bool	launchGradient();								//lancer la rétropropagation du gradient
	bool	learn();										//;)
	void	save();											//sauver l'etat du réseau
//ATTENTION, supprime toutes les donnes ! A n'utiliser qu'avec un nouvel objet
	double	getMomentum();
	void	recuperateur();		// fonction appelée pour récupérer à partir d'un fichier

	void	getMostRecent();	//actualise le réseau
	void	writeReport(bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires);
	void	learnNetwork(const int nb_exemples, char** tabloFichiers, double** inputs);

	char	getLettreTestee();
	void	setLettreTestee(char lettre_testee);
	double	getMaximalDistance();
	void	setMaximalDistance(double maximal_distance);
	int		getMaxLimitLoop();
	void	setMaxLimitLoop(int maxLimitLoop);

private:
	Layer*	m_firstLayer;			//seule la première couche suffit
	int		m_totalBindingsNumber;
	bool	m_initialized;			//on procede à la propagation seulement si m_initialized
									//est vrai
	bool	m_gradientInitialized;	//pareil pour la rétro
	double	m_momentum;				//facteur d'inertie, define -> ALPHA
	double	m_maximal_distance;
	int		m_maxLimitLoop;
	char	m_testedLetter;
	char*	m_nameFile;
};


template <class T>
void	displayArray(T* data, int length);						//affichage d'un tableau
double	distance(double* data1, double* data2, int length);		//calcul d'écart
double	distanceMod(double* data1, double* data2, int length);	//calcul d'écart


#endif
