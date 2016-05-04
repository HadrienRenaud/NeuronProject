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
#include "menu.h"
#include "system.h"
#include "database.h"
#include "buttons.h"

using namespace std;

int main(int argc, char** argv)
{
    SDL_Window *window(0);
    SDL_Renderer *ren = init(window);

    menu(ren);
    SDL_DestroyRenderer(ren);
    quit(window);

    return 0;
}
