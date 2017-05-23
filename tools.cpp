#include "tools.h"
#include "ReadNetwork.h"





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
