#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "config.h"

//! Charge l'interface graphique avec les boutons pour accéder aux fonctionnalités du programme.
void  menu(SDL_Renderer *ren);

/*!	Gère les raccourcis clavier : analyse les évènements liés au clavier et indique quelle touches ont été enfoncées dans un tableau de booléens.
    \param event l'évènement récupéré par la SDL, analysable pour déterminer s'il s'agit d'une touche pressée, relâchée, d'un clic/mouvement de souris, etc.
    \param keyboardInput tableau de 26 + 3 (échap, gauche, droite) booléens pour chaque touche
 */
void  keyboard(SDL_Event event, bool* keyboardInput);

#endif  // MENU_H_INCLUDED
