
#include "config.h"



using namespace std;

int main(int argc, char** argv)
{
	SDL_Window *	window(0);
	SDL_Renderer *	ren = init(window);

	menu(ren);
	SDL_DestroyRenderer(ren);
	quit(window);

	return 0;
}
