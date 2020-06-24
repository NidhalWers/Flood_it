#ifndef __STRUCTURE_ACYCLIQUE__
#define __STRUCTURE_ACYCLIQUE__

#include "Liste_case.h"

typedef struct{
    int dim ; /* dimension de la grille */
    int nbcl ; /* nombre de couleurs */
    ListeCase Lzsg ; /* Liste des cases de la zone Zsg */
    ListeCase * B ; /* Tableau de listes de cases de la bordure*/
    int ** App ; /* Tableau a double entree des appartenances */
}S_Zsg ;

/* Initialise la structure */
void init_Zsg(S_Zsg *S, int dim, int nbcl);

/*Libere la structure */
void libereZSG(S_Zsg *S);

/*Ajoute une case dans la liste Lzsg */
int ajoute_Zsg(S_Zsg *S, int i, int j);

/* Ajoute une case dans la bordure d'une couleur cl donnée */
int ajoute_Bordure(S_Zsg *S, int i, int j, int cl);

/* Renvoie vrai si une case est dans Lzsg */
int appartient_Zsg(S_Zsg *S, int i, int j);

/* Renvoie vrai si une case est dans la bordure de couleur cl donnée */
int appartient_Bordure(S_Zsg *S, int i, int j, int cl);

/* met à jour la Zsg et la bordure, la fonciton retourne le nombre de case
ajoutée à la Zsg  */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

#endif