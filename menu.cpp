#include "menu.h"
#include "ReadNetwork.h"


using namespace std;

void menu(SDL_Renderer *ren)
{
	bool quitLoop     = false;
	bool testing      = false;
	bool keyboardInput[29]   = { false };
	int xMouse(0), yMouse(0);
	int compteurTest    = 0;
	int nombreTests     = 0;
	int posRep      = 0;
	int length_alphabet    = LENGTH_ALPHABET;
	char testedImageName[150]  = "";
	char testedImageNameFull[200] = "";
	char testedImageText[150]  = "";

	SDL_Color color      = { 255, 255, 255 };
	char testResult[]    = "This is a A";

	DIR *   dp       = NULL;
	struct dirent * ep;

	clock_t t0(clock()); //temps de départ du programme
	SDL_Event event;
	SDL_Surface * backgroundSurface = IMG_Load("resources/background.png");
	SDL_Texture * background   = SDL_CreateTextureFromSurface(ren, backgroundSurface);


	SDL_Surface * loadingText = TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", 80), "Loading ...", color);
	SDL_Texture * loading  = SDL_CreateTextureFromSurface(ren, loadingText);

	const int buttonsNumber    = 9;

	Button exitButton(ren, "Exit", 16, 235, 680, 530, 100, 30);
	Button databaseButton(ren, "Create images", 16, 235, 20, 440, 200, 30);
	Button filterButton(ren, "Filter images", 16, 235, 20, 490, 200, 30);
	Button scriptButton(ren, "Execute script", 16, 235, 580, 450, 200, 30);
	Button statisticsButton(ren, "Statistics", 16, 235, 580, 490, 200, 30);
	Button learnButton(ren, "Learn", 16, 235, 20, 530, 200, 30);
	Button testButton(ren, "Test", 24, 235, 300, 500, 200, 60);
	Button caseDatabase(ren, "resources/option1.png", "resources/option2.png", true, 25, 473);
	Button caseLearn(ren, "resources/option1.png", "resources/option2.png", true, 25, 563);

	Button*   allButtons[buttonsNumber] = { &exitButton, &databaseButton, &filterButton, &learnButton, &testButton, &caseDatabase, &caseLearn, &statisticsButton, &scriptButton};

	SDL_Texture * textDatabase    = SDL_CreateTextureFromSurface(ren, TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", 12), "Save filtered images only", color));
	SDL_Texture * textLearn     = SDL_CreateTextureFromSurface(ren, TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", 12), "Start from last save", color));



	SDL_Rect testTarget, testingField;
	SDL_Rect nameImageTestedPos, resultTestPos;
	int heightNameImageTested = 25;

	SDL_Surface * testedImage    = NULL;
	SDL_Surface * resultTest    = NULL;
	SDL_Surface * nameImageTested   = NULL;
	SDL_Surface * resultModel    = emptyButton(4, TESTING_BACKGROUND_DIMENSIONS + 2 * heightNameImageTested, TESTING_BACKGROUND_DIMENSIONS + heightNameImageTested + 40 + 10);
	SDL_Surface * result     = emptyButton(4, TESTING_BACKGROUND_DIMENSIONS + 2 * heightNameImageTested, TESTING_BACKGROUND_DIMENSIONS + heightNameImageTested + 40 + 10);
	SDL_Texture * resultTex    = NULL;

	SDL_SetTextureBlendMode(resultTex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(resultTex, 235);

	testingField.x   = backgroundSurface->w / 2 - resultModel->w / 2;
	testingField.y   = 30;

	resultTestPos.x   = resultModel->w / 2 - (TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", 30), testResult, color))->w / 2;

	nameImageTestedPos.y = 5;
	resultTestPos.y   = heightNameImageTested + TESTING_BACKGROUND_DIMENSIONS + 10;

	Button nextButton(ren, "Next", 16, 235, (backgroundSurface->w / 2 + resultModel->w / 2 - 100), (testingField.y + resultModel->h), 100, 30);
	Button previousButton(ren, "Previous", 16, 235, (backgroundSurface->w / 2 - resultModel->w / 2), (testingField.y + resultModel->h), 100, 30);



	renderTexture(ren, background, 0, 0);
	renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
	SDL_RenderPresent(ren);

	length_alphabet = getLenghtAlphabet();


	int sizes[] = {FIRST_LAYER_SIZE,LAST_LAYER_SIZE};

	ReadNetwork* rdnk = load(string(DOSSIERBACKUP) + string(NOMBACKUP), false);
	if (rdnk == 0){
        cout << "Pas de sauvegarde trouvee, creation d'un reseau vierge." << endl;
        rdnk = new ReadNetwork(2,sizes,(char*)CHARS,0,MAXIMAL_DISTANCE);
    }
    else
        cout << "Chargement de la sauvegarde " << DOSSIERBACKUP << NOMBACKUP << " reussi." << endl;



// Ancienne méthode de création / récupération des réseaux ; obsolète
/*
	cout << "Recuperation des " << length_alphabet <<" reseaux ... " << flush;
	NetworkArray* tablo_net = new NetworkArray(length_alphabet);

	tablo_net->getMostRecent();
	cout << "Reseaux recuperes." << endl;



	double input[FIRST_LAYER_SIZE];*/





	while (!quitLoop)
	{
		SDL_RenderClear(ren);
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			quitLoop = true;
			break;

		case SDL_KEYDOWN:
			if (((float)(clock() - t0) / CLOCKS_PER_SEC) > REFRESH_TIME)
			{
				keyboard(event, keyboardInput);
				t0 = clock();
			}

		case SDL_MOUSEMOTION:
			xMouse = event.motion.x;
			yMouse = event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			for (int i = 0; i < buttonsNumber; i++)
				allButtons[i]->pressDown(xMouse, yMouse);

			nextButton.pressDown(xMouse, yMouse);
			previousButton.pressDown(xMouse, yMouse);
			break;

		case SDL_MOUSEBUTTONUP:
			for (int i = 0; i < buttonsNumber; i++)
				allButtons[i]->pressUp(xMouse, yMouse);
			nextButton.pressUp(xMouse, yMouse);
			previousButton.pressUp(xMouse, yMouse);

			break;

		}


		renderTexture(ren, background, 0, 0);
		renderTexture(ren, textDatabase, 40, 473);
		renderTexture(ren, textLearn, 40, 563);

		for (int i = 0; i < buttonsNumber; i++)
			allButtons[i]->renderButton(ren, xMouse, yMouse);


		if (testing && (previousButton.hasBeenPressed() || keyboardInput[27]))
		{
			keyboardInput[27] = false;
			previousButton.reset();
			if (compteurTest > 1)
			{
				rewinddir(dp);
				for (posRep = 0; posRep < compteurTest - 1; posRep++)
					readdir(dp);

				compteurTest -= 2;

				nextButton.setPress(true);
			}
		}

		if (testing && (nextButton.hasBeenPressed() || keyboardInput[28]))
		{
			keyboardInput[28] = false;
			nextButton.reset();
			ep     = readdir(dp);
			compteurTest++;

			if (ep != NULL && compteurTest - 2 < nombreTests)
			{
				while (strlen(ep->d_name) < 4)
				{
					ep = readdir(dp);
					compteurTest++;
				}

				strcpy(testedImageName, ep->d_name);
				strcpy(testedImageNameFull, DOSSIERTEST);
				strcat(testedImageNameFull, ep->d_name);
				strcpy(testedImageText, ep->d_name);
				testedImageText[strlen(testedImageText) - 4] = '\0';                                                                                                                                                                                                                                 //Celui correspondant au fichier texte est coupé 4 caractères avant la fin (on tronque le .png)
				strcat(testedImageText, ".txt");                                                                                                                                                                                                                                 //Puis on lui ajoute ".txt"

				testedImage = IMG_Load(testedImageNameFull);

				if (testedImage == NULL)
                    err("Menu - Bug 1 : L'image " + string(testedImageNameFull) + " n'a pas pu être ouverte.", 1);

				else
				{
					if (testedImage->w < testedImage->h)
					{
						testTarget.h = TESTING_BACKGROUND_DIMENSIONS;
						testTarget.w = TESTING_BACKGROUND_DIMENSIONS * testedImage->w / testedImage->h;
						testTarget.x = result->w / 2 - testTarget.w / 2;
						testTarget.y = heightNameImageTested;
					}
					else
					{
						testTarget.h = TESTING_BACKGROUND_DIMENSIONS * testedImage->h / testedImage->w;
						testTarget.w = TESTING_BACKGROUND_DIMENSIONS;
						testTarget.x = result->w / 2 - TESTING_BACKGROUND_DIMENSIONS / 2;
						testTarget.y = heightNameImageTested + TESTING_BACKGROUND_DIMENSIONS - testTarget.h;
					}

					testedImageName[strlen(testedImageName) - 4] = '\0';                                                                                                                                                                                                                                                                                         //On enlève le .png
					strcat(testedImageName, ".txt");                                                                                                                                                                                                                                                                                         // On ajoutee le .txt

					cout << endl << "Image testee : " << testedImageName << endl;

					testResult[strlen(testResult) - 1] = rdnk->test(testedImageText,(char*)DOSSIERTESTTEXT);

					cout << "Resultat du test : " << testResult << endl;

					resultTest    = TTF_RenderText_Blended(TTF_OpenFont("resources/font_test.ttf", 30), testResult, color);

					nameImageTested   = TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", 15), ep->d_name, color);
					nameImageTestedPos.x = result->w / 2 - nameImageTested->w / 2;


					SDL_BlitSurface(resultModel, NULL, result, NULL);
					SDL_BlitScaled(testedImage, NULL, result, &testTarget);
					SDL_BlitSurface(nameImageTested, NULL, result, &nameImageTestedPos);
					SDL_BlitSurface(resultTest, NULL, result, &resultTestPos);

					resultTex = SDL_CreateTextureFromSurface(ren, result);
				}
			}
			else
			{
				testing = false;
				closedir(dp);
			}
		}



		if (exitButton.hasBeenPressed() || keyboardInput[26])
		{
			keyboardInput[26] = false;
			exitButton.reset();
			quitLoop   = true;
		}
		else if (databaseButton.hasBeenPressed() || keyboardInput[2])
		{
			keyboardInput[2] = false;
			databaseButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
			SDL_RenderPresent(ren);
			database(!caseDatabase.hasBeenPressed(), caseDatabase.hasBeenPressed());
		}
		else if (filterButton.hasBeenPressed() || keyboardInput[5])
		{
			keyboardInput[5] = false;
			filterButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
			SDL_RenderPresent(ren);
			filtres();
		}
		else if (learnButton.hasBeenPressed() || keyboardInput[11])
		{
			keyboardInput[11] = false;
			learnButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
			SDL_RenderPresent(ren);

			if (!caseLearn.hasBeenPressed())
			{
			    // Obsolète
				//delete tablo_net;
				//NetworkArray* tablo_net = new NetworkArray(length_alphabet);
				//tablo_net->learnAllNetworks();

                delete rdnk;
				rdnk = new ReadNetwork(3,sizes,(char*)CHARS,0,MAXIMAL_DISTANCE);
			}

            rdnk->train();
            rdnk->save(string(DOSSIERBACKUP) + string(NOMBACKUP));
            cout << "Reseau sauvegarde." << endl;

		}
		else if (testButton.hasBeenPressed() || keyboardInput[19])
		{
			keyboardInput[19] = false;
			testButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);

			testing = true;
			nextButton.setPress(true);

			compteurTest = -1;
			nombreTests  = countExemples(DOSSIERTEST);

			dp    = opendir(DOSSIERTEST);
			if (dp == NULL)
				exit(1);


			SDL_RenderPresent(ren);
			filtres(DOSSIERTEST, DOSSIERTESTTEXT, true);
		}
		else if (testing)
		{
			renderTexture(ren, resultTex, testingField.x, testingField.y);
			nextButton.renderButton(ren, xMouse, yMouse);
			previousButton.renderButton(ren, xMouse, yMouse);
			SDL_RenderPresent(ren);
		}
		else if (scriptButton.hasBeenPressed() || keyboardInput[18])
		{
			keyboardInput[18] = false;
			scriptButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
			SDL_RenderPresent(ren);

			ifstream file(NAME_SCRIPT_FILE);
			if (file)
                scriptFile(file);
            else
                cout << "Pas de script a executer." << endl;

		}

		else if (statisticsButton.hasBeenPressed() || keyboardInput[0])
		{
			keyboardInput[0] = false;
			statisticsButton.reset();
			renderTexture(ren, loading, (400 - loadingText->w / 2) + 30, 50);
			SDL_RenderPresent(ren);

			filtres(DOSSIERTEST, DOSSIERTESTTEXT, true);
			cout << "Sur les exemples donnes, le reseau a un taux de reussite de : " << rdnk->testAllExamples() << endl << endl;
		}

		else
			SDL_RenderPresent(ren);
	}

	rdnk->save(string(DOSSIERBACKUP) + string(NOMBACKUP));
  cout << "Reseau sauvegarde." << endl;
	delete rdnk;

	//Obsolète
	/*tablo_net->save();
	delete tablo_net;*/
}




void keyboard(SDL_Event event, bool* keyboardInput)
{
	switch (event.key.keysym.sym) {
	case SDLK_a:
		keyboardInput[0] = true;
		break;

	case SDLK_c:
		keyboardInput[2] = true;
		break;

	case SDLK_f:
		keyboardInput[5] = true;
		break;

	case SDLK_t:
		keyboardInput[19] = true;
		break;

	case SDLK_s:
		keyboardInput[18] = true;
		break;

	case SDLK_l:
		keyboardInput[11] = true;
		break;

	case SDLK_ESCAPE:
		keyboardInput[26] = true;
		break;

	case SDLK_LEFT:
		keyboardInput[27] = true;
		break;

	case SDLK_RIGHT:
		keyboardInput[28] = true;
		break;

	}
}
