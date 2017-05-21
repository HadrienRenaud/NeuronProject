#include "script.h"

using namespace std;

void scriptFile(ifstream &input)
{
	std::vector<string> cmdsVect;
	string cmdName;

	while(input >> cmdName)
	{
		cmdsVect.push_back(cmdName);
	}

	string cmds[cmdsVect.size()];
	for (int i = 0; i < (int)cmdsVect.size(); ++i)
		cmds[i]=cmdsVect[i];

	std::cout << cmdsVect.size() << "\n";

	input.close();

	commands(cmdsVect.size(),cmds);
}

void commands(int nbCmds, string cmds[])
{
	int length_alphabet = getLenghtAlphabet();
	std::cout << length_alphabet << "\n";
	NetworkArray* tablo_net = new NetworkArray(length_alphabet);

	for (int i = 0; i < nbCmds; ++i)
	{
		cout << i << " : " << cmds[i] <<endl;

		if (cmds[i] == "new")
		{
			if (tablo_net)
				delete tablo_net;
			NetworkArray* tablo_net = new NetworkArray(length_alphabet);
		}

		else if (cmds[i] == "save")
		{
			if (tablo_net)
				tablo_net->save();
			else
				cout << "Reseaux vides" <<endl;
		}

		else if (cmds[i] == "learn")
			tablo_net->learnAllNetworks();

	  #ifndef NO_GRAPHIC
		else if (cmds[i] == "filter")
			filtres();

		else if (cmds[i] == "database")
			database(false, true);
	  #endif

		else if (cmds[i] == "test")
		{
      #ifndef NO_GRAPHIC
			filtres(DOSSIERTEST, DOSSIERTESTTEXT, true);
      #endif //NO_GRAPHIC
			tablo_net->testAll();

		}

		cout << endl;
	}

	if (tablo_net)
		delete tablo_net;
}

int getLenghtAlphabet()
{
	ifstream	optionsFile(NAME_CONFIG_FILE);
	string		line;
	string		cmdName;
	string		bin;
	string		cmdValueStr;
	int		cmdValue = LENGTH_ALPHABET;

	while (getline(optionsFile, line))
	{
		if (!(line.length() == 0 || line[0] == '#'))
		{
			istringstream line_stream(line);
			line_stream >> cmdName;
			line_stream >> bin;
			line_stream >> cmdValueStr;

			if (cmdName == "length_alphabet")
				cmdValue = atoi(cmdValueStr.c_str());
		}
	}
	return cmdValue;
}

bool readExemple(char* nom_fichier, double entrees[], int taille_entree, string directory)
{
	char* temp = new char[MAX_LENGTH_NAME_FILE];
	strncpy(temp, nom_fichier, MAX_LENGTH_NAME_FILE);
	ifstream file((directory + string(temp)).c_str());                                                         //ouverture du fichier

	if (!file)
	{
		//ERREUR -> message dans erreur.txt, affiché dans la console
		err("Apprentissage - Bug 1 : Exemple " + directory + string(temp) + " non lu.",1);
		delete temp;
		return false;
	}
	else
	{
		// On lit le fichier
		for (int i(0); i < taille_entree; i++)
			file >> entrees[i];

		delete temp;                                                                                                                                                                        // on insère les valeurs dans entrees
		return true;
	}
}

int countExemples(string directory)
{
	int nombre_fichiers(0);                                                         //Initialisation
	DIR*   dp;
	struct dirent* ep;

	dp = opendir(directory.c_str());                                                         //On ouvre le répertoire contenant les images avant de le parcourir
	if (dp != NULL)                                                         //Si l'ouverture du répertoire a fonctionné
	{
		while ((ep = readdir(dp)))                                                                                                                 //On parcourt le répertoire
			if (strlen(ep->d_name) >= 4)                                                                                                                                                                         //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
				nombre_fichiers++;
		closedir(dp);                                                                                                                 //Fermeture du répertoire d'images
	}
	return nombre_fichiers;
}

int getArrayOfFileNames(char** tabloFichiers, string directory)
{
	int compteur(0);
	DIR*   dp;
	struct dirent* ep;

	//On ouvre le répertoire contenant les images avant de le parcourir
	dp = opendir(directory.c_str());

	//Si l'ouverture du répertoire a fonctionné
	if (dp != NULL)
	{
		//On parcourt le répertoire
		while ((ep = readdir(dp)))
		{
			//Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
			if (strlen(ep->d_name) >= 4 && ep->d_name[0]!=0)
			{
				//on inscrit le nom du fichier dans le tablo
				strncpy(tabloFichiers[compteur], ep->d_name,MAX_LENGTH_NAME_FILE);
				compteur++;
			}
		}

		//Fermeture du répertoire d'images
		closedir(dp);
	}
	return compteur;
}

void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples, string directory)
{
	//cout << "Lecture des exemples ... " << flush;
	for (int i(0); i < nb_exemples; i++)
		readExemple(tabloFichiers[i], tabloExemple[i], FIRST_LAYER_SIZE, directory);                                                                                                                  //on lit chacun des exemples
	//cout << "Lecture terminee, " << countExemples(DOSSIERTEXTES) << " exemples lus." << endl;
}
