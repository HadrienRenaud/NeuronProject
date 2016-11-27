#include "script.h"

using namespace std;

void scriptFile(ifstream &input)
{
    std::vector<string> cmdsVect;
	string cmdName;
	int compteur(0);

    while(input >> cmdName)
    {
        cmdsVect.push_back(cmdName);
    }

    string cmds[cmdsVect.size()];
    for (unsigned int i = 0; i < (int)cmdsVect.size(); ++i)
        cmds[i]=cmdsVect[i];

    std::cout << cmdsVect.size() << "\n";

    input.close();

    commands(cmdsVect.size(),cmds);
}

void commands(int nbCmds, string cmds[])
{
    int length_alphabet = getLenghtAlphabet();
    NetworkArray* tablo_net = new NetworkArray(length_alphabet);

    for (int i = 0; i < nbCmds; ++i)
    {
        cout << "Command " << i << " : " << cmds[i] <<endl;

        if (cmds[i] == "new")
        {
            if (tablo_net)
                delete tablo_net;
            NetworkArray* tablo_net = new NetworkArray(length_alphabet);
        }

        else if (cmds[i] == "save")
        {
            if (tablo_net)
                tablo_net -> save();
            else
                cout << "Réseaux vides" <<endl;
        }

        else if (cmds[i] == "learn")
            tablo_net->learnAllNetworks();

        else if (cmds[i] == "filter")
            filtres();

        else if (cmds[i] == "database")
            database(false, true, length_alphabet);

        else if (cmds[i] == "test")
        {
            filtres(DOSSIERTEST, DOSSIERTESTTEXT, true);
            cout << tablo_net->testAll() << endl;
        }

        cout << endl << "End commands" << endl;
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
