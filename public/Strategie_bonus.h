#ifndef __STRATEGIE_BONUS__
#define __STRATEGIE_BONUS__

#include "Parcours_largeur.h"

int strategie_deux_coins( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S);

int strategie_trois_coins( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S);

void init_tab(int *m,int taille);
#endif