/*
   --- PROGRAMME DE FILTRES --- VERSION 2.0 ---
   Voir la description dans filtres2.h
 */


#include "filtres2.h"

void filtres(const char* repertory_dep, const char* repertory_arr, bool selectif)
{
	int nombreimages = 0, compteurimages = 0;

	DIR *   dp;
	struct dirent * ep;
	SDL_Surface * lettre;

	char pathPng[150];                                                         //Le nom du répertoire d'images
	char pathTxt[150];                                                         //Le nom du répertoire où seront créés les fichiers textes


	//Allocation dynamique d'espace et création des trois tableaux 2D qui contiendront les composantes rouges, vertes, bleues, de chaque pixel de l'image
	//Je dois pas être très doué mais j'ai pas réussi à créer un seul tableau 3D où chaque case contient trois sous-cases avec les composantes
	//Attention : dans l'état actuel, le programme ne peut pas traiter le cas d'images dont une des dimensions est plus grande que 10000 pixels
	int ** pixelsR = NULL;
	int ** pixelsG = NULL;
	int ** pixelsB = NULL;

	//On réserve pour chaque tableau 10000 cases mémoire de la taille d'un tableau ...
	pixelsR = (int**)malloc(TAILLEMAX * sizeof(int*));
	pixelsG = (int**)malloc(TAILLEMAX * sizeof(int*));
	pixelsB = (int**)malloc(TAILLEMAX * sizeof(int*));
	for (int i = 0; i < TAILLEMAX; i++)
	{
		//... qui occupe lui même 10000 cases
		pixelsR[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
		pixelsG[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
		pixelsB[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
	}


	//On compte le nombre d'images dans le répertoire
	dp = opendir(repertory_dep);                                                            //On ouvre le répertoire contenant les images
	if (dp != NULL)                                                                //Si l'ouverture du répertoire a fonctionné
	{
		while ((ep = readdir(dp)))                                                                                                                 //On parcourt le répertoire
		{
			if (strlen(ep->d_name) >= 4)                                                                                                                                                                         //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers)

				if (!selectif || !dejaFiltree(repertory_arr, ep->d_name))                                                                                                                                                                                                                                 //Il y a évaluation paresseuse : pas d'appels inutiles à déjaFiltree
					nombreimages++;
		}
		(void)closedir(dp);                                                                                                                 //Fermeture du répertoire d'images
	}



	dp = opendir(repertory_dep);                                                         //On parcourt le répertoire une deuxième fois, avec traitement de chaque image
	if (dp != NULL && nombreimages != 0)
	{
		while ((ep = readdir(dp)))
		{
			if (strlen(ep->d_name) >= 4)
			{
				if (!selectif || !dejaFiltree(repertory_arr, ep->d_name))
				{
					cout << "Chargement : " << 100 * compteurimages / nombreimages << "% - ";
					cout << ep->d_name << " : traitement ... " << flush;

					pathNames(pathPng, pathTxt, ep->d_name, repertory_dep, repertory_arr);

					lettre = IMG_Load(pathPng);                                                                                                                                                                                                                                                                                         //On charge dans une surface SDL l'image à laquelle on accède par le chemin relatif dossierPng
					if (lettre == NULL)                                                                                                                                                                                                                                                                                         //Si il y a eu un souci d'ouverture de l'image, on écrit une erreur dans erreur.txt
					{
						ofstream file("erreur.txt", ofstream::ate);
						file << "Filtres2 - Bug 1 : L'image " << pathPng << " n'a pas pu etre ouverte." << endl;
						cout << "Filtres2 - Bug 1 : L'image " << pathPng << " n'a pas pu etre ouverte." << endl;
					}
					else
						filtres_indiv(lettre, pathTxt, pixelsR, pixelsG, pixelsB);

					compteurimages++;
					cout << " Traitee." << endl;
				}
			}
		}
		(void)closedir(dp);                                                                                                                 //Fermeture du répertoire d'images
		cout << "Chargement : " << 100 * compteurimages / nombreimages << "% - Programme termine !" << endl << endl;
	}


	//On libère tout l'espace alloué pour les tableaux de pixels
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



void filtres_indiv(SDL_Surface *lettre, char* pathTxt, int **pixelsR, int **pixelsG, int **pixelsB)
{
	bool notEmpty = true;
	bool needsManualResize;

	double final[TAILLE][TAILLE] = {{0}};

	int distancemax[3];                                                         //Le max de la "distance chromatique" séparant les pixels du pixel de référence (voir plus bas)
	int reference [3] = { 0 };                                                         //La "référence" de couleur, qui sera définie comme la couleur du pixel de coordonnées (0,0)
	int marges[4]  = { 0 };

	FILE*   fichier   = NULL;                                                         //Un pointeur de fichier

	SDL_Surface * resized   = NULL;

	fichier = fopen(pathTxt, "w+");                                                         //On ouvre (ou on créé) un fichier txt

	if (fichier == NULL)                                                         //Si l'ouverture du fichier texte a échoué, on inscrit une erreur dans erreur.txt
	{
		ofstream file("erreur.txt", ofstream::ate);
		file << "Filtres2 - Bug 2 : Le fichier " << pathTxt << " n'a pas pu etre ouvert." << endl;
		cout << "Filtres2 - Bug 2 : Le fichier " << pathTxt << " n'a pas pu etre ouvert." << endl;
	}
	else                                                         //Si l'ouverture a réussi

	{
		analysePixel(lettre, pixelsR, pixelsG, pixelsB);
		distanceChro(pixelsR, pixelsG, pixelsB, lettre->w, lettre->h, reference, distancemax);

		//On calcule les marges : on rogne le plus possible de lignes/colonnes contenant uniquement des pixels de la couleur de référence
		margeLigne(pixelsR, pixelsG, pixelsB, lettre->w, lettre->h, reference, marges);
		margeColonne(pixelsR, pixelsG, pixelsB, lettre->w, lettre->h, reference, marges);
		notEmpty   = margeSynthese(lettre->w, lettre->h, marges);                                                                                                                 //on rend l'image carrée en diminuant alternativement les marges gauche/droite ou haut/bas selon si la hauteur est plus grande que la largeur ou non

		needsManualResize = (lettre->w - marges[2] - marges[3] >= MANUAL_RESIZE_MULTIPLIER * TAILLE);

		if (!needsManualResize)
		{
			resized = SDL_CreateRGBSurface(SDL_SWSURFACE, TAILLE, TAILLE, 32, 0, 0, 0, 255);
			resize(lettre, resized, reference, marges);

			finalMatrix(resized, final, reference, marges, distancemax);
			printingTxt(fichier, final, notEmpty);
		}
		else
		{
			resized = SDL_CreateRGBSurface(SDL_SWSURFACE, MANUAL_RESIZE_MULTIPLIER * TAILLE, MANUAL_RESIZE_MULTIPLIER * TAILLE, 32, 0, 0, 0, 255);
			resize(lettre, resized, reference, marges);

			manualResizeFinalMatrix(resized, final, reference, marges, distancemax);

			printingTxt(fichier, final, notEmpty);
		}

		fclose(fichier);
	}


}

void pathNames(char* pathPng, char* pathTxt, char* namePng, const char* repertory_dep, const char* repertory_arr)
{
	char nameTxt[100] = "";                                                         //Le nom du fichier texte associé

	//Traitement des noms de fichiers : le but est de créer un fichier txt de même nom que l'image traitée, et de le place dans le sous dossier "textes"
	strcpy(pathPng, repertory_dep);
	strcat(pathPng, namePng);                                                         //On obtient le chemin relatif menant à l'image en concaténant "images/" et le nom de l'image

	strcpy(nameTxt, namePng);
	nameTxt[strlen(nameTxt) - 4] = '\0';                                                         //Celui correspondant au fichier texte est coupé 4 caractères avant la fin (on tronque le .png)
	strcat(nameTxt, ".txt");                                                         //Puis on lui ajoute ".txt"

	strcpy(pathTxt, repertory_arr);
	strcat(pathTxt, nameTxt);

}


Uint32 getPixel(SDL_Surface * surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 * p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16*)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32*)p;

	default:
		return 0;
	}
}


void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	/*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
	   En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
	   de l'image : 8, 16, 24 ou 32 bits.*/
	int nbOctetsParPixel = surface->format->BytesPerPixel;
	/*Ici p est l'adresse du pixel que l'on veut modifier*/
	/*surface->pixels contient l'adresse du premier pixel de l'image*/
	Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

	/*Gestion différente suivant le nombre d'octets par pixel de l'image*/
	switch (nbOctetsParPixel) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		/*Suivant l'architecture de la machine*/
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;
	}
}



void margeLigne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[])
{
	marges[0] = 0;
	marges[1] = 0;

	int interrA = 0;
	int interrB = 0;
	int x  = 0, y = 0;

	for (y = 0; y < hauteur; y++)
	{
		interrB = 0;

		for (x = 0; x < largeur; x++)
			if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
				interrB = 1;

		if (interrB == 1)
			interrA = 1;
		else if (interrA == 0)
			marges[0]++;
	}

	interrA = 0;

	for (y = hauteur - 1; y >= 0; y--)
	{
		interrB = 0;

		for (x = 0; x < largeur; x++)
			if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
				interrB = 1;

		if (interrB == 1)
			interrA = 1;
		else if (interrA == 0)
			marges[1]++;
	}



}

void margeColonne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[])
{
	marges[2] = 0;
	marges[3] = 0;

	int interrA = 0;
	int interrB = 0;
	int x  = 0, y = 0;

	for (x = 0; x < largeur; x++)
	{
		interrB = 0;

		for (y = 0; y < hauteur; y++)
			if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
				interrB = 1;

		if (interrB == 1)
			interrA = 1;
		else if (interrA == 0)
			marges[2]++;
	}

	interrA = 0;

	for (x = largeur - 1; x >= 0; x--)
	{
		interrB = 0;

		for (y = 0; y < hauteur; y++)
			if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
				interrB = 1;

		if (interrB == 1)
			interrA = 1;
		else if (interrA == 0)
			marges[3]++;
	}
}

bool margeSynthese(int imageWidth, int imageHeight, int marges[])
{
	int hauteur = imageHeight - marges[0] - marges[1];
	int largeur = imageWidth - marges[2] - marges[3];

	if (hauteur <= 0 || largeur <= 0)                                                         //Dans le cas où l'une des dimensions est négative, cela signifie que l'image a été totalement rognée : on n'inscrit alors que des -1 dans le fichier textes
		return false;

	else
	{
		int i = 0;

		while (hauteur < largeur)
		{
			if (i == 0)
			{
				i = 1;
				marges[0]--;
			}
			else
			{
				i = 0;
				marges[1]--;
			}
			hauteur = imageHeight - marges[0] - marges[1];
		}
		//Dans le cas où l'une des marges est négative, cela signifie que rendre l'image carrée et centrée sur la lettre est impossible sans dépasser de l'image totale
		//On diminue alors l'autre marge d'autant que nécessaire pour rendre nulle celle qui était négative (super clair comme phrase)
		if (marges[0] < 0)
		{
			marges[1] += marges[0];
			marges[0] = 0;
		}
		if (marges[1] < 0)
		{
			marges[0] += marges[1];
			marges[1] = 0;
		}

		hauteur = imageHeight - marges[0] - marges[1];

		while (largeur < hauteur)
		{
			if (i == 0)
			{
				i = 1;
				marges[2]--;
			}
			else
			{
				i = 0;
				marges[3]--;
			}
			largeur = imageWidth - marges[2] - marges[3];
		}
		if (marges[2] < 0)
		{
			marges[3] += marges[2];
			marges[2] = 0;
		}
		if (marges[3] < 0)
		{
			marges[2] += marges[3];
			marges[3] = 0;
		}

		largeur = imageWidth - marges[2] - marges[3];
		//A ce stade, largeur = hauteur, et si l'image initiale le permettait, les marges sont telles qu'une fois rognées, on obtient une image carrée centrée sur la lettre

		return true;
	}
}




void analysePixel(SDL_Surface *image, int **pixelsR, int **pixelsG, int **pixelsB)
{
	Uint8 red, green, blue;
	Uint32 infopixel;

	SDL_LockSurface(image);                                                         //On verouille la surface SDL (indispensable pour lire les pixels)
	for (int y = 0; y < image->h; y++)                                                         //On parcourt tous les pixels de l'image
	{
		for (int x = 0; x < image->w; x++)
		{
			//On récupère les infos d'un pixel dans infopixel puis ses composantes dans red, green, blue
			infopixel = getPixel(image, x, y);
			SDL_GetRGB(infopixel, image->format, &red, &green, &blue);
			//On place le tout dans les tableaux correspondants
			pixelsR[y][x] = (int)red;
			pixelsG[y][x] = (int)green;
			pixelsB[y][x] = (int)blue;
		}
	}

	SDL_UnlockSurface(image);                                                         //On déverouille la surface

}

void distanceChro(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int distancemax[])
{
	for (int i = 0; i < 3; i++)                                                         //On réinitialise la distance chromatique
		distancemax[i] = 0;

	reference[0] = pixelsR[0][0];
	reference[1] = pixelsG[0][0];
	reference[2] = pixelsB[0][0];
	int red, green, blue;

	for (int y = 0; y < hauteur; y++)                                                         //On parcourt tous les pixels de l'image
	{
		for (int x = 0; x < largeur; x++)
		{
			red  = pixelsR[y][x];
			green = pixelsG[y][x];
			blue = pixelsB[y][x];

			//Calcul de la distance chromatique à la référence maximale : si le pixel considéré possède une distance chromatique moyenne à la référence supérieure à la moyenne des distancemax[i], on actualise distancemax
			if (distancemax[0] + distancemax[1] + distancemax[2] < abs(red - reference[0]) + abs(green - reference[1]) + abs(blue - reference[2]))
			{
				distancemax[0] = abs(red - reference[0]);
				distancemax[1] = abs(green - reference[1]);
				distancemax[2] = abs(blue - reference[2]);
			}
		}
	}
}


bool dejaFiltree(const char* repertory_arr, const char* imageName)
{
	char textName[200];

	strcpy(textName, repertory_arr);
	strcat(textName, imageName);
	textName[strlen(textName) - 4] = '\0';
	strcat(textName, ".txt");

	ifstream file(textName);

	return (bool)file;                                                         //Booléen correspondant au succès de l'ouverture du fichier : vaut vrai si le fichier a été trouvé, faux sinon.
}


void resize(SDL_Surface *lettre, SDL_Surface *resized, int reference[], int marges[])
{
	SDL_Rect dest;

	dest.x = marges[2];
	dest.y = marges[0];
	dest.w = lettre->w - marges[3] - marges[2];
	dest.h = lettre->h - marges[1] - marges[0];

	SDL_FillRect(resized, NULL, SDL_MapRGB(resized->format, reference[0], reference[1], reference[2]));
	SDL_BlitScaled(lettre, &dest, resized, NULL);

}

void manualResizeFinalMatrix(SDL_Surface * image, double final[TAILLE][TAILLE], int reference[], int marges[], int distancemax[])
{
	Uint8 color[3];
	int colorAverage[3];
	double moyenne = 0;

    SDL_LockSurface(image);
    for (int y = 0; y < TAILLE; y++)
    {
        for (int x = 0; x < TAILLE; x++)
        {
            for (int k = 0; k < 3; k++)
                colorAverage[k] = 0;

            for (int j = 0; j < MANUAL_RESIZE_MULTIPLIER; j++)
            {
                for (int i = 0; i < MANUAL_RESIZE_MULTIPLIER; i++)
                {
                    SDL_GetRGB(getPixel(image, x * MANUAL_RESIZE_MULTIPLIER + i, y * MANUAL_RESIZE_MULTIPLIER + j), image->format, &color[0], &color[1], &color[2]);
                    for (int k = 0; k < 3; k++)
                        colorAverage[k] += (int)color[k];
                }
            }

            for (int k = 0; k < 3; k++)
                colorAverage[k] /= (MANUAL_RESIZE_MULTIPLIER * MANUAL_RESIZE_MULTIPLIER);

            //On calcule le rapport de (la distance chromatique à la référence du pixel) et de (distancemax), la distance chromatique maximale
            moyenne = abs(colorAverage[0] - reference[0]) + abs(colorAverage[1] - reference[1]) + abs(colorAverage[2] - reference[2]);
            moyenne /= (distancemax[0] + distancemax[1] + distancemax[2]);

            final[y][x] = moyenne;                                                                                                                                                                                                                          //On écrit le résultat avec 2 décimales

        }
    }

    SDL_UnlockSurface(image);
}


void finalMatrix(SDL_Surface * resized, double final[TAILLE][TAILLE], int reference[], int marges[], int distancemax[])
{

	Uint8 red, green, blue;
	double moyenne = 0;


    SDL_LockSurface(resized);                                                                                                                 //On verrouille la surface SDL (indispensable pour lire les pixels)

    for (int y = 0; y < resized->h; y++)                                                                                                                 //On parcourt tous les pixels de l'image
    {
        for (int x = 0; x < resized->w; x++)
        {
            SDL_GetRGB(getPixel(resized, x, y), resized->format, &red, &green, &blue);

            //On calcule le rapport de (la distance chromatique à la référence du pixel) et de (distancemax), la distance chromatique maximale
            moyenne = abs((int)red - reference[0]) + abs((int)green - reference[1]) + abs((int)blue - reference[2]);
            moyenne /= (distancemax[0] + distancemax[1] + distancemax[2]);

            final[y][x] = moyenne;

        }
    }

    SDL_UnlockSurface(resized);                                                                                                                 //On déverrouille la surface

}


void printingTxt(FILE * fichier, double final[TAILLE][TAILLE], bool notEmpty)
{


	if (!notEmpty)
	{
		for (int y = 0; y < TAILLE; y++)
		{
			for (int x = 0; x < TAILLE; x++)
				fprintf(fichier, "0.00 ");

			fprintf(fichier, "\n ");
		}
		fprintf(fichier, "0");
	}
	else
	{
		for (int y = 0; y < TAILLE; y++)                                                                                                                 //On parcourt tous les pixels de l'image
		{
			for (int x = 0; x < TAILLE; x++)
				fprintf(fichier, "%.2f ", final[y][x]);                                                                                                                                                                                                                                 //On écrit le résultat avec 2 décimales

			fprintf(fichier, "\n ");                                                                                                                                                                         //Au bout d'une ligne de pixels, on revient à la ligne avant d'écrire la ligne suivante
		}
		fprintf(fichier, "%d", nbConnectedComponentMatrix(final));

	}
}

/*
   void pixelsfinal(int **pixelsR, int pixelsRfinal[20][20], int Largeur, int Hauteur, int marges[])
   {
    int x = 0,y = 0, i = 0, j = 0;
    int somme = 0;
    int res = 0;
    double a = (Largeur - marges[2] - marges[3])/TAILLE;
    double b = (Hauteur - marges[0] - marges[1])/TAILLE;
    int u = marges[0], v = marges[2];

    for (y = 0; y < TAILLE; y++)
    {
        for (x = 0; x < TAILLE; x++)
        {
            somme = 0;
            res = 0;
            for (j = floor(y*b)+1; j < (y+1)*b - 1; j++)
            {
                for (i = floor(x*a)+1; i < (x+1)*a - 1; i++)
                    {somme +=pixelsR[u+j][v+i];
                    res += 1;}
            }
            for (i = floor(x*a)+1; i < (x+1)*a - 1; i++)
                {
                    somme += (floor(y*b)+1 - y*b)*pixelsR[u+(int)floor(y*b)][v+i];
                    if (y != TAILLE - 1)
                    somme += ((y+1)*b - floor((y+1)*b) + 1)*pixelsR[u+(int)floor((y+1)*b)][v+i];
                    else
                    somme += pixelsR[u+(int)((y+1)*b - 1)][v+i];
                    res += (floor(y*b)+1 - y*b) + ((y+1)*b - floor((y+1)*b) + 1);
                }
            for (j = floor(y*b)+1; j < (y+1)*b - 1; j++)
                {
                    somme += (floor(x*a)+1 - x*a)*pixelsR[u+j][v+(int)floor(x*a)];
                    if (x != TAILLE - 1)
                    somme += ((x+1)*a - floor((x+1)*a) + 1)*pixelsR[u+j][v+(int)floor((x+1)*a)];
                    else
                    somme +=pixelsR[u+j][v+(int)((x+1)*a - 1)];
                    res += (floor(x*a)+1 - x*a) + ((x+1)*a - floor((x+1)*a) + 1);
                }

            somme += (floor(x*a)+1 - x*a) * (floor(y*b)+1 - y*b) * pixelsR[u+(int)floor(y*b)][v+(int)floor(x*a)];
            if (x != TAILLE - 1)
            somme += ((x+1)*a - floor((x+1)*a) + 1) * (floor(y*b)+1 - y*b) * pixelsR[u+(int)floor(y*b)][v+(int)floor((x+1)*a)];
            else
            somme += (floor(y*b)+1 - y*b) * pixelsR[u+(int)floor(y*b)][v+(int)((x+1)*a - 1)];
            if (y != TAILLE - 1)
            somme += (floor(x*a)+1 - x*a) * ((y+1)*b - floor((y+1)*b) + 1) * pixelsR[u+(int)floor((y+1)*b)][v+(int)floor(x*a)];
            else
            somme += (floor(x*a)+1 - x*a) * pixelsR[u+(int)((y+1)*b - 1)][v+(int)floor(x*a)];
            if (x != TAILLE - 1)
            {
                if (y != TAILLE - 1)
                somme += ((x+1)*a - floor((x+1)*a) + 1) * ((y+1)*b - floor((y+1)*b) + 1) * pixelsR[u+(int)floor((y+1)*b)][v+(int)floor((x+1)*a)];
                else
                somme += ((x+1)*a - floor((x+1)*a) + 1) * pixelsR[u+(int)((y+1)*b - 1)][v+(int)floor((x+1)*a)];
            }

            else
            {
                if (y != TAILLE - 1)
                somme += ((y+1)*b - floor((y+1)*b) + 1) * pixelsR[u+(int)floor((y+1)*b)][v+(int)((x+1)*a - 1)];
                else
                somme += pixelsR[u+(int)((y+1)*b - 1)][v+(int)((x+1)*a - 1)];
            }

            res += (floor(x*a)+1 - x*a) * (floor(y*b)+1 - y*b) + ((x+1)*a - floor((x+1)*a) + 1) * (floor(y*b)+1 - y*b);
            res += (floor(x*a)+1 - x*a) * ((y+1)*b - floor((y+1)*b) + 1) + ((x+1)*a - floor((x+1)*a) + 1) * ((y+1)*b - floor((y+1)*b) + 1);

            pixelsRfinal[y][x] = somme/(a*b);


        }
    }

   }*/
