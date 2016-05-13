#include "database.h"
#include "filtres2.h"


void database(bool sauvegarde, bool filtre)
{
    int **pixelsR = NULL;
    int **pixelsG = NULL;
    int **pixelsB = NULL;
	pixelsR = (int**)malloc(TAILLEMAX*sizeof(int*));
	pixelsG = (int**)malloc(TAILLEMAX*sizeof(int*));
	pixelsB = (int**)malloc(TAILLEMAX*sizeof(int*));
    for (int i = 0; i < TAILLEMAX; i++)
    {
        pixelsR[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
        pixelsG[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
        pixelsB[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
    }


    int nombrePolices = 0, compteurPolices = 0;
    DIR *dp;
    struct dirent *ep;

    dp = opendir(DOSSIERPOLICES);
    if (dp != NULL)
    {
        while ((ep = readdir(dp)))
        {
            if (strlen(ep->d_name) >= 4)
                nombrePolices++;
        }
        (void) closedir (dp);
    }


    dp = opendir(DOSSIERPOLICES);
    if (dp != NULL && nombrePolices != 0)
    {
        while ((ep = readdir(dp)))
        {
            if (strlen(ep->d_name) >= 4)
            {
                cout << endl << "Chargement : " << 100*compteurPolices/nombrePolices << "% - ";
                cout << ep->d_name << " : traitement ... " << flush;
                filterPngs(ep->d_name,sauvegarde, filtre, pixelsR, pixelsG, pixelsB);
                compteurPolices++;
                cout << endl << "Traitee." << endl;
            }
        }
        (void) closedir (dp);
        cout << "Chargement : " << 100*compteurPolices/nombrePolices << "% - Programme termine !" << endl;
    }


    for (int i = 0; i < 10000; i++)
        {
            free(pixelsR[i]);
            free(pixelsG[i]);
            free(pixelsB[i]);
        }
        free(pixelsR);
        free(pixelsG);
        free(pixelsB);


}

void filterPngs(char* file, bool sauvegarde, bool filtre, int **pixelsR, int **pixelsG, int **pixelsB)
{
    int i = 0;
    char caracteres[CHARNUMBER][2] = CHARS;
    char fontname[100] = "";
    strcpy(fontname,DOSSIERPOLICES);
    strcat(fontname,file);

    SDL_Surface *texte = NULL;
    SDL_Surface *background = SDL_CreateRGBSurface(SDL_SWSURFACE, 400, 400, 32, 0, 0, 0, 0);
    SDL_Rect dest;
    SDL_FillRect(background, NULL, SDL_MapRGB(background->format, BACKGROUNDCOLOR));
    SDL_Color color = {TEXTCOLOR};

    TTF_Font *font = TTF_OpenFont(fontname, TEXTSIZE);

    if(font == NULL)
        {
            ofstream file("erreur.txt");
            file << "Database - Bug 2 : La police " << fontname << " n'a pas pu etre chargee." << endl;
            cout << "Database - Bug 2 : La police " << fontname << " n'a pas pu etre chargee." << endl;
        }
    else
    {
        cout << "Police chargee !" << endl;
        cout << "Creation des images : ";
        for (i = 0; i < CHARNUMBER; i++)
        {
            cout << caracteres[i] << " " << flush;
            strcpy(fontname,DOSSIERIMAGES);
            strcat(fontname,caracteres[i]);
            if (i < 26)
                strcat(fontname,"_U_");
            else
                strcat(fontname,"_L_");

            strcat(fontname,file);
            fontname[strlen(fontname)-4] = '\0';
            strcat(fontname,".png");

            texte = TTF_RenderText_Solid(font, caracteres[i], color);

            dest.x = background->w / 2 - texte->w / 2;
            dest.y = background->h / 2 - texte->h / 2;
            dest.w = texte->w;
            dest.h = texte->h;
            SDL_BlitSurface(texte,NULL,background,&dest);

            if (background == NULL)
            {
                ofstream file("erreur.txt");
                file << "Database - Bug 2 : L'image " << fontname << " n'a pas pu etre creee." <<endl;
                cout << "Database - Bug 2 : L'image " << fontname << " n'a pas pu etre creee." <<endl;
            }
            else
            {

                if (sauvegarde)
                    IMG_SavePNG(background,fontname);

                strcpy(fontname,caracteres[i]);
                if (i < 26)
                    strcat(fontname,"_U_");
                else
                    strcat(fontname,"_L_");

                strcat(fontname,file);
                fontname[strlen(fontname)-4] = '\0';
                strcat(fontname,".png");

                if (filtre)
                    filtres_indiv(fontname, pixelsR, pixelsG, pixelsB, background);

                SDL_FillRect(background, NULL, SDL_MapRGB(background->format, BACKGROUNDCOLOR));
            }
        }
    }

    SDL_FreeSurface(texte);
    SDL_FreeSurface(background);

}
