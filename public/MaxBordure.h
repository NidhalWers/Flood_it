#ifndef __MAX_BORDURE__
#define __MAX_BORDURE__
#include "graphe_zone.h"
#include "Liste_case.h"
#include "API_Grille.h"



typedef struct{
    int dim ; /* dimension de la grille */
    int nbcl ; /* nombre de couleurs */
    Cellule_som *Szsg ; /* Liste des Sommet de la zone Zsg */
    Cellule_som ** B ; /* Tableau de listes des sommet de la bordure par couleur */
    int * BnbS;/* Tableau du nb de sommet par couleur bordure*/
    /* Tableau a double entree des appartenances */
}S_ZsgGr ;

/* Initialise la structure */
void init_ZsgGr(S_ZsgGr *S, int dim, int nbcl);

/* libere la structure */
void libere_ZsgGr(S_ZsgGr *S);

/*Ajoute une case dans la liste Lzsg */
int ajoute_Zsg_sommet(S_ZsgGr *S,Sommet *s);

/* Ajoute une case dans la bordure d'une couleur cl donnée */
void ajout_BordureGraphe(S_ZsgGr *S, Sommet *s, int cl);

/* Renvoie vrai si une case est dans Lzsg */
int appartient_Zsg_sommet(Sommet *s);

/* Renvoie vrai si une case est dans la bordure de couleur cl donnée */
int appartient_bordure_sommet(Sommet *s);

/* met à jour la Zsg et la bordure, la fonciton retourne le nombre de case
ajoutée à la Zsg  */
int majBordureGraphe(S_ZsgGr *S, int cl,int *t );

int strategie_max_B( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G,  S_ZsgGr *S, int t);

void affi_bordure(S_ZsgGr *S);

#endif