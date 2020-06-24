#include <stdlib.h>
#include <stdio.h>
#include "Parcours_largeur.h"


void affiche_li(Cellule_som *liste){
    Cellule_som *tete=liste;
    int count=0;
    printf("-------------------\naffiche liste :\n");
    while(tete && count <10){
        printf("  sommet %d\n",tete->sommet->num);
        //printf("test %d\n", tete->suiv==NULL);
        tete=tete->suiv;
        printf("test null : %d\n", tete==NULL);
        count++;
    }
    printf("-----------------\n");
}















Sommet *recupere_liste_sommet(Cellule_som **liste){
    int count=0;
    Sommet *res;
    Cellule_som *tete=*liste;
    if(tete->suiv){
        Cellule_som *tmp=tete->suiv;
        while(tmp){
            tmp=tmp->suiv;
            if(tmp){
                tete=tete->suiv;    
            }
            count++;
            
        }
        res=tete->suiv->sommet;
        Cellule_som *tmp2=tete->suiv;
        tete->suiv=NULL;
        free(tmp2);
    }
    else{
        res=tete->sommet;
        free(tete);
        *liste =NULL;
    }

    return res;   
    
}






//effectue un parcourt en largeur afin de determiner quel est le chemin le plus court entre 2 points
void parcours_largeur(Sommet *s1){
    Cellule_som *file=NULL;
    ajoute_liste_sommet(&file, s1); 
    
    s1->distance=0;
    Cellule_som *adj;
    Sommet *cell;
    while(file){
        //recupere la fin de la liste soit le premier a y etre entré 
        cell=recupere_liste_sommet(&file);
        adj=cell->sommet_adj;
       //regarde tout les adjacents du sommet recupéré
        while(adj){
            //printf("\t\t\t\tadjacent de %d : sommet %d\n", cell->num, adj->sommet->num);

            //ajoute ce sommet a la file si il n'a pas été visité dans le parcourt 
            if(adj->sommet->pere==NULL && adj->sommet!=s1){
                ajoute_liste_sommet(&file, adj->sommet);
                //ajoute_last(&str_file, adj->sommet);
                //affiche_li(file);
            }
            //associe un pere a ce sommet si il n'en a pas 
            //ou un nouveau pere si la distance avec ce dernier et plus petite
            if((adj->sommet->pere==NULL || cell->distance < adj->sommet->pere->distance )  && adj->sommet!=s1){
                adj->sommet->pere=cell;
                adj->sommet->distance= adj->sommet->pere->distance +1;
            }
            adj=adj->suiv;
        }  
        
       
    }
  
    /*
    Sommet *tmp=s2;
    printf("chemin le plus court du sommet %d au sommet %d :  -->",s2->num,s1->num);
    while(tmp->pere->marque!=0){
       
        tmp=tmp->pere;
        printf(" som %d --> ",tmp->num);
        
    }
    printf("\ndistance = %d\n",s2->distance);
    */
}










void reset_sommet(Cellule_som **liste){
    //printf("reset\n");
    Cellule_som *tete=*liste;
    while(tete){
        tete->sommet->pere=NULL;
        tete->sommet->distance=1000;
        tete=tete->suiv;
    }
}









int application_parcours_L( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S, int *taille, int x1, int y1, int x2, int y2){

    //on détermine les zones demandees
    Sommet *Zone_sup_g= G->mat[x1][y1];
    Sommet *Zone_inf_d= G->mat[x2][y2];
    //printf("som 1 : %d\tsom 2 : %d\n",Zone_sup_g->num,Zone_inf_d->num);

    //on détermine le parcours le plus court entre ces zones 
    parcours_largeur(Zone_sup_g);
    int *tab_cl= malloc(sizeof(int) * Zone_inf_d->distance);
    Sommet *tmp=Zone_inf_d;
    int j;
    for(j=0; j<Zone_inf_d->distance; j++){
        tmp=tmp->pere;
        tab_cl[Zone_inf_d->distance - j - 1]=tmp->cl;
    }


    //on ajoute les sommets de ce chemin à la zone et maj des bordures
    
    int i, couleur;
    //effectue le parcourt en prenant les couleurs determinées avant 
    
    for(i=0; i<Zone_inf_d->distance; i++){
        couleur=tab_cl[i];
        //met à jour la bordure
        Cellule_som *adj=NULL;
        
        Cellule_som *bor=(S->B)[couleur];
        //selectionne la partie du tableau contenant les sommets de la couleur choisie

        while(bor){              
            adj=bor->sommet->sommet_adj;
            //printf("\t cherche adjacent de %d : \n",bor->sommet->num);

            //pour chaque sommet regarde ses adjacents pour les rajouter dans la bordure si besoin
            while(adj){
               if(adj->sommet->marque==2){
                    ajout_BordureGraphe(S,adj->sommet,adj->sommet->cl);
                }
                adj=adj->suiv;
            }
            bor=bor->suiv;
        }

        //on met a jour la zone
        majBordureGraphe(S, couleur, taille);
        
        
        if(aff==1){
            //inondation graphique :
		    //On atribue la nouvelle couleur à chaque case
		    //au niveau de la grille
            Cellule_som *som_zone=S->Szsg;
            int x, y;
            while(som_zone){
                ListeCase som_case=som_zone->sommet->cases;
                while(som_case){
                    x=som_case->i;
                    y=som_case->j;
                    Grille_attribue_couleur_case(Gr,x,y,couleur);
                    som_case=som_case->suiv;
                }
                som_zone=som_zone->suiv;
            }
            //Grille_attente_touche();
			Grille_redessine_Grille();
        }
    }
   
    free(tab_cl);
    //printf("\tParcours du sommet %d au sommet %d : %d\n", Zone_sup_g->num, Zone_inf_d->num, Zone_inf_d->distance);
    return Zone_inf_d->distance;
}












//strategie final ou l'on commence par faire le parcourt jusqu'a l'endroit désiré 
//puis applique la stratégie maxBordure sur la fin
int strategie_parcours_L( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G, S_ZsgGr *S){
    int res=0;
   
    //initialisation SZsg avec bordures
    ajoute_Zsg_sommet(S,G->mat[0][0]);
    int taille=1;
   
    Cellule_som *adj_tmp=G->mat[0][0]->sommet_adj;
    //ajoute les sommets adjacents au premier sommet dans la bordure
    while ( G->mat[0][0]->sommet_adj){
        ajout_BordureGraphe(S,G->mat[0][0]->sommet_adj->sommet,G->mat[0][0]->sommet_adj->sommet->cl);
        G->mat[0][0]->sommet_adj=G->mat[0][0]->sommet_adj->suiv;
    }
    G->mat[0][0]->sommet_adj=adj_tmp;
    
    //effectue le chemin jusqu'a la zone demandé 
    res += application_parcours_L( M, Gr, dim , nbcl, aff, G, S, &taille, 0, 0, dim-1, dim-1);
   
    //reset_sommet(&(S->Szsg));
    //puis applique la stratégie maxBordure
    return res + strategie_max_B(M , Gr , dim , nbcl , aff, G, S, taille);

}


