#ifndef NETWORKARRAY_H_INCLUDED
#define NETWORKARRAY_H_INCLUDED

#include "Network.h"

class Network;

class NetworkArray {
public:
	NetworkArray();		//constructeur
	~NetworkArray();	//destructeur
	//METHODES
	char testNetworks(double input[]);
	void	learnAllNetworks();
	void	getMostRecent();
	void	getLettresTestees();

	char*	getAlphabet();
	void	setAlphabet(char* alphabet);
	double	getMaximalDistance();
	void	setMaximalDistance(double maximal_distance);
	int		getMaxLimitLoop();
	void	setMaxLimitLoop(int maxLimitLoop);

private:
	Network**	m_tablo_net;	//le tableau des networks
	double		m_maximal_distance;
	int			m_maxLimitLoop;
	const char* m_alphabet;
};

template <class T>
void displayArray(T* data, int length);																									//affichage d'un tableau
bool readExemple(char* nom_fichier, double entrees[], int taille_entree, string directory = DIR_EXEMPLE);
int		countExemples(string directory = DIR_EXEMPLE);																		//compte les exemples dans directory
void	getArrayOfFileNames(char** tabloFichier, string directory = DIR_EXEMPLE);											// lit le tableau correspondant à une image
void	getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, string directory = DIR_EXEMPLE);	// récupère tous les tablos d'exemples
#endif	//NETWORKARRAY_H_INCLUDED
