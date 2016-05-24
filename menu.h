#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "config.h"

void	menu(SDL_Renderer *ren);
void	keyboard(SDL_Event event, bool* keyboardInput);
int		getLenghtAlphabet();

#endif	// MENU_H_INCLUDED
