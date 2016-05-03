#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

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

SDL_Renderer* init(SDL_Window *window);
void quit(SDL_Window *window);
SDL_Texture* loadTexture(const std::string &fileImg, SDL_Renderer *ren);
void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);



#endif // SYSTEM_H_INCLUDED
