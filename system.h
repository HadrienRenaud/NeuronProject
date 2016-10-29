#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "config.h"

SDL_Renderer* init(SDL_Window *window);
void      quit(SDL_Window *window);
SDL_Texture*  loadTexture(const std::string &fileImg, SDL_Renderer *ren);
void      renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);



#endif  // SYSTEM_H_INCLUDED
