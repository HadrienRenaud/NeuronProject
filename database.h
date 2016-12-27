#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "config.h"

using namespace std;


/*! Pour chacune des polices dans le DOSSIERPOLICES, appelle la fonction filterPngs.
    Note : on utilise de gros tableaux dans filterPngs, ils sont initialisés dans cette fonction pour éviter d'avoir à les reconstruire et redétruire à chaque exécution de filterPngs.
    \param sauvegarde veut-on sauvegarder individuellement les images générées au format .png ?
    \param filtre veut-on filtrer immédiatement les images générées ? (évite d'avoir à exécuter les filtres ensuite).
*/
void  database(bool sauvegarde, bool filtre);


/*! Synthétise les images correspondant à l'alphabet défini par les constantes LENGTH_ALPHABET et CHARS.
    \param nameFont le nom du fichier de police à charger pour la lettre.
    \param pixels trois tableaux 10000x10000 qui vont contenir les composantes rouge, verte, bleue de la lettre pour les filtres.
    \param caracteres l'alphabet utilisé
*/
void  filterPngs(char* nameFont, bool sauvegarde, bool filtre, int **pixelsR, int **pixelsG, int **pixelsB, char caracteres[LENGTH_ALPHABET][2]);


//! Écris les noms des chemins où aller chercher la police, l'image à créer, le txt à créer.
void  pathNames(char* pathPng, char* pathTxt, char* nameFont, const char* repertoryPng, const char* repertoryTxt, int i, char caracteres[LENGTH_ALPHABET][2]);

#endif  // DATABASE_H_INCLUDED
