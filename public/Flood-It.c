#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"
#include "graphe_zone.h"
#include "MaxBordure.h"
#include "Parcours_largeur.h"
#include "Strategie_bonus.h"
#include "Structure_acyclique.h"
 
int main(int argc,char**argv){
    
  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=7){
    printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-1-2> <aff 0/1>\n",argv[0]);
    return 1;
  }
  printf("debut\n\n");
  dim=atoi(argv[1]);


  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Generation de l'instance */
  if(exo!=6){
    M=(int **) malloc(sizeof(int*)*dim);
    for (i=0;i<dim;i++){
      M[i]=(int*) malloc(sizeof(int)*dim);
      if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
    }
  
  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);
  }
  /* Affichage de la grille */

  if (aff==1){
    Grille_init(dim,nbcl, 500,&G);

    Grille_ouvre_fenetre(G);

    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }

    Grille_redessine_Grille();
  }

    if(exo != 6){
        temps_initial = clock ();
    }
  



  if (exo==0){

    int res=sequence_aleatoire_rec(M, G, dim, nbcl, aff);    
    printf("resultat sequence aleatoire recursif : %d\n", res); 
     
  }
  if(exo==1){
    printf("resultat sequence aleatoire impératif : %d\n", sequence_aleatoire_imp(M, G, dim, nbcl, aff));
  }

  if(exo==2){
    printf("resultat sequence aleatoire rapide : %d\n", sequence_aleatoire_rapide(M, G, dim, nbcl, aff));
    
  }
  if(exo==3){
    Graphe_zone  Gr= cree_graphe_zone(dim,M);
    S_ZsgGr *S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    init_ZsgGr(S,dim,nbcl);

    //affiche_graphe(&Gr,dim);
    

    int res=strategie_max_B(M,G,dim,nbcl,aff,&Gr, S, 0);
    printf("resultat stratégie max bordure : %d\n",res);
    
    
    libere_ZsgGr(S);
    libere_Graphe_zone(&Gr, dim);
    
       
  }
  if(exo==4){
    Graphe_zone  Gr= cree_graphe_zone(dim,M);
    S_ZsgGr *S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    init_ZsgGr(S,dim,nbcl);
    

    //affiche_graphe(&Gr,dim);
    
    int res=strategie_parcours_L(M,G,dim,nbcl,aff,&Gr, S);
    printf("resultat stratégie parcours largeur : %d\n",res);


    libere_ZsgGr(S);
    libere_Graphe_zone(&Gr, dim);
    
  }
  if(exo==5){

    Graphe_zone  Gr= cree_graphe_zone(dim,M);
    S_ZsgGr *S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    init_ZsgGr(S,dim,nbcl);
    //affiche_graphe(&Gr,dim);

   
    printf("Choisissez la stratégie :\n");
    printf("1 - Stratégie deux coins\n");
    printf("2 - Stratégie trois coins\n");
    int ch, lus, res;
    lus = scanf("%d", &ch);
    

    switch( ch){
    	case 1 :
		{
			res=strategie_deux_coins( M,G,dim,nbcl,aff,&Gr, S);
    		printf("resultat stratégie deux coins : %d\n",res);
			break;
		}
		case 2 :
		{
			res=strategie_trois_coins( M,G,dim,nbcl,aff,&Gr, S);
    		printf("resultat stratégie trois coins : %d\n",res);
			break;
		}
		default :
			break ;
    }


    libere_ZsgGr(S);
    libere_Graphe_zone(&Gr, dim);   
	

  }

  ////////////////////////////////////////////////////////////////////// Zone de test
  if(exo==6){


    printf("Choisissez la valeur à faire varier :\n");
    printf("1 - dimension\n");
    printf("2 - nombre couleur\n");
    printf("3 - difficulté\n");
    int ch, lus;
    lus = scanf("%d", &ch);

    switch( ch){
    	case 1 :
		{	
			
            printf("Choisissez la fonction tester :\n");
            printf("1 - aléatoire rec\n");
            printf("2 - aléatoire imp\n");
            printf("3 - aléatoire rapide\n");
            printf("4 - max bordure\n");
            printf("5 - parcours largeur\n");
            printf("6 - deux coins\n");
            int ch2, lus2;
            lus2 = scanf("%d", &ch2);



            clock_t temps_initial; 
    		clock_t temps_final;
    		float temps_cpu; 

            int res;
            Graphe_zone  Gr;
            S_ZsgGr *S;

    		temps_cpu=0;
    		int aff=0;
			int nb_cl=10;
    		int nb_diff=0;
			int seed;  
			int temoin;	//nombre de test 
			int taille;  //le paramètre qui va varier
			int temoin_max=15; //le nombre de temoin max

            switch( ch2){
    	        case 1 :
		        {
                    int limite=200;
                    float *tab_temps_alea_rec=(float*)malloc(sizeof(float)*(limite/10));
                    float *tab_essais_alea_rec=(float*)malloc(sizeof(float)*(limite/10));
                    

                    for(taille=10; taille<=limite; taille+=10){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;          
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea rec\n");
                            temps_initial = clock ();
                            res=sequence_aleatoire_rec(M,G,taille,nb_cl,0);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_alea_rec[(taille/10)-1]+=res/temoin_max;
                            tab_temps_alea_rec[(taille/10)-1]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rec_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=10){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_alea_rec[(taille/10)-1],tab_temps_alea_rec[(taille/10)-1]);
                    }

		        	break;
		        }
		        case 2 :
		        {
                    int limite=200;
                    float *tab_temps_alea_imp=(float*)malloc(sizeof(float)*(limite/10));
                    float *tab_essais_alea_imp=(float*)malloc(sizeof(float)*(limite/10));
                    

                    for(taille=10; taille<=limite; taille+=10){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;        
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea imp\n");
						    temps_initial = clock ();
          				    res=sequence_aleatoire_imp(M,G,taille,nb_cl,0);
        				    temps_final = clock ();
          				    temps_cpu=( temps_final - temps_initial ) * 1e-6;

						    tab_essais_alea_imp[(taille/10)-1]+=res/temoin_max;
						    tab_temps_alea_imp[(taille/10)-1]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_imp_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=10){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_alea_imp[(taille/10)-1],tab_temps_alea_imp[(taille/10)-1]);
                    }

		        	break;
		        }
                 case 3 :
		        {
                    int limite=1500;   
                    int saut=100;
                    float *tab_temps_alea_rapide=(float*)malloc(sizeof(float)*(limite/saut));
                    float *tab_essais_alea_rapide=(float*)malloc(sizeof(float)*(limite/saut));
                    

                    for(taille=10; taille<=limite; taille+=saut){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea rapide\n");
						                temps_initial = clock ();
          			            res=sequence_aleatoire_rapide(M,G,taille,nb_cl,0);
        				            temps_final = clock ();
          			            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_alea_rapide[(taille/saut)-1]+=res/temoin_max;
						                tab_temps_alea_rapide[(taille/saut)-1]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rapide_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=saut){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_alea_rapide[(taille/saut)-1],tab_temps_alea_rapide[(taille/saut)-1]);
                    }
		        	break;
		        }
		        case 4 :
		        {
                    int limite=1500;
                    int saut=100;
                    float *tab_temps_max_B=(float*)malloc(sizeof(float)*(limite/saut));
                    float *tab_essais_max_B=(float*)malloc(sizeof(float)*(limite/saut));
                    

                    for(taille=10; taille<=limite; taille+=saut){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);
                            temps_initial = clock ();
                            //printf("\t\tmax bordure\n");
						                Gr= cree_graphe_zone(taille,M);
    					              S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    					              init_ZsgGr(S,taille,nb_cl);

						                temps_initial = clock ();
    					              res=strategie_max_B(M,G,taille,nb_cl,0,&Gr, S, 0);
						                temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_max_B[(taille/saut)-1]+=res/temoin_max;
						                tab_temps_max_B[(taille/saut)-1]+=temps_cpu/temoin_max;

    					              libere_ZsgGr(S);
    					              libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Max_Bordure_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=saut){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_max_B[(taille/saut)-1],tab_temps_max_B[(taille/saut)-1]);
                    }

		        	break;
		        }
                 case 5 :
		        {
                    int limite=1500;
                    int saut=100;
                    float *tab_temps_parcours_L=(float*)malloc(sizeof(float)*(limite/saut));
                    float *tab_essais_parcours_L=(float*)malloc(sizeof(float)*(limite/saut));
                    

                    for(taille=10; taille<=limite; taille+=saut){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);
                            temps_initial = clock ();
                            //printf("\t\tparcours largeur\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);

                            
                            res=strategie_parcours_L(M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_parcours_L[(taille/saut)-1]+=res/temoin_max;
						                tab_temps_parcours_L[(taille/saut)-1]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Parcours_largeur_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=saut){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_parcours_L[(taille/saut)-1],tab_temps_parcours_L[(taille/saut)-1]);
                    }

		        	break;
		        }
		        case 6 :
		        {
                    int limite=1500;
                    int saut=100;
                    float *tab_temps_deux_coins=(float*)malloc(sizeof(float)*(limite/saut));
                    float *tab_essais_deux_coins=(float*)malloc(sizeof(float)*(limite/saut));
                    
                    
                    for(taille=10; taille<=limite; taille+=100){  //on fait varier la taille 10 par 10
                        //printf("\ttaille n°%d\n",taille);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();

                            //printf("\t\tdeux coins\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);
                            
                            
                            res=strategie_deux_coins( M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_deux_coins[(taille/saut)-1]+=res/temoin_max;
						                tab_temps_deux_coins[(taille/saut)-1]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);  

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Deux_coins_dimension.txt","w");
                    fprintf(F1, "taille  |  nombre d'essais  |  temps cpu\n");
                    for(taille=10; taille<=limite; taille+=saut){
                      fprintf(F1, "%d\t%f\t%f\n",taille,tab_essais_deux_coins[(taille/saut)-1],tab_temps_deux_coins[(taille/saut)-1]);
                    }
		        	break;
		        }
		        default :
		        	break ;
                }
			
			break;
		}
		case 2 : /////////////////////////////////////////////////////////////////////////////////////////
		{
            printf("Choisissez la fonction tester :\n");
            printf("1 - aléatoire rec\n");
            printf("2 - aléatoire imp\n");
            printf("3 - aléatoire rapide\n");
            printf("4 - max bordure\n");
            printf("5 - parcours largeur\n");
            printf("6 - deux coins\n");
            int ch2, lus2;
            lus2 = scanf("%d", &ch2);



            clock_t temps_initial; 
    		clock_t temps_final;
    		float temps_cpu; 

            int res;
            Graphe_zone  Gr;
            S_ZsgGr *S;

    		temps_cpu=0;
    		int aff=0;
			int nb_cl; //le paramètre qui va varier
    		int nb_diff=0;
			int seed;  
			int temoin;	//nombre de test 
			int taille=100;  
			int temoin_max=15; //le nombre de temoin max

            switch( ch2){
    	        case 1 :
		        {
                    int limite=20;
                    float *tab_temps_alea_rec=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_alea_rec=(float*)malloc(sizeof(float)*(limite+1-5));
                    

                    for(nb_cl=5; nb_cl<=limite; nb_cl++){  //on fait varier la taille 10 par 10
                        //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea rec\n");
                            temps_initial = clock ();
                            res=sequence_aleatoire_rec(M,G,taille,nb_cl,0);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_alea_rec[nb_cl-5]+=res/temoin_max;
                            tab_temps_alea_rec[nb_cl-5]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rec_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_alea_rec[nb_cl-5],tab_temps_alea_rec[nb_cl-5]);
                    }

		        	break;
		        }
		        case 2 :
		        {
                    int limite=20;
                    float *tab_temps_alea_imp=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_alea_imp=(float*)malloc(sizeof(float)*(limite+1-5));
                    

                    for(nb_cl=5; nb_cl<=limite; nb_cl++){//on fait varier la taille 10 par 10
                        //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea imp\n");
						                temps_initial = clock ();
          			            res=sequence_aleatoire_imp(M,G,taille,nb_cl,0);
        				            temps_final = clock ();
          			            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_alea_imp[nb_cl-5]+=res/temoin_max;
						                tab_temps_alea_imp[nb_cl-5]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_imp_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_alea_imp[nb_cl-5],tab_temps_alea_imp[nb_cl-5]);
                    }

		        	break;
		        }
                 case 3 :
		        {
                    int limite=20;
                    float *tab_temps_alea_rapide=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_alea_rapide=(float*)malloc(sizeof(float)*(limite+1-5));
                    

                    for(nb_cl=5; nb_cl<=limite; nb_cl++){  //on fait varier la taille 10 par 10
                       //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();

                            //printf("\t\tsequence alea rapide\n");
						    
          			            res=sequence_aleatoire_rapide(M,G,taille,nb_cl,0);
        				            temps_final = clock ();
          			            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_alea_rapide[nb_cl-5]+=res/temoin_max;
						                tab_temps_alea_rapide[nb_cl-5]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rapide_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_alea_rapide[nb_cl-5],tab_temps_alea_rapide[nb_cl-5]);
                    }
		        	break;
		        }
		        case 4 :
		        {
                    int limite=20;
                    float *tab_temps_max_B=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_max_B=(float*)malloc(sizeof(float)*(limite+1-5));
                    

                   for(nb_cl=5; nb_cl<=limite; nb_cl++){  //on fait varier la taille 10 par 10
                        //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();

                            //printf("\t\tmax bordure\n");
						                Gr= cree_graphe_zone(taille,M);
    					              S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    					              init_ZsgGr(S,taille,nb_cl);

		            				    
    					              res=strategie_max_B(M,G,taille,nb_cl,0,&Gr, S, 0);
						                temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

            						    tab_essais_max_B[nb_cl-5]+=res/temoin_max;
						                tab_temps_max_B[nb_cl-5]+=temps_cpu/temoin_max;

                					                libere_ZsgGr(S);
    					                libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Max_Bordure_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_max_B[nb_cl-5],tab_temps_max_B[nb_cl-5]);
                    }

		        	break;
		        }
                 case 5 :
		        {
                    int limite=20;
                    float *tab_temps_parcours_L=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_parcours_L=(float*)malloc(sizeof(float)*(limite+1-5));
                    

                    for(nb_cl=5; nb_cl<=limite; nb_cl++){   //on fait varier la taille 10 par 10
                        //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();//printf("\t\tparcours largeur\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);

                            
                            res=strategie_parcours_L(M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_parcours_L[nb_cl-5]+=res/temoin_max;
						                tab_temps_parcours_L[nb_cl-5]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Parcours_largeur_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_parcours_L[nb_cl-5],tab_temps_parcours_L[nb_cl-5]);
                    }

		        	break;
		        }
		        case 6 :
		        {
                    int limite=20;
                    float *tab_temps_deux_coins=(float*)malloc(sizeof(float)*(limite+1-5));
                    float *tab_essais_deux_coins=(float*)malloc(sizeof(float)*(limite+1-5));
                    
                    
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){  //on fait varier la taille 10 par 10
                        //printf("\tnb couleur %d\n",nb_cl);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();//printf("\t\tdeux coins\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);
                            
                            
                            res=strategie_deux_coins( M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_deux_coins[nb_cl-5]+=res/temoin_max;
						                tab_temps_deux_coins[nb_cl-5]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);  

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Deux_coins_couleur.txt","w");
                    fprintf(F1, "couleur  |  nombre d'essais  |  temps cpu\n");
                    for(nb_cl=5; nb_cl<=limite; nb_cl++){
                      fprintf(F1, "%d\t%f\t%f\n",nb_cl,tab_essais_deux_coins[nb_cl-5],tab_temps_deux_coins[nb_cl-5]);
                    }
		        	break;
		        }
		        default :
		        	break ;
                }
			break;
		}
    case 3 ://///////////////////////////////////////////////////////////////////////////////////
		{
			printf("Choisissez la fonction tester :\n");
            printf("1 - aléatoire rec\n");
            printf("2 - aléatoire imp\n");
            printf("3 - aléatoire rapide\n");
            printf("4 - max bordure\n");
            printf("5 - parcours largeur\n");
            printf("6 - deux coins\n");
            int ch2, lus2;
            lus2 = scanf("%d", &ch2);



            clock_t temps_initial; 
    		clock_t temps_final;
    		float temps_cpu; 

            int res;
            Graphe_zone  Gr;
            S_ZsgGr *S;

    		temps_cpu=0;
    		int aff=0;
			int nb_cl=10; 
    		int nb_diff; //le paramètre qui va varier
			int seed;  
			int temoin;	//nombre de test 
			int taille=1010;  
			int temoin_max=15; //le nombre de temoin max

            switch( ch2){
    	        case 1 :
		        {
                    int limite=15;
                    float *tab_temps_alea_rec=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_alea_rec=(float*)malloc(sizeof(float)*(limite+1));
                    

                    for(nb_diff=0; nb_diff<=limite; nb_diff++){  //on fait varier la taille 10 par 10
                        //printf("\tnb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea rec\n");
                            temps_initial = clock ();
                            res=sequence_aleatoire_rec(M,G,taille,nb_cl,0);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_alea_rec[nb_diff]+=res/temoin_max;
                            tab_temps_alea_rec[nb_diff]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rec_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_alea_rec[nb_diff],tab_temps_alea_rec[nb_diff]);
                    }

		        	break;
		        }
		        case 2 :
		        {
                    int limite=15;
                    float *tab_temps_alea_imp=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_alea_imp=(float*)malloc(sizeof(float)*(limite+1));
                    

                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ //on fait varier la difficulté 
                        //printf("\tnb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque difficulté
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea imp\n");
						                temps_initial = clock ();
          			            res=sequence_aleatoire_imp(M,G,taille,nb_cl,0);
        				            temps_final = clock ();

          			            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_alea_imp[nb_diff]+=res/temoin_max;
						                tab_temps_alea_imp[nb_diff]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_imp_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_alea_imp[nb_diff],tab_temps_alea_imp[nb_diff]);
                    }

		        	break;
		        }
                 case 3 :
		        {
                    int limite=15;
                    float *tab_temps_alea_rapide=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_alea_rapide=(float*)malloc(sizeof(float)*(limite+1));
                    

                    for(nb_diff=0; nb_diff<=limite; nb_diff++){   //on fait varier la taille 10 par 10
                       //printf("\tnb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            //printf("\t\tsequence alea rapide\n");
						                temps_initial = clock ();
          			            res=sequence_aleatoire_rapide(M,G,taille,nb_cl,0);
        				            temps_final = clock ();
          			            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_alea_rapide[nb_diff]+=res/temoin_max;
						                tab_temps_alea_rapide[nb_diff]+=temps_cpu/temoin_max;

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Alea_rapide_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_alea_rapide[nb_diff],tab_temps_alea_rapide[nb_diff]);
                    }
		        	break;
		        }
		        case 4 :
		        {
                    int limite=15;
                    float *tab_temps_max_B=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_max_B=(float*)malloc(sizeof(float)*(limite+1));
                    

                   for(nb_diff=0; nb_diff<=limite; nb_diff++){   //on fait varier la taille 10 par 10
                        //printf("\tnb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();//printf("\t\tmax bordure\n");
						                Gr= cree_graphe_zone(taille,M);
    					              S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
    					              init_ZsgGr(S,taille,nb_cl);

						                
    					              res=strategie_max_B(M,G,taille,nb_cl,0,&Gr, S, 0);
						                temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

						                tab_essais_max_B[nb_diff]+=res/temoin_max;
						                tab_temps_max_B[nb_diff]+=temps_cpu/temoin_max;

    					                libere_ZsgGr(S);
    					                libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Max_Bordure_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){  
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_max_B[nb_diff],tab_temps_max_B[nb_diff]);
                    }

		        	break;
		        }
                 case 5 :
		        {
                    int limite=15;
                    float *tab_temps_parcours_L=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_parcours_L=(float*)malloc(sizeof(float)*(limite+1));
                    

                    for(nb_diff=0; nb_diff<=limite; nb_diff++){    //on fait varier la taille 10 par 10
                        //printf("\tnb nb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();//printf("\t\tparcours largeur\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);

                            
                            res=strategie_parcours_L(M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_parcours_L[nb_diff]+=res/temoin_max;
						    tab_temps_parcours_L[nb_diff]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Parcours_largeur_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_parcours_L[nb_diff],tab_temps_parcours_L[nb_diff]);
                    }

		        	break;
		        }
		        case 6 :
		        {
                    int limite=15;
                    float *tab_temps_deux_coins=(float*)malloc(sizeof(float)*(limite+1));
                    float *tab_essais_deux_coins=(float*)malloc(sizeof(float)*(limite+1));
                    
                    
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){   //on fait varier la taille 10 par 10
                        //printf("\tnb diff %d\n",nb_diff);
                        for(temoin=0; temoin<temoin_max; temoin++){  //15 essais pour chaque taille
                            seed=temoin;
                            M=(int **) malloc(sizeof(int*)*taille);
                            for (i=0;i<taille;i++){
                                M[i]=(int*) malloc(sizeof(int)*taille);

                                if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
                            }
                            Gene_instance_genere_matrice(taille, nb_cl, nb_diff, seed, M);

                            temps_initial = clock ();//printf("\t\tdeux coins\n");
                            Gr= cree_graphe_zone(taille,M);
                            S=(S_ZsgGr*)malloc(sizeof(S_ZsgGr));
                            init_ZsgGr(S,taille,nb_cl);
                            
                            
                            res=strategie_deux_coins( M,G,taille,nb_cl,0,&Gr, S);
                            temps_final = clock ();
                            temps_cpu=( temps_final - temps_initial ) * 1e-6;

                            tab_essais_deux_coins[nb_diff]+=res/temoin_max;
						    tab_temps_deux_coins[nb_diff]+=temps_cpu/temoin_max;

                            libere_ZsgGr(S);
                            libere_Graphe_zone(&Gr, taille);  

                            for(i = 0; i< taille; i++) {
                            if (M[i]){free(M[i]);}
                            }if (M){ free(M); }
                        
                        }
                    }

                    ///////  ECRITURE DANS FICHIER ////////
                    FILE *F1=fopen("Deux_coins_difficulte.txt","w");
                    fprintf(F1, "difficulte  |  nombre d'essais  |  temps cpu\n");
                    for(nb_diff=0; nb_diff<=limite; nb_diff++){ 
                      fprintf(F1, "%d\t%f\t%f\n",nb_diff,tab_essais_deux_coins[nb_diff],tab_temps_deux_coins[nb_diff]);
                    }
		        	break;
		        }
		        default :
		        	break ;
                }
			break;
		}
	default :
		break ;
    }     
        
}


  
  
  //////////////////////////////////////////////////////////////////////

  if(exo!=6){
    for(i = 0; i< dim; i++) {
      if (M[i]){free(M[i]);}
      }
    if (M) free(M);
  
  
  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
  printf("%f\n",temps_cpu);

  }
  
  if (aff==1){
   
    Grille_ferme_fenetre();

    Grille_free(&G);
  }
  printf("fin main\n");
  return 0;
}
