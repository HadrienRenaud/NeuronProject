#include "menu.h"


using namespace std;

const char g_alphabet[LENGTH_ALPHABET] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

void menu(SDL_Renderer *ren)
{
    bool quitLoop = false;
    bool testing = false;
    int xMouse(0),yMouse(0);
    int compteurTest = 0;
    int nombreTests = 0;
    int posRep = 0;
    char testedImageName[150] = "";
    char testedImageNameFull[200] = "";
    char testedImageText[150] = "";

    SDL_Color color = {255,255,255};
    char testResult[] = "This is a A";

    DIR *dp = NULL;
    struct dirent *ep;

    SDL_Event event;
    SDL_Surface *backgroundSurface = IMG_Load("resources/background.png");
    SDL_Texture *background = SDL_CreateTextureFromSurface(ren,backgroundSurface);


    SDL_Surface *loadingText = TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",80), "Loading ...", color);
    SDL_Texture *loading = SDL_CreateTextureFromSurface(ren,loadingText);

    Button exitButton(ren,"Exit",16,235,680,530,100,30);
    Button databaseButton(ren,"Create images",16,235,20,440,200,30);
    Button filterButton(ren,"Filter images",16,235,20,490,200,30);
    Button learnButton(ren,"Learn",16,235,20,530,200,30);
    Button testButton(ren,"Test",24,235,300,500,200,60);
    Button caseDatabase(ren, "resources/option1.png", "resources/option2.png", true, 25, 473);
    Button caseLearn(ren, "resources/option1.png", "resources/option2.png", true, 25, 563);

    const int buttonsNumber = 7;
    Button* allButtons[buttonsNumber] = {&exitButton, &databaseButton, &filterButton, &learnButton, &testButton, &caseDatabase, &caseLearn};

    SDL_Texture *textDatabase = SDL_CreateTextureFromSurface(ren,TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",12), "Filter without saving images", color));
    SDL_Texture *textLearn = SDL_CreateTextureFromSurface(ren,TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",12), "Start from last save", color));



    SDL_Rect testTarget,testingField;
    SDL_Rect nameImageTestedPos, resultTestPos;
    int heightNameImageTested = 25;

    SDL_Surface *testedImage = NULL;
    SDL_Surface *resultTest = NULL;
    SDL_Surface *nameImageTested = NULL;
    SDL_Surface *resultModel = emptyButton(4,TESTING_BACKGROUND_DIMENSIONS + 2*heightNameImageTested,TESTING_BACKGROUND_DIMENSIONS + heightNameImageTested + 40 + 10);
    SDL_Surface *result = emptyButton(4,TESTING_BACKGROUND_DIMENSIONS + 2*heightNameImageTested,TESTING_BACKGROUND_DIMENSIONS + heightNameImageTested + 40 + 10);
    SDL_Texture *resultTex = NULL;

    SDL_SetTextureBlendMode(resultTex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(resultTex,235);

    testingField.x = backgroundSurface->w/2 - resultModel->w/2;
    testingField.y = 30;

    resultTestPos.x = resultModel->w / 2 - (TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",30), testResult,color))->w/2;

    nameImageTestedPos.y = 5;
    resultTestPos.y = heightNameImageTested + TESTING_BACKGROUND_DIMENSIONS + 5;

    Button nextButton(ren,"Next",16,235,(backgroundSurface->w/2 + resultModel->w/2 - 100), (testingField.y + resultModel->h),100,30);
    Button previousButton(ren,"Previous",16,235,(backgroundSurface->w/2 - resultModel->w/2), (testingField.y + resultModel->h),100,30);



    renderTexture(ren,background,0,0);
    renderTexture(ren,loading,(400 - loadingText->w / 2) + 30, 50);
    SDL_RenderPresent(ren);

    cout << "Recuperation des reseaux ... " << flush;
    Network** tablo_net(createurs_reseaux());

    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        plus_recent(tablo_net[i],g_alphabet[i]);

    cout << "Reseaux recuperes." << endl;

    double input[FIRST_LAYER_SIZE];

    while (!quitLoop)
    {
        SDL_RenderClear(ren);
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quitLoop = true;
                break;
            case SDL_MOUSEMOTION:
                xMouse = event.motion.x;
                yMouse = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:

                for (int i = 0; i < buttonsNumber; i++)
                    allButtons[i]->pressDown(xMouse,yMouse);
                nextButton.pressDown(xMouse,yMouse);
                previousButton.pressDown(xMouse,yMouse);

                break;
            case SDL_MOUSEBUTTONUP:

                for (int i = 0; i < buttonsNumber; i++)
                    allButtons[i]->pressUp(xMouse,yMouse);
                nextButton.pressUp(xMouse,yMouse);
                previousButton.pressUp(xMouse,yMouse);

                break;

                // TODO : METTRE DES RACCOURCIS CLAVIERS
                // TODO : Interompre l'affichage quand on interrompt le test
        }


        renderTexture(ren,background,0,0);
        renderTexture(ren,textDatabase,40,473);
        renderTexture(ren,textLearn,40,563);

        for (int i = 0; i < buttonsNumber; i++)
            allButtons[i]->renderButton(ren,xMouse,yMouse);


        if (testing && previousButton.hasBeenPressed())
        {
            previousButton.reset();
            if (compteurTest > 1)
            {
                rewinddir(dp);
                for (posRep = 0; posRep < compteurTest -1; posRep++)
                    readdir(dp);

                compteurTest -= 2;

                nextButton.setPress(true);
            }
        }

        if (testing && nextButton.hasBeenPressed())
        {
            nextButton.reset();
            ep = readdir(dp);
            compteurTest++;

            if (ep != NULL && compteurTest < nombreTests)
            {
                while(strlen(ep->d_name) < 4)
                {
                    ep = readdir(dp);
                    compteurTest++;
                }

                strcpy(testedImageName,ep->d_name);
                strcpy(testedImageNameFull,DOSSIERTEST);
                strcat(testedImageNameFull,ep->d_name);
                strcpy(testedImageText,ep->d_name);
                testedImageText[strlen(testedImageText)-4]='\0';    //Celui correspondant au fichier texte est coupé 4 caractères avant la fin (on tronque le .png)
                strcat(testedImageText,".txt");   //Puis on lui ajoute ".txt"

                testedImage = IMG_Load(testedImageNameFull);

                if (testedImage == NULL)
                {
                    ofstream file("erreur.txt",ofstream::ate);
                    file << "Menu - Bug 1 : L'image " << testedImageNameFull << " n'a pas pu être ouverte." << endl;
                    cout << "Menu - Bug 1 : L'image " << testedImageNameFull << " n'a pas pu être ouverte." << endl;
                }
                else
                {
                    if (testedImage->w < testedImage->h)
                    {
                        testTarget.h = TESTING_BACKGROUND_DIMENSIONS;
                        testTarget.w = TESTING_BACKGROUND_DIMENSIONS*testedImage->w/testedImage->h;
                        testTarget.x = result->w/2 - testTarget.w/2;
                        testTarget.y = heightNameImageTested;
                    }
                    else
                    {
                        testTarget.h = TESTING_BACKGROUND_DIMENSIONS*testedImage->h/testedImage->w;
                        testTarget.w = TESTING_BACKGROUND_DIMENSIONS;
                        testTarget.x = result->w/2 - TESTING_BACKGROUND_DIMENSIONS/2;
                        testTarget.y = heightNameImageTested + TESTING_BACKGROUND_DIMENSIONS - testTarget.h;
                    }

                    testedImageName[strlen(testedImageName)-4]='\0'; //On enlève le .png
                    strcat(testedImageName,".txt"); // On ajoutee le .txt

                    cout << "testedImageName : " << testedImageName << endl;
                    if(readExemple(testedImageText,input,FIRST_LAYER_SIZE,DOSSIERTESTTEXT))
                        testResult[strlen(testResult) - 1] = tester(tablo_net,input);
                    else
                        testResult[strlen(testResult) - 1] = '_';
                    cout << "testResult : " << testResult << endl;

                    resultTest = TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",30), testResult, color);

                    nameImageTested = TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf",15), ep->d_name, color);
                    nameImageTestedPos.x = result->w/2 - nameImageTested->w/2;


                    SDL_BlitSurface(resultModel,NULL,result,NULL);
                    SDL_BlitScaled(testedImage,NULL,result,&testTarget);
                    SDL_BlitSurface(nameImageTested,NULL,result,&nameImageTestedPos);
                    SDL_BlitSurface(resultTest,NULL,result,&resultTestPos);

                    resultTex = SDL_CreateTextureFromSurface(ren,result);
                }
            }
            else
            {
                testing = false;
                closedir(dp);
            }
        }

        if (testing)
        {
            renderTexture(ren,resultTex,testingField.x,testingField.y);
            nextButton.renderButton(ren,xMouse,yMouse);
            previousButton.renderButton(ren,xMouse,yMouse);
        }


        if (exitButton.hasBeenPressed())
        {
            exitButton.reset();
            quitLoop = true;
        }
        if (databaseButton.hasBeenPressed())
        {
            databaseButton.reset();
            renderTexture(ren,loading,(400 - loadingText->w / 2) + 30, 50);
            SDL_RenderPresent(ren);
            database(!caseDatabase.hasBeenPressed(),true);
        }
        else if (filterButton.hasBeenPressed())
        {
            filterButton.reset();
            renderTexture(ren,loading,(400 - loadingText->w / 2) + 30, 50);
            SDL_RenderPresent(ren);
            filtres();
        }
        else if (learnButton.hasBeenPressed())
        {
            learnButton.reset();
            renderTexture(ren,loading,(400 - loadingText->w / 2) + 30, 50);
            SDL_RenderPresent(ren);

            if (!caseLearn.hasBeenPressed()){
                tablo_net = apprend_nouveau();
            }
            else
                apprend_tous(tablo_net);
        }

        else if (testButton.hasBeenPressed())
        {
            testButton.reset();
            renderTexture(ren,loading,(400 - loadingText->w / 2) + 30, 50);

            testing = true;
            nextButton.setPress(true);

            compteurTest = 0;
            nombreTests = countExemples(DOSSIERTEST);

            dp = opendir(DOSSIERTEST);
            if (dp == NULL)
                exit(1);

            SDL_RenderPresent(ren);
            filtres(DOSSIERTEST,DOSSIERTESTTEXT);
        }

        else
            SDL_RenderPresent(ren);
    }
    destructeur_reseaux(tablo_net);
}
