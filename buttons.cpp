#include "buttons.h"

using namespace std;

Button::Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB)
{
	x	= xB;
	y	= yB;
	int				alpha2	= alpha + (255 - alpha) * 0.4;
	int				alpha3	= alpha + (255 - alpha) * 0.7;

	SDL_Color		color	= { 255, 255, 255 };
	SDL_Rect		dest;

	SDL_Surface *	texte			= TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", size), text.c_str(), color);
	SDL_Surface *	imageS			= emptyButton(1, texte->w + 10, texte->h + 10);
	SDL_Surface *	imageLightS		= emptyButton(2, texte->w + 10, texte->h + 10);
	SDL_Surface *	imagePressedS	= emptyButton(3, texte->w + 10, texte->h + 10);
	dest.x	= 5;
	dest.y	= 5;
	dest.w	= texte->w;
	dest.h	= texte->h;
	SDL_BlitSurface(texte, NULL, imageS, &dest);
	SDL_BlitSurface(texte, NULL, imageLightS, &dest);
	SDL_BlitSurface(texte, NULL, imagePressedS, &dest);
	image			= SDL_CreateTextureFromSurface(ren, imageS);
	imageLight		= SDL_CreateTextureFromSurface(ren, imageLightS);
	imagePressed	= SDL_CreateTextureFromSurface(ren, imagePressedS);

	SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(imageLight, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(imagePressed, SDL_BLENDMODE_BLEND);

	SDL_SetTextureAlphaMod(image, alpha);
	SDL_SetTextureAlphaMod(imageLight, alpha2);
	SDL_SetTextureAlphaMod(imagePressed, alpha3);

	press		= false;
	pressing	= false;
}

Button::Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB, int width, int height)
{
	x	= xB;
	y	= yB;
	int				alpha2	= alpha + (255 - alpha) * 0.4;
	int				alpha3	= alpha + (255 - alpha) * 0.7;

	SDL_Color		color	= { 255, 255, 255 };
	SDL_Rect		dest;

	SDL_Surface *	texte			= TTF_RenderText_Blended(TTF_OpenFont("resources/font_buttons.ttf", size), text.c_str(), color);
	SDL_Surface *	imageS			= emptyButton(1, width, height);
	SDL_Surface *	imageLightS		= emptyButton(2, width, height);
	SDL_Surface *	imagePressedS	= emptyButton(3, width, height);

	dest.x	= width / 2 - texte->w / 2;
	dest.y	= height / 2 - texte->h / 2;
	dest.w	= width;
	dest.h	= height;
	SDL_BlitSurface(texte, NULL, imageS, &dest);
	SDL_BlitSurface(texte, NULL, imageLightS, &dest);
	SDL_BlitSurface(texte, NULL, imagePressedS, &dest);
	image			= SDL_CreateTextureFromSurface(ren, imageS);
	imageLight		= SDL_CreateTextureFromSurface(ren, imageLightS);
	imagePressed	= SDL_CreateTextureFromSurface(ren, imagePressedS);

	SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(imageLight, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(imagePressed, SDL_BLENDMODE_BLEND);

	SDL_SetTextureAlphaMod(image, alpha);
	SDL_SetTextureAlphaMod(imageLight, alpha2);
	SDL_SetTextureAlphaMod(imagePressed, alpha3);

	press		= false;
	pressing	= false;
}


Button::Button(SDL_Renderer *ren, const std::string &file1, const std::string &file2, bool alreadyPressed, int xB, int yB)
{
	x				= xB;
	y				= yB;

	image			= loadTexture(file1.c_str(), ren);
	imageLight		= NULL;
	imagePressed	= loadTexture(file2.c_str(), ren);

	SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(imagePressed, SDL_BLENDMODE_BLEND);

	int w, h, w2, h2;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	SDL_QueryTexture(imagePressed, NULL, NULL, &w2, &h2);

	if ((w != w2) | (h != h2))
	{
		FILE* fichier = fopen("erreur.txt", "a");
		fprintf(fichier, "Les images d'un bouton sont de tailles differentes.\n");
		printf("Les images d'un bouton sont de tailles differentes.\n");
		fclose(fichier);
		exit(1);
	}

	press		= alreadyPressed;
	pressing	= false;
}

void Button::reset()
{
	press		= false;
	pressing	= false;
}

bool Button::enlight(int xMouse, int yMouse)
{
	int w, h;

	SDL_QueryTexture(image, NULL, NULL, &w, &h);

	if (xMouse >= x && xMouse < x + w && yMouse >= y && yMouse < y + h)
		return true;
	else
		return false;


}

void Button::renderButton(SDL_Renderer *ren, int xMouse, int yMouse)
{
	if (imageLight != NULL)
	{
		if ((pressing && enlight(xMouse, yMouse)) | press)
			renderTexture(ren, imagePressed, x, y);
		else
		{
			if (enlight(xMouse, yMouse))
				renderTexture(ren, imageLight, x, y);
			else
				renderTexture(ren, image, x, y);
		}
	}
	else
	{
		if (press)
			renderTexture(ren, imagePressed, x, y);
		else
			renderTexture(ren, image, x, y);
	}

}

void Button::pressDown(int xMouse, int yMouse)
{
	if (enlight(xMouse, yMouse))
		pressing = true;
	else
		pressing = false;
}

void Button::pressUp(int xMouse, int yMouse)
{

	if (enlight(xMouse, yMouse) && pressing)
	{
		pressing	= false;
		press		= !press;
	}
	else
		pressing = false;
}

bool Button::hasBeenPressed()
{
	return press;
}
void Button::setPress(bool someBool)
{
	press = someBool;
}




SDL_Surface* emptyButton(int skin, int width, int height)
{
	SDL_Surface *	empty	= SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 255);
	SDL_Surface *	corners = NULL;
	SDL_Rect		dest, src;
	Uint32			framework, top, bottom;

	int				colors[27];
	ifstream		file("resources/colors.txt");

	for (int i = 0; i < 27; i++)
		file >> colors[i];


	if (skin == 1)
	{
		corners		= IMG_Load("resources/corners1.png");
		framework	= SDL_MapRGBA(empty->format, colors[0], colors[1], colors[2], 255);
		top			= SDL_MapRGBA(empty->format, colors[9], colors[10], colors[11], 255);
		bottom		= SDL_MapRGBA(empty->format, colors[18], colors[19], colors[20], 255);
	}
	else if (skin == 2)
	{
		corners		= IMG_Load("resources/corners2.png");
		framework	= SDL_MapRGBA(empty->format, colors[3], colors[4], colors[5], 255);
		top			= SDL_MapRGBA(empty->format, colors[12], colors[13], colors[14], 255);
		bottom		= SDL_MapRGBA(empty->format, colors[21], colors[22], colors[23], 255);
	}
	else if (skin == 3)
	{
		corners		= IMG_Load("resources/corners3.png");
		framework	= SDL_MapRGBA(empty->format, colors[6], colors[7], colors[8], 255);
		top			= SDL_MapRGBA(empty->format, colors[15], colors[16], colors[17], 255);
		bottom		= SDL_MapRGBA(empty->format, colors[24], colors[25], colors[26], 255);
	}
	else
	{
		corners		= IMG_Load("resources/corners4.png");
		framework	= SDL_MapRGBA(empty->format, colors[0], colors[1], colors[2], 255);
		top			= SDL_MapRGBA(empty->format, colors[9], colors[10], colors[11], 255);
		bottom		= top;
	}



	dest.x	= 0;
	dest.y	= 0;
	dest.w	= width;
	dest.h	= floor(height / 2);
	SDL_FillRect(empty, &dest, top);

	dest.x	= 0;
	dest.y	= dest.h;
	dest.w	= width;
	dest.h	= height - dest.h;
	SDL_FillRect(empty, &dest, bottom);

	dest.x	= 0;
	dest.y	= 0;
	dest.w	= width;
	dest.h	= 1;
	SDL_FillRect(empty, &dest, framework);
	dest.y	= height - 1;
	SDL_FillRect(empty, &dest, framework);
	dest.y	= 0;
	dest.w	= 1;
	dest.h	= height;
	SDL_FillRect(empty, &dest, framework);
	dest.x	= width - 1;
	SDL_FillRect(empty, &dest, framework);

	dest.w	= 3;
	dest.h	= 3;
	src.w	= 3;
	src.h	= 3;

	dest.x	= 0;
	dest.y	= 0;
	src.x	= 0;
	src.y	= 0;
	SDL_BlitSurface(corners, &src, empty, &dest);
	src.x	= 3;
	src.y	= 0;
	dest.x	= width - 3;
	dest.y	= 0;
	SDL_BlitSurface(corners, &src, empty, &dest);
	src.x	= 0;
	src.y	= 3;
	dest.x	= 0;
	dest.y	= height - 3;
	SDL_BlitSurface(corners, &src, empty, &dest);
	src.x	= 3;
	src.y	= 3;
	dest.x	= width - 3;
	dest.y	= height - 3;
	SDL_BlitSurface(corners, &src, empty, &dest);

	SDL_SetColorKey(empty, SDL_TRUE, SDL_MapRGB(empty->format, 255, 0, 0));

	return empty;

}
