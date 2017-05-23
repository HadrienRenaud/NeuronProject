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

	// Initialisations
	double maximal_distance = MAXIMAL_DISTANCE;
	double momentum = ALPHA;
	double mu = MU;
	int length_alphabet = LENGTH_ALPHABET;
	int sizesBis[MAX_NUMBER_LAYER] = {FIRST_LAYER_SIZE,(int)(2*LAST_LAYER_SIZE),LAST_LAYER_SIZE};
	int layerNb = 3;

	getConfigValue(&length_alphabet, &mu, &maximal_distance, &momentum, &layerNb, sizesBis);
	int sizes[layerNb];
	for (int i = 0; i < layerNb; i++)
		sizes[i] = sizesBis[i];

	ReadNetwork* rdnk = load(string(DOSSIERBACKUP) + string(NOMBACKUP), false);
	if (rdnk == 0){
    cout << "Pas de sauvegarde trouvee, creation d'un reseau vierge." << endl;
    rdnk = new ReadNetwork(layerNb,sizes,(char*)CHARS,0,maximal_distance, momentum, mu);
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
			rdnk = new ReadNetwork(layerNb,sizes,(char*)CHARS,0,maximal_distance, momentum);
		}
		else if (cmds[i] == "save")
			rdnk->save(string(DOSSIERBACKUP) + string(NOMBACKUP));

		else if (cmds[i] == "learn")
			rdnk->train(maximal_distance);

		else if (cmds[i] == "script")
		{
			ifstream file(NAME_SCRIPT_FILE);
			if (file)
				scriptFile(file);
			else
				cout << "Pas de script a executer." << endl;
		}

		else if (cmds[i][0] == '=')
		{
			int id_egal = cmds[i].find("=",1);
			string cmd = cmds[i].substr(1,id_egal - 1);
			cout << "Change " << cmd << " vers " << cmds[i].substr(id_egal+1).c_str() << endl;
			if (cmd=="length_alphabet")
				length_alphabet = atoi(cmds[i].substr(id_egal+1).c_str());
			else if (cmd=="maximal_distance")
				maximal_distance = atof(cmds[i].substr(id_egal+1).c_str());
			else if (cmd=="momentum")
				momentum = atof(cmds[i].substr(id_egal+1).c_str());
			else if (cmd=="mu")
				mu = atof(cmds[i].substr(id_egal+1).c_str());

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
				for (int j = 0; j < layerNb; j++)
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

void getConfigValue(int* length_alphabet, double* mu, double* maximal_distance, double* momentum, int* layerNb, int sizes[])
{
	ifstream	optionsFile(NAME_CONFIG_FILE);
	string		line;
	string		cmdName;
	string		bin;
	string		cmdValueStr("");

	while (getline(optionsFile, line))
	{
		if (!(line.length() == 0 || line[0] == '#'))
		{
			istringstream line_stream(line);
			line_stream >> cmdName;
			line_stream >> bin;
			line_stream >> cmdValueStr;

			if (cmdName == "mu" && cmdValueStr.length() > 0)
				*mu = atof(cmdValueStr.c_str());
			if (cmdName == "length_alphabet" && cmdValueStr.length() > 0)
				*length_alphabet = atoi(cmdValueStr.c_str());
			if (cmdName == "maximal_distance" && cmdValueStr.length() > 0)
				*maximal_distance = atoi(cmdValueStr.c_str());
			if (cmdName == "momentum" && cmdValueStr.length() > 0)
				*momentum = atoi(cmdValueStr.c_str());

			if (cmdName == "sizes") {
				int sizesbis[MAX_NUMBER_LAYER];
				sizesbis[0] = FIRST_LAYER_SIZE;
				short j = 0;
				short jbis = 0;
				int index = 0;

				// On parcourt les couches
				while (cmdValueStr.substr(j).find(",")!=std::string::npos)
				{
					jbis = j + cmdValueStr.substr(j).find(",");
					sizesbis[index] = atof(cmdValueStr.substr(j,jbis).c_str());
					index ++;
					if (sizesbis[index] ==0 && index < *layerNb)
						sizesbis[index] = sizes[index];
					j = jbis + 1;
				}

				//Dernière couche
				jbis = j + cmdValueStr.substr(j).find(",");
				sizesbis[index] = atof(cmdValueStr.substr(j,jbis).c_str());
				if (sizesbis[index] ==0 && index < *layerNb)
					sizesbis[index] = sizes[index];

				//On actualise layerNb
				*layerNb = index+1;

				// On recopie dans sizes
				for (int j = 0; j < *layerNb; j++)
					sizes[j] = sizesbis[j];
			}
		}
	}
}
