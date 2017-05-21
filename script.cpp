#include "script.h"
#include "ReadNetwork.h"
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
	double maximal_distance = MAXIMAL_DISTANCE;
	int length_alphabet = getLenghtAlphabet();
	std::cout << "length_alphabet : " << length_alphabet << "\n";

	int sizes[] = {FIRST_LAYER_SIZE,(int)(2*LAST_LAYER_SIZE),LAST_LAYER_SIZE};
	int layerNb = 3;

	ReadNetwork* rdnk = load(string(DOSSIERBACKUP) + string(NOMBACKUP), false);
	if (rdnk == 0){
    cout << "Pas de sauvegarde trouvee, creation d'un reseau vierge." << endl;
    rdnk = new ReadNetwork(layerNb,sizes,(char*)CHARS,0,MAXIMAL_DISTANCE);
  }
  else
    cout << "Chargement de la sauvegarde " << DOSSIERBACKUP << NOMBACKUP << " reussi." << endl;

	for (int i = 0; i < nbCmds; ++i)
	{
		cout << i << " : " << cmds[i] <<endl;

		if (cmds[i] == "new")
		{
			if (rdnk)
				delete rdnk;
			rdnk = new ReadNetwork(layerNb,sizes,(char*)CHARS,0,maximal_distance);
		}
		else if (cmds[i] == "save")
			rdnk->save(string(DOSSIERBACKUP) + string(NOMBACKUP));

		else if (cmds[i] == "learn")
			rdnk->train();

		else if (cmds[i][0] == '=')
		{
			int id_egal = cmds[i].find("=",1);
			string cmd = cmds[i].substr(1,id_egal);
			if (cmd=="length_alphabet")
					length_alphabet = atoi(cmds[i].substr(id_egal+1).c_str());
			else if (cmd=="maximal_distance")
				maximal_distance = atof(cmds[i].substr(id_egal+1).c_str());

			else if (cmd=="sizes")
			{
				int sizesbis[cmds[i].length()+1];
				sizesbis[0] = FIRST_LAYER_SIZE;
				short j = 0;
				short jbis = 0;
				int index = 0;
				while (cmds[i].substr(j).find(",")!=std::string::npos)
				{
					jbis = cmds[i].substr(j).find(",");
					index ++;
					sizesbis[index] = atof(cmds[i].substr(j,jbis).c_str());
					if (sizesbis[index] ==0 && index < layerNb)
						sizesbis[index] = sizes[index];
				}
				layerNb = index+1;
				int sizes[layerNb];
				for (size_t j = 0; j < layerNb; j++)
					sizes[j] = sizesbis[j];
			}
		}

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
			rdnk->testAllExamples();
		}

		cout << endl;
	}

	if (rdnk)
		delete rdnk;
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
		//on lit chacun des exemples
		readExemple(tabloFichiers[i], tabloExemple[i], FIRST_LAYER_SIZE, directory);
	//cout << "Lecture terminee, " << countExemples(DOSSIERTEXTES) << " exemples lus." << endl;
}
