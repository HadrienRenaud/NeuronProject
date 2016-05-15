#include "system.h"

using namespace std;




SDL_Renderer* init(SDL_Window *window)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("NeuronProjet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		ofstream file("erreur.txt");
		file << "Impossible de creer la fenetre :" << SDL_GetError() << endl;
		exit(1);
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	TTF_Init();
	return renderer;
}


void quit(SDL_Window *window)
{
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}


SDL_Texture* loadTexture(const std::string &fileImg, SDL_Renderer *ren)
{
	SDL_Surface *image = IMG_Load(fileImg.c_str());

	if (image == NULL)
	{
		ofstream file("erreur.txt");
		file << "Impossible d'ouvrir l'image " << fileImg << endl;
		exit(1);
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_FreeSurface(image);
	return texture;
}


void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y)
{
	int			w, h;

	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect	dst;
	dst.x	= x;
	dst.y	= y;
	dst.w	= w;
	dst.h	= h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

