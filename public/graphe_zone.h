#ifndef __GRAPHE_ZONE__
#define __GRAPHE_ZONE__

#include "Liste_case.h"

typedef struct sommet Sommet ;

/* Element d’une liste cha\^in\’ee de pointeurs sur Sommets */
typedef struct cellule_som {
    Sommet * sommet ;
    struct cellule_som * suiv ;
} Cellule_som ;


struct sommet {
    int num ; /* Numero du sommet (sert uniquement a l’affichage) */
    int cl ;    /* Couleur d’origine du sommet-zone */
    ListeCase cases ;    /* Listes des cases du sommet-zone */
    int nbcase_som ; /* Nombre de cases de cette liste */
    int marque;

    Cellule_som * sommet_adj ; /* Liste des areteses pointeurs sur les sommets
                                adjacents */

    int distance ; /* Nombre d’aretes reliant ce sommet a la racine
                    du parcours en largeur */
    Sommet *pere ; /* Pere du sommet dans l’arborescence du parcours en largeur */                         
};


typedef struct graphe_zone {
    int nbsom ;     /* Nombre de sommets dans le graphe */
    Cellule_som * som ; /* Liste chainee des sommets du graphe */
    Sommet *** mat ;     /* Matrice de pointeurs sur les sommets indiquant
                        a quel sommet appartient une case (i,j) de la grille */
} Graphe_zone;


/* ajoute un pointeur sur Sommet à une liste chainée */
void ajoute_liste_sommet(Cellule_som **liste,Sommet *s);

/*libere le Graphe zone */
void libere_Graphe_zone(Graphe_zone *G,int dim);

/* détruit une liste chainée de Cellule_som sans détruire les sommets pointées par cette liste */
void detruit_liste_somme(Cellule_som *liste);

/* met à jour deux sommets s1 et s2 en indiquant qu'ils sont adjacents */
void ajoute_voisin(Sommet *s1, Sommet *s2);

/* renvoie vrais ssi les deux sommets sont adjacents */
int adjacent(Sommet *s1, Sommet *s2);

void affiche_graphe(Graphe_zone *G,int dim);


/* crée le graphe tout entier */
Graphe_zone cree_graphe_zone();

#endif

