#include "Entete_Fonctions.h"
#include "Structure_acyclique.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sequence_aleatoire_rapide ( int ** M , Grille *G , int dim , int nbcl , int aff ){
    S_Zsg S;
    init_Zsg(&S,dim, nbcl);
    
    Elnt_liste *cell;
    
    int cpt=0, taille=0, cl=M[0][0];
    int rand_col;
    
    taille+=agrandit_Zsg(M, &S, cl, 0, 0); //on effectue agrandit_zone à partir
                                          //de la case supérieur gauche
                                          //C'est l'initialisation de la zone
    

    srand(time(NULL));
    int i, j; //pour l'affichage
    
    while(taille < dim*dim){        
        
        cpt++;

        //choix alea de la couleur
		do{
			
			rand_col=rand() % nbcl; //on tire une couleur tant qu'elle est égale à la couleur précédente
			
		}while(rand_col == cl || ! S.B[rand_col] ); //pour aller encore plus vite on teste la B[cl]
		cl=rand_col;
        //printf("\n--------\ncouleur :%d\n",cl);



        /*
        printf("------------------\naffiche bordure\n");
        int t;
        for(t=0; t<nbcl; t++){
            ListeCase tmp=S.B[t];
            printf("bordure %d :\t",t);
            while(tmp){
                printf(" case %d-%d | ",tmp->i, tmp->j);
                tmp=tmp->suiv;
            }
            printf("\n");
        }
        printf("-------------------\n");
        */
        


        //mise à jour lzsg et bordure
        cell=S.B[cl];
        while(cell){
            if(! appartient_Zsg(&S,cell->i,cell->j)){
                taille+=agrandit_Zsg(M,&S,cl,cell->i, cell->j);
            }
            cell=cell->suiv;
            
        }
        detruit_liste(&(S.B[cl])); //on détruit B[cl], car les cases sont ajoutées à zone lors de agrandit_zone
        
        

        //printf("\t\t\ttaille : %d\n",taille);
        if(aff==1){
            //On inonde seulement quand on demande l'affichage
            //car on peut determiner si les cases sont dans la zone
            //sans changer de couleur,
            //grace à l'attribue App de la structure acyclique
            cell=S.Lzsg;
            while(cell){                //inondation matricielle :
		    	M[cell->i][cell->j]=cl; //On change de couleur 
		    	cell=cell->suiv;        //pour chaque case de la zone
									    //Au niveau de la matrice

		    }
            //inondation graphique :
		    //On atribue la nouvelle couleur à chaque case
		    //au niveau de la grille
            for(i=0; i<dim; i++){
                for(j=0; j<dim; j++){
                    Grille_attribue_couleur_case(G,i,j,M[i][j]);
                
                }
            }
            Grille_attente_touche();
			Grille_redessine_Grille();
        }
    }

    libereZSG(&S);
    return cpt;
}