#include "graphe_zone.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Entete_Fonctions.h"
#include "MaxBordure.h"


/* Initialise la structure */
void init_ZsgGr(S_ZsgGr *S, int dim, int nbcl){
    
    S->dim=dim;
    
    S->Szsg=NULL;
    
    S->dim=dim;
    S->nbcl=nbcl;
    S->BnbS= malloc(nbcl * sizeof(int));
    
    //printf("dimension : %d | %d \n",dim, S->dim);
    //printf("couleur : %d | %d \n",nbcl, S->nbcl);
    
    S->B=(Cellule_som **) malloc(nbcl * sizeof(Cellule_som*));
    int i;
   
    for (i=0; i<S->nbcl; i++){
        S->BnbS[i]=0;
    }
    
    for(i=0; i<S->nbcl; i++){
        (S->B)[i]=NULL;  
    }
    

}



/*Ajoute une case dans la liste Lzsg */
int ajoute_Zsg_sommet(S_ZsgGr *S,Sommet *s){
    
    ajoute_liste_sommet(&(S->Szsg),s);
    s->marque=0;

    
}
//ajoute les sommets dans le tableaux a la bonne couleur 
void ajout_BordureGraphe(S_ZsgGr *S, Sommet *s, int cl){
    //printf("ajoute %d dans bordure %d\n", s->num, cl);
    ajoute_liste_sommet(&(S->B)[cl],s);
    s->marque=1;
    (S->BnbS)[cl]++;
   
}
/* Ajoute une case dans la bordure d'une couleur cl donnée */


/* Renvoie vrai si une case est dans Lzsg */
int appartient_Zsg_sommet(Sommet *s){
    return s->marque==0;
}


/* Renvoie vrai si une case est dans la bordure de couleur cl donnée */
int appartient_bordure_sommet(Sommet *s){
    return s->marque==1;

}


/* met à jour la Zsg et la bordure, la fonction retourne le nombre de case
ajoutée à la Zsg  */

int majBordureGraphe(S_ZsgGr *S, int cl,int *t ){
    //printf("maj bordu %d\n", cl);

    //recupere la bordure la partie du tableau avec la couleur qui nous interesse 
    Cellule_som *tete=(S->B)[cl];
    //ajoute chaque sommet dans notre zone 
    while(tete){
        
        //printf("ajoute %d dans zone\n",tete->sommet->num);
        ajoute_Zsg_sommet(S,tete->sommet);
        
        tete=tete->suiv;
        
        (*t)++;
    }
    //on vide cette partie du tableau
    detruit_liste_somme((S->B)[cl]);
    (S->B)[cl]=NULL;
    return *t;
}


//application de la stratégie
int strategie_max_B( int ** M , Grille *Gr , int dim , int nbcl , int aff, Graphe_zone *G,  S_ZsgGr *S, int t){
    int nbessai=0;
    
    int taille=t;
    //vérification du premier sommet pour l'ajouter a la bordure si il ne l'est pas deja 
    if(!appartient_Zsg_sommet(G->mat[0][0])){
        ajoute_Zsg_sommet(S,G->mat[0][0]);
        taille++;
    
   
    //ajout de la bordure du premier sommet, initialisation 
        while ( G->mat[0][0]->sommet_adj){

            
            ajout_BordureGraphe(S,G->mat[0][0]->sommet_adj->sommet,G->mat[0][0]->sommet_adj->sommet->cl);

            G->mat[0][0]->sommet_adj=G->mat[0][0]->sommet_adj->suiv;

        }
    }

   
    int i, clC, index;
    while(taille<G->nbsom){ //lorsque tous les sommets sont dans la zone
            
        clC=0;
        
        //affi_bordure(S);
        //on choisit la bordure la plus rentable avec BnbS, 
        //c'est un tableau qui repertorie le nombre dans sommet de la même couleur dans la bordure
        for (i=0;i<nbcl;i++){
            //printf("comparaison rentable  : %d / clC= %d\n",S->BnbS[i],clC);
             if(S->BnbS[i]>clC){
                clC=S->BnbS[i];
                index=i;
            }
        }
        
        //printf("bordure rentable :%d\n",index);
       
        Cellule_som *adj=NULL;
        //met à jour la bordure

        //bor est donc la liste de sommet dans la bordure que l'on va integrer dans la zone
        Cellule_som *bor=(S->B)[index];

        while(bor){              
        
            
            adj=bor->sommet->sommet_adj;
            
            //printf("\t cherche adjacent de %d : \n",bor->sommet->num);

            //pour chaque sommet on rajoute dans la bordure 
            //on rajoute tout les sommet adjacent a ce sommet qui n'y est pas deja
            while(adj){
               
                //on verifie avec marque que le sommet n'est pas deja en bordure ou dans la zone 
                if(adj->sommet->marque==2){
                    ajout_BordureGraphe(S,adj->sommet,adj->sommet->cl);
                }
                adj=adj->suiv;
            }
            bor=bor->suiv;
            
        }
        //affi_bordure(S);
        
        //on met a jour la zone
        majBordureGraphe(S,index,&taille);

        
        
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
                    Grille_attribue_couleur_case(Gr,x,y,index);
                    som_case=som_case->suiv;
                }
                som_zone=som_zone->suiv;
            }
            //Grille_attente_touche();
			Grille_redessine_Grille();
        }
        

        S->BnbS[index]=0;

        nbessai++;
        //printf("\t\t\tnb essai = %d, taille %d, couleur %d\n", nbessai,taille,index);
    

    }
    return nbessai;
}

void libere_ZsgGr(S_ZsgGr *S){
    //liberation du tableau de listes des sommet de la bordure par couleur */
    free(S->BnbS);
    detruit_liste_somme(S->Szsg);
    int i;
    for(i=0; i<S->nbcl; i++){
        detruit_liste_somme(S->B[i]);
    }
    free(S->B);
    free(S);
}









//fonction affichant le contenu de la bordure 
void affi_bordure(S_ZsgGr *S){
    printf("bordure\n");
    int i;
    for(i=0; i<S->nbcl; i++){
        printf("%d :",i);
        Cellule_som *bord=S->B[i];
        while(bord){
            printf("som %d\t",bord->sommet->num);
            bord=bord->suiv;
        }
        printf("\n");
    }
}