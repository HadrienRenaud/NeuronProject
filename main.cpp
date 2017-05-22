#include "config.h"
#include "ReadNetwork.h"

using namespace std;



#ifndef NO_GRAPHIC
void graphics()
{
	SDL_Window * window(0);
	SDL_Renderer * ren = init(window);

	menu(ren);
	SDL_DestroyRenderer(ren);
	quit(window);
}
#endif //NO_GRAPHIC

void help()
{
	cout << "Welcome to NeuronProject help page !" << endl;
	cout << "NeuronProject is a little software created by french students for their school project." << endl;
	cout << endl;
	cout << "Usage :" <<endl;
	cout << "	./NeuronProject"<< endl;
	cout << "	./NeuronProject [-h/--help]"<< endl;
	cout << "	./NeuronProject [-f/--file] (FILE)"<< endl;
	cout << "	./NeuronProject [-c/--commands] COMMANDS"<< endl;
	cout << endl;
	cout << "Options :"<< endl;
	cout << "	[None] : default, graphics utilisation"<<endl;
	cout << "	-h, --help : display this help file"<< endl;
	cout << "	-f, --file [FILE]: call script with FILE, default in config.h"<< endl;
	cout << "	-c, --commands [COMMANDS] : call script with COMMANDS"<< endl;
	cout << endl;
}

int main(int argc, char** argv)
{
	srand(time(0));//ne pas enlever



    // Version Démo proposée par Guillaume : les fonctions ci-dessous sont les principaux outils pour interagir avec le réseau

	/*
	//ON PREPARE LES ARGUMENTS DU CONSTRUCTEUR DE ReadNetwork
	int sizes[] = {FIRST_LAYER_SIZE,(int)(1.5*LAST_LAYER_SIZE),LAST_LAYER_SIZE};
	//on l'appelle : ReadNetwork(	nbr de couche,
	//								tableau de leurs tailles,//la dernière couche sera donc le nombre de lettres de l'alphabet (qui peut être plus grand en longueur)
	//								alphabet à reconnaitre,
	//								fonction de transfert, //0 = sigmoide
	//								MAXIMAL_DISTANCE);
	ReadNetwork* rdnk = new ReadNetwork(3,sizes,(char*)CHARS,0,MAXIMAL_DISTANCE);
	rdnk->train();
	rdnk->save("aka00");    //sauvegarde le distance maximal, l'alphabet et le réseau


	ReadNetwork* rr = load("aka00", true);
	rr->train();

	double* input = new double[FIRST_LAYER_SIZE];

	filtres("test/","test_texts/",true);    //mettez vos exemples à vous

    cout << "c'est un " << rr->test("lettre.txt","test_texts/") << endl;
    cout << "c'est un " << rdnk->test("lettre.txt","test_texts/") << endl;*/







	if (argc < 2)
		#ifndef NO_GRAPHIC
		graphics();
		#else
		help();
		#endif //NO_GRAPHIC

	else
	{
		for (int i = 1; i < argc; ++i)
		{
			if ((string)argv[i] == "-h" || (string)argv[i] == "--help")
				help();

			else if ((string)argv[i] == "-f" || (string)argv[i] == "--file")
			{
				ifstream file(NAME_SCRIPT_FILE);
				if (i + 1 < argc)
					ifstream file(argv[i+1]);

				if (file)
					scriptFile(file);
				else
					cout << "Pas de script a executer." << endl;
			}

			else if ((string)argv[i] == "-c" || (string) argv[i] == "--commands")
			{
				if (i + 1 < argc)
				{
					string cmds[argc - i -1];
					for (int j = 0; j < argc-i-1; ++j)
						cmds[j] = (string)argv[i+j+1];
					commands(argc - i -1,cmds);
				}
				else
					help();
			}
		}
	}


	return 0;
}
