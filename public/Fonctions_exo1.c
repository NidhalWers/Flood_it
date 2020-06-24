#include <stdio.h>
#include <stdlib.h>
#include "Entete_Fonctions.h"
#include <time.h>

/* Retourne dans L la liste des cases de meme couleur que la case i,j
   et met -1 dans ces cases */
void trouve_zone_rec(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
	
	int cl=M[i][j];

	ajoute_en_tete(L,i,j); //ajoute la case i,j dans la zone
	M[i][j]=-1; //et on lui met -1, comme demandé dans la consigne

	(*taille)++;
	
	if(i!=nbcase-1 && M[i+1][j]==cl){ //Si le voisin de droite existe et est de la bonne couleur
		trouve_zone_rec(M,nbcase,i+1,j,taille,L); //on lui applique recursivement la fonction
	}

	if(i!=0 && M[i-1][j]==cl){  //Si le voisin de gauche existe et est de la bonne couleur
		trouve_zone_rec(M,nbcase,i-1,j,taille,L); //on lui applique recursivement la fonction
	}

	if(j!=nbcase-1 && M[i][j+1]==cl){  //Si le voisin du bas existe et est de la bonne couleur
		trouve_zone_rec(M,nbcase,i,j+1,taille,L); //on lui applique recursivement la fonction
	}

	if(j!=0 && M[i][j-1]==cl){  //Si le voisin du haut existe et est de la bonne couleur
		trouve_zone_rec(M,nbcase,i,j-1,taille,L); //on lui applique recursivement la fonction
	}

	
}


/* Algorithme tirant au sort une couleur: il utilise la fonction recursive pour determiner la Zsg */
int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff){
	
	srand(time(NULL));
	
	int cpt=0;
	int cl= M[0][0];

	ListeCase L;
	Elnt_liste *cell;

	init_liste(&L);
	
	int taille=0; //taille Zone
	int rand_col; //couleur random
	
	
	while(taille < dim*dim ){
		
		detruit_liste(&L); //on détruit la zone, afin de la reconstruire 
						  //car lors de la reconstruction, les voisins de 
						  //la zone, ayant la même couleur, y seront ajoutés
		taille=0;
		trouve_zone_rec(M,dim,0,0,&taille,&L); //on reconstruit la zone à partir de la couleur de 
											  //la case du coin superieur gauche
											  //ce qui entrainera l'agrandissement de la zone

		if(taille==dim*dim) return cpt;  //à la sortie du trouve_zone_rec taille peut être égale 
										//au nombre de case, donc pas besoin de continuer
		
		//choix alea de la couleur
		do{
			
			rand_col=rand() % nbcl;
			
		}while(rand_col == cl ); //on tire une couleur tant qu'elle est égale à la couleur précédente
		cl=rand_col;
		//printf("\n--------\ncouleur :%d\n",cl);
		
		cpt++;
		

		cell=L;
		while(cell){				//inondation matricielle :
			M[cell->i][cell->j]=cl; //On change de couleur 
			cell=cell->suiv;	    //pour chaque case de la zone
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


