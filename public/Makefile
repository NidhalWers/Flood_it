all: Flood-It

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c

Structure_acyclique.o : Structure_acyclique.c Structure_acyclique.h
	gcc -c Structure_acyclique.c

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1.c

Fonctions_exo1_bis.o: Fonctions_exo1_bis.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1_bis.c

Fonctions_exo2.o: Fonctions_exo2.c Entete_Fonctions.h Liste_case.h Structure_acyclique.h
	gcc -c Fonctions_exo2.c

graphe_zone.o: graphe_zone.c graphe_zone.h
	gcc -c graphe_zone.c

MaxBordure.o: MaxBordure.c MaxBordure.h
	gcc -c MaxBordure.c

Parcours_largeur.o: Parcours_largeur.c Parcours_largeur.h
	gcc -c Parcours_largeur.c

Strategie_bonus.o: Strategie_bonus.c Strategie_bonus.h
	gcc -c Strategie_bonus.c

Flood-It.o: Flood-It.c
	gcc -c Flood-It.c 

Flood-It: Flood-It.o Liste_case.o Structure_acyclique.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo1_bis.o Fonctions_exo2.o graphe_zone.o MaxBordure.o Parcours_largeur.o Strategie_bonus.o
	gcc -o Flood-It Flood-It.o Liste_case.o Structure_acyclique.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo1_bis.o Fonctions_exo2.o graphe_zone.o MaxBordure.o Parcours_largeur.o Strategie_bonus.o -lSDL

Partie2 : Strategie_bonus.o Parcours_largeur.o graphe_zone.o Liste_case.o Structure_acyclique.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo1_bis.o Fonctions_exo2.o
	gcc -o Partie2 Strategie_bonus.o Parcours_largeur.o graphe_zone.o Liste_case.o Structure_acyclique.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo1_bis.o Fonctions_exo2.o -lSDL

clean:
	rm -f *.o Flood-It_Partie1
