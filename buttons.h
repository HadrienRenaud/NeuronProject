#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include "config.h"

using namespace std;

/*! Renvoie une surface correspondant à un bouton vide. Par défaut le skin 1 est utilisé pour le bouton standart, le skin 2 quand le bouton est survolé par le curseur, le skin 3 pendant un clic de souris.
    \param width,height la largeur et hauteur du bouton.
    \param skin un entier valant 1, 2, 3 ou 4 et définissant l'apparence du bouton : voir les images des "coins" dans le dossier resources/.
*/
SDL_Surface* emptyButton(int skin, int width, int height);


//! L'objet bouton, sur lequel on peut cliquer sur l'interface graphique.
class Button
{
public:

/*! Construit un bouton. Les dimensions s'adaptent à celles du texte qui s'y trouve.
    \param ren le lecteur multimédia sur lequel on travaille.
    \param text le libellé du bouton.
    \param size la taille de la police (enregistrée dans resources/font_buttons.ttf) utilisée pour le texte du bouton
    \param alpha le paramètre de transparence du bouton, nous l'utilisons actuellement à 235 sur 255
    \param xB,yB les coordonnées auxquelles se trouvera le bouton
*/
Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB);

/*! Autre constructeur.
    \param width la largeur du bouton.
    \param height la hauteur du bouton.
*/
Button(SDL_Renderer *ren, const std::string &text, int size, int alpha, int xB, int yB, int width, int height);


/*! Autre constructeur.
    \param file1 l'image qui sera affichée lorsque le bouton n'est pas cliqué.
    \param file2 l'image qui sera affichée pendant un clic.
*/
Button(SDL_Renderer *ren, const std::string &file1, const std::string &file2, bool alreadyPressed, int xB, int yB);


//! Détermine si le bouton est actuellement survolé par la souris.
bool enlight(int xMouse, int yMouse);

//! Appelé lors d'un clic de souris ; si la souris est alors au dessus du bouton, indique que la souris est enfoncée sur le bouton.
void pressDown(int xMouse, int yMouse);

//! Appelé si le clic de souris est relâché ; si la souris est alors au dessus du bouton et que le bouton a été enfoncé auparavant, indique que le bouton a été pressé.
void pressUp(int xMouse, int yMouse);

//! Appelé dans la boucle principale d'affichage graphique ; répond si le bouton vient d'être pressé ou non, permet de déclencher l'action associée au bouton.
bool hasBeenPressed();

//! Modifie manuellement l'appui sur le bouton.
void setPress(bool someBool);

//! Réinitialise les informations du bouton, notamment le fait que le bouton ait été cliqué ou non, ou s'il est en cours de clic.
void reset();

//! Procède à l'affichage du bouton.
void renderButton(SDL_Renderer *ren, int xMouse, int yMouse);


protected:

//! Abscisse du bouton.
int x;

//! Ordonnée du bouton.
int y;

//! Texture correspondant au bouton par défaut.
SDL_Texture* image;

//! Texture correspondant au bouton pendant un survol de la souris.
SDL_Texture* imageLight;

//! Texture correspondant au bouton pendant que la souris est enfoncée au dessus du bouton.
SDL_Texture* imagePressed;

//! Le bouton vient-il d'être pressé, c'est-à-dire que la souris a été successivement enfoncée puis relâchée au dessus du bouton ?
bool press;

//! Le bouton est-il en train d'être cliqué, c'est-à-dire la souris vient-elle d'être enfoncée au dessus du bouton ?
bool pressing;

};



#endif // BUTTONS_H_INCLUDED
