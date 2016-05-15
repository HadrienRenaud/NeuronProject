#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

SDL_Surface* emptyButton(int skin, int width, int height);

class Button
{
public:
Button(int xB, int yB);
Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB);
Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB, int width, int height);
Button(SDL_Renderer *ren, const std::string &file1, const std::string &file2, bool alreadyPressed, int xB, int yB);
bool enlight(int xMouse, int yMouse);
void pressDown(int xMouse, int yMouse);
void pressUp(int xMouse, int yMouse);
bool hasBeenPressed();
void setPress(bool someBool);
void reset();
void renderButton(SDL_Renderer *ren, int xMouse, int yMouse);

protected:
int				x;
int				y;
SDL_Texture*	image;
SDL_Texture*	imageLight;
SDL_Texture*	imagePressed;
bool			press;
bool			pressing;

};



#endif	// BUTTONS_H_INCLUDED
