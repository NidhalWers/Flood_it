#include <stdlib.h>
#include <stdio.h>
#include "Strategie_bonus.h"


int strategie_deux_coins( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S){
    
    int res=0;
    
    //initialisation SZsg avec bordures
    ajoute_Zsg_sommet(S,G->mat[0][0]);
    int taille=1;
    
    
    Cellule_som *adj_tmp=G->mat[0][0]->sommet_adj;
    while ( G->mat[0][0]->sommet_adj){
        ajout_BordureGraphe(S,G->mat[0][0]->sommet_adj->sommet,G->mat[0][0]->sommet_adj->sommet->cl);
        G->mat[0][0]->sommet_adj=G->mat[0][0]->sommet_adj->suiv;
    }
    G->mat[0][0]->sommet_adj=adj_tmp;
    
    
    res+= application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, dim-1, dim-1);
    reset_sommet(&(S->Szsg));
    
    res+= application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, dim-1, 0);
    reset_sommet(&(S->Szsg));
   
     return res + strategie_max_B(M , Gr , dim , nbcl , aff, G, S, taille);

}






int strategie_trois_coins( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S){
    int res=0;

    //initialisation SZsg avec bordures
    ajoute_Zsg_sommet(S,G->mat[0][0]);
    int taille=1;

    Cellule_som *adj_tmp=G->mat[0][0]->sommet_adj;
    while ( G->mat[0][0]->sommet_adj){
        ajout_BordureGraphe(S,G->mat[0][0]->sommet_adj->sommet,G->mat[0][0]->sommet_adj->sommet->cl);
        G->mat[0][0]->sommet_adj=G->mat[0][0]->sommet_adj->suiv;
    }
    G->mat[0][0]->sommet_adj=adj_tmp;

    res+= application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, dim-1, dim-1);
    reset_sommet(&(S->Szsg));

    res+= application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, dim-1, 0);
    reset_sommet(&(S->Szsg));

    res+= application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, 0, dim-1);
    //reset_sommet(&(S->Szsg));

     return res + strategie_max_B(M , Gr , dim , nbcl , aff, G, S, taille);
}

void init_tab(int *m,int taille){
    int i;
    for(i=0;i<taille;i++){
        m[i]=0;
    }
}