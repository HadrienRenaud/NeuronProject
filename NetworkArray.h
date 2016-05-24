#ifndef NETWORKARRAY_H_INCLUDED
#define NETWORKARRAY_H_INCLUDED

#include "config.h"

using namespace std;

class Network;

/* Classe comprenant tous les réseaux avec des méthodes groupées.*/
class NetworkArray {
public:

	/* constructeur : crée les réseaux, et charge les options */
	NetworkArray(int length_alphabet = LENGTH_ALPHABET);

	/* destructeur : détruit les réseaux */
	~NetworkArray();

	//METHODES

	/*! teste l'entree sur les reseaux
	 * fait passer l'entree dans les reseaux et retourne la lettre du reseau maximal.
	 * Si aucun reseau n'a plus que LOWER_BOUND_DISTINCTION, retourne '_' .
	 */
	char testNetworks(double input[]);

	/*! lance l'apprentissage des reseaux.
	 * chaque reseau apprend l'un après l'autre sur les exemples donnes dans DOSSIERTEXTES,
	 * l'apprentissage d'un reseau s'arrete quand sa distance_maximale sur les exemples est inferieure à maximal_distance
	 * ou quand il a deja effectue max_limit_loop apprentissages .
	 */
	void	learnAllNetworks();

	/*! actualise les reseaux
	 * chaque reseau reprend les poids enregistres dans le fichier specifie dans mostRecent.txt
	 */
	void	getMostRecent();

	/*! actualise les options des reseaux */
	void	setOptions();
	double	getMomentum();
	void	setMomentum(double momentum);
	char*	getAlphabet();
	void	setAlphabet(char* alphabet);
	double	getMaximalDistance();
	void	setMaximalDistance(double maximal_distance);
	int		getMaxLimitLoop();
	void	setMaxLimitLoop(int maxLimitLoop);

private:
	/* le tableau des networks */
	Network**	m_tablo_net;
	/*la distance maximale après un apprentissage productif*/
	double		m_maximal_distance;
	/* la limite du nombre de boucles d'apprentissage*/
	int			m_maxLimitLoop;
	/* l'alphabet concerne */
	const char* m_alphabet;
	/* la longueur de l'alphabet concerne*/
	int			m_length_alphabet;
	/* le moment d'inertie de tous les réseaux*/
	double		m_momentum;
};

/*affichage d'un tableau */
template <class T>
void displayArray(T* data, int length);
/*! Lit un exemple :
 * l'exemple est donne comme son nom de fichier `nom_fichier` et l'entrée recuperee sera sur `entrees`
 */
bool readExemple(char* nom_fichier, double entrees[], int taille_entree, string directory = DOSSIERTEXTES);

/*compte les exemples dans directory */
int		countExemples(string directory = DOSSIERTEXTES);

/* Recupere les noms des fichiers dans un dossier, les met dans le `tabloFichiers` passe en argument */
void	getArrayOfFileNames(char** tabloFichier, string directory = DOSSIERTEXTES);

/* récupère tous les tablos d'exemples */
void	getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, string directory = DOSSIERTEXTES);

#endif		//NETWORKARRAY_H_INCLUDED
