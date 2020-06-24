#include "Structure_acyclique.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Initialise la structure */
void init_Zsg(S_Zsg *S, int dim, int nbcl){
     //On initialise la structure   
    init_liste(&(S->Lzsg));
    
    //et ton ses champs
    S->dim=dim;
    S->nbcl=nbcl;

    //printf("dimension : %d | %d \n",dim, S->dim);
    //printf("couleur : %d | %d \n",nbcl, S->nbcl);
    
    S->B=(ListeCase *) malloc(nbcl * sizeof(ListeCase));
    int i;
    for(i=0; i<S->nbcl; i++){
        init_liste(&((S->B)[i]));
    }

    
    S->App=(int**) malloc(S->dim * sizeof(int*));
    int x,y;
    for(x=0; x<S->dim; x++){ 
        S->App[x]=(int*)malloc(S->dim * sizeof(int));
        for(y=0; y<S->dim; y++){
            (S->App)[x][y]=-2;
        }       
    }
   

}

/*Ajoute une case dans la liste Lzsg */
int ajoute_Zsg(S_Zsg *S, int i, int j){
    //printf("ajoute %d %d à la zone\n",i,j);
    ListeCase elnt;
    
    elnt=(Elnt_liste*)malloc(sizeof(Elnt_liste));
    if(! elnt){
        
        return 0;
    }
    elnt->suiv=S->Lzsg;
    elnt->i=i;
    elnt->j=j;
    S->Lzsg=elnt;

    (S->App)[i][j]=-1;
    
}

/* Ajoute une case dans la bordure d'une couleur cl donnée */
int ajoute_Bordure(S_Zsg *S, int i, int j, int cl){
    //printf("ajoute %d %d à la bordure %d\n",i,j,cl);
    Elnt_liste *elnt;
    elnt=(Elnt_liste*)malloc(sizeof(Elnt_liste ));
    if(! elnt){
        return 0;
    }
    elnt->suiv=(S->B)[cl];
    elnt->i=i;
    elnt->j=j;
    (S->B)[cl]=elnt;

    (S->App)[i][j]=cl;
    
}

/* Renvoie vrai si une case est dans Lzsg */
int appartient_Zsg(S_Zsg *S, int i, int j){
    return (S->App)[i][j]==-1;
}

/* Renvoie vrai si une case est dans la bordure de couleur cl donnée */
int appartient_Bordure(S_Zsg *S, int i, int j, int cl){
    return (S->App)[i][j]==cl;
}

/* met à jour la Zsg et la bordure, la fonciton retourne le nombre de case
ajoutée à la Zsg  */
int agrandit_Zsg(int **M, S_Zsg *S, int cl, int k, int l){
    //printf("agrandit couleur %d, case %d %d\n",cl,k,l);
    int cpt=0;
    int nbcase=S->dim;
   
    ListeCase Pile=NULL; //on utilisera le principe de la pile 
    init_liste(&Pile);  //donc on initialise une pile
    ajoute_en_tete(&Pile, k, l); //et on lui ajoute la case k,l
    
    
    if(! appartient_Zsg(S,k,l)){  //Si cette case n'est pas dans la zone
        ajoute_Zsg(S,k,l);        //on l'ajoute à la zone  
        cpt++;                    //et on incrémente le compteur du nombre de case ajoutées
    }
    
    int x,y;
    while(! test_liste_vide(&Pile)){ //tant que la Pile n'est pas vide
        /*printf("---------\nPile :\n");
        ListeCase tmp=Pile;
        while(tmp){
            printf("case %d %d\n",tmp->i, tmp->j);
            tmp=tmp->suiv;
        }
        */
        enleve_en_tete(&Pile, &x, &y); //On récupère son premier élément
        if(x!=nbcase-1){                                     //Si il n'est pas tout à droite, alors
            if(M[x+1][y]==cl && ! appartient_Zsg(S,x+1,y)){     //si son voisin de droite est de la bonne couleur,
                                                                //et n'appartient pas à la zone
                ajoute_Zsg(S,x+1,y);                            //on l'ajoute à la zone
                ajoute_en_tete(&Pile,x+1,y);                    //et en tête de Pile, afin de traiter ses propres voisins
                cpt++;                                          //enfin on incrémente le compteur du nombre de case ajoutées
            }else{                                           //Sinon
                if(M[x+1][y]!=cl && (S->App)[x+1][y]==-2 ){     //Si son voisin de droite n'est pas de la bonne couleur
                                                                //et s'il n'a pas été déjà traité
                    ajoute_Bordure(S,x+1,y,M[x+1][y]);          //alors on l'ajoute à la bordure de sa couleur
                }
            }
        }

        if(x!=0 ){                                          //Si il n'est pas tout à gauche, alors
            if(M[x-1][y]==cl && ! appartient_Zsg(S,x-1,y)){     //si son voisin de gauche est de la bonne couleur,
                                                                //et n'appartient pas à la zone
                ajoute_Zsg(S,x-1,y);                            //on l'ajoute à la zone
                ajoute_en_tete(&Pile,x-1,y);                    //et en tête de Pile, afin de traiter ses propres voisins
                cpt++;                                          //enfin on incrémente le compteur du nombre de case ajoutées
            }else{                                           //Sinon
                if(M[x-1][y]!=cl && (S->App)[x-1][y]==-2 ){      //Si son voisin de gauche n'est pas de la bonne couleur
                                                                //et s'il n'a pas été déjà traité
                    ajoute_Bordure(S,x-1,y,M[x-1][y]);           //alors on l'ajoute à la bordure de sa couleur
                }
            }
        }

        if(y!=nbcase-1){                                     //Meme fonctionnement pour le bas
            if(M[x][y+1]==cl && ! appartient_Zsg(S,x,y+1)){
                ajoute_Zsg(S,x,y+1);
                ajoute_en_tete(&Pile,x,y+1);
                cpt++;
            }else{
                if(M[x][y+1]!=cl && (S->App)[x][y+1]==-2){
                    ajoute_Bordure(S,x,y+1,M[x][y+1]);
                }
            }
        }

        if(y!=0){                                           //Même fonctionnement pour le haut
            if(M[x][y-1]==cl && ! appartient_Zsg(S,x,y-1)){
                ajoute_Zsg(S,x,y-1);
                ajoute_en_tete(&Pile,x,y-1);
                cpt++;
            }
            else{ 
                if(M[x][y-1]!=cl && (S->App)[x][y-1]==-2){
                    ajoute_Bordure(S,x,y-1,M[x][y-1]);
                }
            }
        }

    }

    return cpt;
}
void libereZSG(S_Zsg *S){
    
    int i;
    for (i=0;i<S->dim;i++){
        
        free(S->App[i]);
    }
    free(S->App);
    detruit_liste(&(S->Lzsg));
    free(S->Lzsg);
    free(S->B);
    
    
}
