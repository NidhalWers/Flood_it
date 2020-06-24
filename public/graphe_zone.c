#include "graphe_zone.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Entete_Fonctions.h"

void ajoute_liste_sommet(Cellule_som **liste,Sommet *s){
    
    Cellule_som* cell;
    
    cell= (Cellule_som*) malloc(sizeof(Cellule_som));
    cell->sommet=s;
    cell->suiv=*liste;
    
    
    
    *liste = cell;
}

/* détruit une liste chainée de Cellule_som sans détruire les sommets pointées par cette liste */
void detruit_liste_somme(Cellule_som *liste){
    Cellule_som *temp;
    while (liste){
        temp=liste;
        liste=liste->suiv;
        free(temp);
    }
    

}

/* met à jour deux sommets s1 et s2 en indiquant qu'ils sont adjacents */
void ajoute_voisin(Sommet *s1, Sommet *s2){
    
    
    ajoute_liste_sommet(&(s1->sommet_adj),s2);
    ajoute_liste_sommet(&(s2->sommet_adj),s1);

       
}

/* renvoie vrais ssi les deux sommets sont adjacents */
int adjacent(Sommet *s1, Sommet *s2){
    
    Cellule_som *var=s1->sommet_adj;
    while(var){

        if(var->sommet->num==s2->num)
            return 1;
        var=var->suiv;
    }

    return 0;
}


/* crée le graphe tout entier */
Graphe_zone  cree_graphe_zone(int dim,int **M){
    
    Graphe_zone *G;
    G=(Graphe_zone*)malloc(sizeof(Graphe_zone));
    G->som=NULL;
    G->nbsom=0;
    
    int i,j;
    //initialisation du graphe
    G->mat=malloc(sizeof(Sommet**)*dim);
    
    for (i=0;i<dim;i++){ 
        
        G->mat[i]=malloc(sizeof(Sommet*)*dim);
        for (j=0;j<dim;j++){
            
           G->mat[i][j]=NULL;
        }
    }
    
    //association des cases de la matrice au sommet du graphe
    int num_som=0;
    ListeCase L;
    init_liste(&L);
    int t=0;
    int cpt=0;
    
    for (i=0;i<dim;i++){
        for (j=0;j<dim;j++){
            
            if (G->mat[i][j]==NULL){
                int cl=M[i][j];
                num_som++;
                
                //on trouve la zone de case de même couleur qui va constituer notre sommet 
                trouve_zone_rec(M,dim,i,j,&t,&L);
               
                Sommet *s;
                s=(Sommet*)malloc(sizeof(Sommet));
                s->num=num_som;
                s->cl=cl;
                s->nbcase_som=t;
                s->sommet_adj=NULL;
                s->cases=L;
                s->marque=2;
                s->pere=NULL;
                s->distance=10000;
                int i,j;
                ajoute_liste_sommet(&(G->som),s);
                
                //on fait pointer chaque case de la zone vers notre sommet 
                while(L){
                    
                    i=L->i;
                    j=L->j;
                    //printf("i:%d,j:%d\n",i,j);
                    
                    G->mat[i][j]=s;
                    //printf("%d\n",G->mat[i][j]->num);
                    (cpt)++;
                    
                    L=L->suiv;
                }
                
                //creer_sommet(num_som,L,t,G,cl,&cpt);
                
            }
        }
        G->nbsom=num_som;
    }

    
    //creation des arcs par les sommets voisins
    for (i=0;i<dim;i++){
        for (j=0;j<dim;j++){
            if( i<dim-1 && G->mat[i][j]!=G->mat[i+1][j]){
                
                if(!adjacent(G->mat[i][j],G->mat[i+1][j])){
                    
                    ajoute_voisin(G->mat[i][j],G->mat[i+1][j]);
                    //printf("%d<->%d\n",G->mat[i][j]->num,G->mat[i+1][j]->num);
                }
            }
               
            if ( j<dim-1 && G->mat[i][j]!=G->mat[i][j+1]){
                
                if(!adjacent(G->mat[i][j],G->mat[i][j+1])){
        
                    
                    
                    ajoute_voisin(G->mat[i][j],G->mat[i][j+1]);

                    //printf("%d<->%d\n",G->mat[i][j]->num,G->mat[i][j+1]->num);
                }
            }
        
        }
    }

    int bool1 = G->mat[0][0]->sommet_adj==NULL;
    //printf("bool1 :%d\n",bool1);
    
    return *G;
}



void affiche_graphe(Graphe_zone *G,int dim){
    int j,i;
    for (i=0;i<dim;i++){
        printf("|\n");
        for (j=0;j<dim;j++){
            printf("|%d",(G->mat[i][j]->num)+1000);
        }   
    }
    printf("|\n\n");
}



void libere_Graphe_zone(Graphe_zone *G,int dim){
   
   int i;
   
    Cellule_som*  tmp2=G->som;
    while(G->som){
        
        Cellule_som* tmp=G->som;

        detruit_liste_somme(tmp->sommet->sommet_adj);

        detruit_liste(&(tmp->sommet->cases));
       
        G->som=G->som->suiv;
        free(tmp->sommet);
    }
    detruit_liste_somme(tmp2);
    for(i=0;i<dim;i++){
        free(G->mat[i]); 
    }
    free(G->mat);
    
    
}