#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "config.h"

//! Prends en argument une fenêtre SDL et renvoie un lecteur multimédia, sur lequel on pourra coller des textures ; à appeler au début du programme graphique.
SDL_Renderer* init(SDL_Window *window);

//! Nettoie et quitte la fenêtre SDL, à appeler à la fin du programme graphique.
void      quit(SDL_Window *window);

/*! Charge une image dans une surface, puis sur une texture.
    \param fileImg le chemin d'un fichier image à charger.
 */
SDL_Texture*  loadTexture(const std::string &fileImg, SDL_Renderer *ren);

/*! Affiche une texture sur le lecteur.
    \param tex la texture à coller.
    \param x,y les coordonnées auxquelles la texture sera collée.
*/
void      renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y);



#endif  // SYSTEM_H_INCLUDED
