/*
--- PROGRAMME DE FILTRES --- VERSION 2.0 ---
Voir la description dans filtres2.h
*/


#include "filtres2.h"

void filtres(){
    filtres(DOSSIERIMAGES);
}
void filtres(const char* repertory_dep){
    filtres(repertory_dep,DOSSIERTEXTES);
}
void filtres(const char* repertory_dep, const char* repertory_arr){
	int nombreimages = 0, compteurimages = 0;
    DIR *dp;
    struct dirent *ep;
    SDL_Surface *lettre = NULL; //Une surface SDL, structure la plus utilisée de la SDL


    //Allocation dynamique d'espace et création des trois tableaux 2D qui contiendront les composantes rouges, vertes, bleues, de chaque pixel de l'image
    //Je dois pas être très doué mais j'ai pas réussi à créer un seul tableau 3D où chaque case contient trois sous-cases avec les composantes
    //Attention : dans l'état actuel, le programme ne peut pas traiter le cas d'images dont une des dimensions est plus grande que 10000 pixels
    int **pixelsR = NULL;
    int **pixelsG = NULL;
    int **pixelsB = NULL;
    //On réserve pour chaque tableau 10000 cases mémoire de la taille d'un tableau ...
	pixelsR = (int**)malloc(TAILLEMAX*sizeof(int*));
	pixelsG = (int**)malloc(TAILLEMAX*sizeof(int*));
	pixelsB = (int**)malloc(TAILLEMAX*sizeof(int*));
    for (int i = 0; i < TAILLEMAX; i++)
    {
        //... qui occupe lui même 10000 cases
        pixelsR[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
        pixelsG[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
        pixelsB[i] = (int*)malloc(sizeof(int) * TAILLEMAX);
    }


    //On compte le nombre d'images dans le répertoire
    dp = opendir(repertory_dep);    //On ouvre le répertoire contenant les images
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
        {
            if (strlen(ep->d_name) >= 4) //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers)
                nombreimages++;
        }
        (void) closedir (dp);   //Fermeture du répertoire d'images
    }



    dp = opendir(repertory_dep);    //On parcourt le répertoire une deuxième fois, avec traitement de chaque image
    if (dp != NULL && nombreimages != 0)
    {
    while ((ep = readdir(dp)))
    {
        if (strlen(ep->d_name) >= 4)
        {
            cout << "Chargement : " << 100*compteurimages/nombreimages << "% - ";
            cout << ep->d_name << " : traitement ... ";
            lettre = NULL;
            filtres_indiv(ep->d_name, pixelsR, pixelsG, pixelsB, lettre, repertory_dep, repertory_arr);
            compteurimages++;
            cout << " Traitee." << endl;
        }
    }
    (void) closedir (dp);   //Fermeture du répertoire d'images
    cout << "Chargement : " << 100*compteurimages/nombreimages << "% - Programme termine !" << endl;
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

void filtres_indiv(char* nomFichierPng, int **pixelsR, int **pixelsG, int **pixelsB, SDL_Surface *lettre)
{
	filtres_indiv(nomFichierPng, pixelsR, pixelsG, pixelsB, lettre, DOSSIERIMAGES, DOSSIERTEXTES);
}
void filtres_indiv(char* nomFichierPng, int **pixelsR, int **pixelsG, int **pixelsB, SDL_Surface *lettre, const char* repertory_dep, const char* repertory_arr)
{
	int i = 0;
	int x = 0, y = 0;
	int hauteur, largeur;   //Les vraies dimensions de l'image, calculées après calcul des marges à rogner
	int distancemax[3];   //Le max de la "distance chromatique" séparant les pixels du pixel de référence (voir plus bas)

	double moyenne = 0;
	//Les tableaux qui contiendront les composantes finales des 400 pixels
	int pixelsRfinal[TAILLE][TAILLE];
	int pixelsGfinal[TAILLE][TAILLE];
	int pixelsBfinal[TAILLE][TAILLE];

	int reference [3] = {0};    //La "référence" de couleur, qui sera définie comme la couleur du pixel de coordonnées (0,0)
	int marges[4] = {0};

    //Les variables suivantes sont nécessaires pour des fonctions que je n'ai pas codé moi même : récupération de la couleur des pixels et parcours du répertoire d'images
	Uint8 red, green, blue;
    Uint32 infopixel;

    FILE* fichier = NULL;   //Un pointeur de fichier
    char nomFichierTxt[300] = "";   //Le nom du fichier texte associé
    char dossierPng[100];   //Le nom du répertoire d'images
    char dossierTxt[100];   //Le nom du répertoire où seront créés les fichiers textes

    //Traitement des noms de fichiers : le but est de créer un fichier txt de même nom que l'image traitée, et de le place dans le sous dossier "textes"
	strcpy(dossierPng, repertory_dep);
    strcpy(dossierTxt, repertory_arr);
	dossierPng[strlen(repertory_dep)]='\0'; //On coupe la chaîne correspondant au chemin relatif du fichier .png
    dossierTxt[strlen(repertory_arr)]='\0'; //Idem pour le .txt

    //On récupère le nom du fichier dans chaque chaîne
    strcpy(nomFichierTxt, nomFichierPng);
    nomFichierTxt[strlen(nomFichierPng)-4]='\0';    //Celui correspondant au fichier texte est coupé 4 caractères avant la fin (on tronque le .png)
    strcat(nomFichierTxt,".txt");   //Puis on lui ajoute ".txt"


    strcat(dossierPng,nomFichierPng);   //On obtient le chemin relatif menant à l'image en concaténant "images/" et le nom de l'image
    strcat(dossierTxt,nomFichierTxt);   //Idem pour le dossier "textes/" et le fichier ".txt"

    if (lettre == NULL)
        lettre = IMG_Load(dossierPng);  //On charge dans une surface SDL l'image à laquelle on accède par le chemin relatif dossierPng

    if (lettre == NULL) //Si il y a eu un souci d'ouverture de l'image, on écrit une erreur dans erreur.txt
    {
        ofstream file("erreur.txt",ofstream::ate);
        file << "Filtres2 - Bug 1 : L'image " << dossierPng << " n'a pas pu etre ouverte." << endl;
        cout << "Filtres2 - Bug 1 : L'image " << dossierPng << " n'a pas pu etre ouverte." << endl;
    }
    else
    {

    fichier = fopen(dossierTxt,"w+");   //On ouvre (ou on créé) un fichier txt

    if (fichier == NULL){    //Si l'ouverture du fichier texte a échoué, on inscrit une erreur dans erreur.txt
        ofstream file("erreur.txt",ofstream::ate);
        file << "Filtres2 - Bug 2 : Le fichier " << dossierTxt << " n'a pas pu etre ouvert." <<endl;
        cout << "Filtres2 - Bug 2 : Le fichier " << dossierTxt << " n'a pas pu etre ouvert." <<endl;
    }
	else //Si l'ouverture a réussi
	{
    //La couleur du pixel de référence
    infopixel = getPixel(lettre, 0,0);
    SDL_GetRGB(infopixel, lettre->format, &red, &green, &blue);
    reference[0] = (int)red;
    reference[1] = (int)green;
    reference[2] = (int)blue;

    for(i = 0; i < 3; i++)  //On réinitialise la distance chromatique
        distancemax[i] = 0;


    SDL_LockSurface(lettre);    //On verouille la surface SDL (indispensable pour lire les pixels)
    for(y = 0; y < lettre->h ; y++) //On parcourt tous les pixels de l'image
    {
        for(x = 0; x < lettre->w ; x++)
        {
            //On récupère les infos d'un pixel dans infopixel ses composantes dans red, green, blue
            infopixel = getPixel(lettre, x,y);
            SDL_GetRGB(infopixel, lettre->format, &red, &green, &blue);
            //On place le tout dans les tableaux correspondants
            pixelsR[y][x] = (int)red;
            pixelsG[y][x] = (int)green;
            pixelsB[y][x] = (int)blue;

            //Calcul de la distance chromatique à la référence maximale : si le pixel considéré possède une distance chromatique moyenne à la référence supérieure à la moyenne des distancemax[i], on actualise distancemax
            if (distancemax[0] + distancemax[1] + distancemax[2] < abs(red - reference[0]) + abs(green - reference[1]) + abs(blue - reference[2]))
            {
                distancemax[0] = abs(red - reference[0]);
                distancemax[1] = abs(green - reference[1]);
                distancemax[2] = abs(blue - reference[2]);
            }
        }
    }

    SDL_UnlockSurface(lettre);  //On déverouille la surface

    //Les tableaux qui contiendront le nombre de pixels à rogner, respectivement en haut, en bas, à gauche, à droite :
    marges[0] = 0;
    marges[1] = 0;
    marges[2] = 0;
    marges[3] = 0;
    //On calcule ces marges : on rogne le plus possible de lignes/colonnes contenant uniquement des pixels de la couleur de référence
    margeLigne(pixelsR, pixelsG, pixelsB, lettre->w, lettre->h, reference, marges);
    margeColonne(pixelsR, pixelsG, pixelsB, lettre->w, lettre->h, reference, marges);

    //On calcule les nouvelles dimensions de l'image
    hauteur = lettre->h - marges[0] - marges[1];
    largeur = lettre->w - marges[2] - marges[3];

    if (hauteur <= 0 || largeur <= 0)   //Dans le cas où l'une des dimensions est négative, cela signifie que l'image a été totalement rognée : on n'inscrit alors que des -1 dans le fichier textes
        {
        for(y = 0; y < TAILLE ; y++)
        {
            for(x = 0; x < TAILLE ; x++)
                {
                    fprintf(fichier, "-1 ");
                }
            fprintf(fichier, "\n ");
        }
        }
    else    //Sinon, on rend l'image carrée en diminuant alternativement les marges gauche/droite ou haut/bas selon si la hauteur est plus grande que la largeur ou non
    {
        while(hauteur < largeur)
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
            hauteur = lettre->h - marges[0] - marges[1];
            }
            //Dans le cas où l'une des marges est négative, cela signifie que rendre l'image carrée et centrée sur la lettre est impossible sans dépasser de l'image totale
            //On diminue alors l'autre marge d'autant que nécessaire pour rendre nulle celle qui était négative (super clair comme phrase)
            if (marges[0] < 0)
                {
                    marges[1]+=marges[0];
                    marges[0] = 0;
                }
            if (marges[1] < 0)
                {
                    marges[0]+=marges[1];
                    marges[1] = 0;
                }

        hauteur = lettre->h - marges[0] - marges[1];

        while(largeur < hauteur)
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
            largeur = lettre->w - marges[2] - marges[3];
            }
        if (marges[2] < 0)
            {
                marges[3]+=marges[2];
                marges[2] = 0;
            }
        if (marges[3] < 0)
            {
                marges[2]+=marges[3];
                marges[3] = 0;
            }

        largeur = lettre->w - marges[2] - marges[3];
        //A ce stade, largeur = hauteur, et si l'image initiale le permettait, les marges sont telles qu'une fois rognées, on obtient une image carrée centrée sur la lettre
        //La fonction pixelsfinal calcule alors une image 20x20 (stockée dans pixelsRfinal, etc.) avec la fameuse moyenne pondérée (la fonction est cauchemardesque à coder)
        pixelsfinal(pixelsR, pixelsRfinal, lettre->w, lettre->h, marges);
        pixelsfinal(pixelsG, pixelsGfinal, lettre->w, lettre->h, marges);
        pixelsfinal(pixelsB, pixelsBfinal, lettre->w, lettre->h, marges);

        //On parcourt le tableau final de pixels pour écrire le résultat dans le fichier
        for(y = 0; y < TAILLE ; y++)
        {
            for(x = 0; x < TAILLE ; x++)
                {
                    //On calcule le rapport de la distance chromatique à la référence du pixel et de distancemax, la distance chromatique maximale
                    moyenne = abs(pixelsRfinal[y][x] - reference[0]) + abs(pixelsGfinal[y][x] - reference[1]) + abs(pixelsBfinal[y][x] - reference[2]);
                    moyenne /= (distancemax[0] + distancemax[1] + distancemax[2]);
                    //moyenne = 2*moyenne -1; //On transforme le résultat ([0, 1] -> [-1, 1])
                    fprintf(fichier, "%.2f " , moyenne);    //On écrit le résultat avec 2 décimales
                }
            fprintf(fichier, "\n ");    //Au bout d'une ligne de pixels, on revient à la ligne avant d'écrire la ligne suivante (choix arbitraire, ajustable)
        }


    }
    fclose(fichier);
    }
    }


}


Uint32 getPixel(SDL_Surface * surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

  switch (bpp) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *) p;
  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *) p;

  default:
    return 0;
  }
}

void margeLigne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[])
{
    int interrA = 0;
    int interrB = 0;
    int x = 0, y = 0;

    for (y = 0; y < hauteur; y++)
    {
        interrB = 0;

        for(x = 0; x < largeur; x++)
        {
            if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
                interrB = 1;
        }

        if (interrB == 1)
            interrA = 1;
        else
        {
            if (interrA == 0)
                marges[0]++;
        }
    }

    interrA = 0;

    for (y = hauteur-1; y >= 0; y--)
    {
        interrB = 0;

        for(x = 0; x < largeur; x++)
        {
            if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
                interrB = 1;
        }

        if (interrB == 1)
            interrA = 1;
        else
        {
            if (interrA == 0)
                marges[1]++;
        }
    }



}

void margeColonne(int **pixelsR, int **pixelsG, int **pixelsB, int largeur, int hauteur, int reference[], int marges[])
{
    int interrA = 0;
    int interrB = 0;
    int x = 0, y = 0;

    for (x = 0; x < largeur; x++)
    {
        interrB = 0;

        for (y = 0; y < hauteur; y++)
        {
            if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
                interrB = 1;
        }

        if (interrB == 1)
            interrA = 1;
        else
        {
            if (interrA == 0)
                marges[2]++;
        }
    }

    interrA = 0;

    for (x = largeur - 1; x >= 0; x--)
    {
        interrB = 0;

        for (y = 0; y < hauteur; y++)
        {
            if (pixelsR[y][x] != reference[0] || pixelsG[y][x] != reference[1] || pixelsB[y][x] != reference[2])
                interrB = 1;
        }

        if (interrB == 1)
            interrA = 1;
        else
        {
            if (interrA == 0)
                marges[3]++;
        }
    }
}

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

}
