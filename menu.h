#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

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
#include "filtres2.h"
#include "system.h"
#include "database.h"
#include "buttons.h"
#include "NetworkArray.h"

#define DOSSIERTEST "test/"
#define DOSSIERTESTTEXT "test_texts/"
#define TESTING_BACKGROUND_DIMENSIONS 300
#define REFRESH_TIME 0.2

void menu(SDL_Renderer *ren);
void keyboard(SDL_Event event, bool* keyboardInput);



#endif // MENU_H_INCLUDED
