#ifndef __PARCOURS_LARGEUR__
#define __PARCOURS_LARGEUR__

#include "graphe_zone.h"
#include "Liste_case.h"
#include "MaxBordure.h"
#include "Entete_Fonctions.h"

/* algorithme de parcours en largeur, renvoie le plus court chemin entre deux sommets */
void parcours_largeur(Sommet *s1);


/* retire un pointeur sur Sommet à une liste chainée */
Sommet *recupere_liste_sommet(Cellule_som **liste);

void reset_sommet(Cellule_som **liste);

int application_parcours_L( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S, int *taille, int x1, int y1, int x2, int y2);


int strategie_parcours_L( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S);














#endif