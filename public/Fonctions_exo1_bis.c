#include<stdio.h>
#include <stdlib.h>
#include "Entete_Fonctions.h"
#include <time.h>


/* Retourne dans L la liste des cases de meme couleur que la case i,j
   et met -1 dans ces cases */
void trouve_zone_imp(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
   
	ListeCase Pile;
    init_liste(&Pile);  //Initialisation de la Pile, qui sera utilisée pour remplacer le recursif
    ajoute_en_tete(&Pile, i, j); //On ajoute la case i,j à la pile, la première case traitée
	
    int cl=M[i][j];  //On recupère la couleur de cette case et on la stock dans cl
	//printf("cle=%d et taille=%d\n",cl,nbcase);
	
	ajoute_en_tete(L,i,j);  //ajoute la case i,j dans la zone
	M[i][j]=-1;  //et on lui met -1, comme voulu dans la consigne
	
    *taille=0;
	
	
    int x,y;
    while(! test_liste_vide(&Pile)){
        enleve_en_tete(&Pile, &x, &y); //on retire le première element de la pile
        (*taille)++;

        if(x!=nbcase-1 && M[x+1][y]==cl){ //Si son voisin de droite existe et est de la bonne couleur
	    	ajoute_en_tete(L,x+1, y); //On l'ajoute à la zone
            M[x+1][y]=-1; 			  //on lui met -1
            ajoute_en_tete(&Pile,x+1,y); //et on l'ajoute en tete de Pile,
										// afin de traiter ses propres voisins
	    }

	    if(x!=0 && M[x-1][y]==cl){ //Si son voisin de gauche existe et est de la bonne couleur
	    	ajoute_en_tete(L,x-1, y); //On l'ajoute à la zone
            M[x-1][y]=-1;				//on lui met -1
            ajoute_en_tete(&Pile,x-1,y); //et on l'ajoute en tete de Pile,
										// afin de traiter ses propres voisins
	    }

	    if(y!=nbcase-1 && M[x][y+1]==cl){ //Si son voisin du bas existe et est de la bonne couleur
	    	ajoute_en_tete(L,x, y+1); //On l'ajoute à la zone
            M[x][y+1]=-1;				 //on lui met -1
            ajoute_en_tete(&Pile,x,y+1); //et on l'ajoute en tete de Pile,
										// afin de traiter ses propres voisins
	    }

	    if(y!=0 && M[x][y-1]==cl){ //Si son voisin du haut existe et est de la bonne couleur
	    	ajoute_en_tete(L,x, y-1);  //On l'ajoute à la zone
            M[x][y-1]=-1;				 //on lui met -1
            ajoute_en_tete(&Pile,x,y-1);//et on l'ajoute en tete de Pile,
										// afin de traiter ses propres voisins
	    }

    }
}


/* Algorithme tirant au sort une couleur: il utilise la fonction imperative pour determiner la Zsg */
int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff){
    srand(time(NULL));
	int cpt=0;
	int cl= M[0][0];

	ListeCase L;
	Elnt_liste *cell;

	init_liste(&L);
	
	int taille=0; //taille Zone
	int rand_col; //couleur random
	
	
	while(taille < dim*dim ){
		
		detruit_liste(&L);//on détruit la zone, afin de la reconstruire 
						  //car lors de la reconstruction, les voisins de 
						  //la zone, ayant la même couleur, y seront ajoutés
		taille=0;
		trouve_zone_imp(M,dim,0,0,&taille,&L); //on reconstruit la zone à partir de la couleur de 
											  //la case du coin superieur gauche
											  //ce qui entrainera l'agrandissement de la zone

		if(taille==dim*dim) return cpt; //à la sortie du trouve_zone_rec taille peut être égale 
										//au nombre de case, donc pas besoin de continuer
		
		
		//choix alea de la couleur
		do{
			
			rand_col=rand() % nbcl; //on tire une couleur tant qu'elle est égale à la couleur précédente
			
		}while(rand_col == cl );
		cl=rand_col;
		//printf("\n--------\ncouleur :%d\n",cl);
		
		cpt++;
		

		cell=L;
		while(cell){ 				//inondation matricielle :
			M[cell->i][cell->j]=cl; //On change de couleur
			cell=cell->suiv;		//pour chaque case de la zone
									//Au niveau de la matrice

		}

				
		//inondation graphique :
		//On atribue la nouvelle couleur à chaque case
		//au niveau de la grille
		if(aff==1){
			cell=L;
			while(cell){

				//attribution couleur grille
				//printf("attribue %d comme couleur a zone\n",cl);
				Grille_attribue_couleur_case(G, cell->i, cell->j, cl);
				cell=cell->suiv;

			}
			//Grille_attente_touche();
			Grille_redessine_Grille();
		}

	}
	
	//Grille_ferme_fenetre();
	detruit_liste(&L);
	return cpt;
}