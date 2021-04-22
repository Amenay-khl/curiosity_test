#include "environnement.h"
#include "programme.h"
#include "interprete.h"

#include <stdio.h>
#include <stdlib.h>
//Fichier de Amenay et Yohan pour le projet curiosity en INF304

void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {
  case OK: break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf("Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for(i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch(e.type_err) {
  case OK_PROGRAMME: break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char ** argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;


  if (argc < 2) {
    printf("Usage: %s <fichier_test>\n", argv[0]);
    return 1;
  }

	FILE *f = fopen(argv[1], "r");
	char fichier_terrain[100];
	char fichier_programme[100];
	int pas;
	char etat_attendu;
	int compteur = 0;
	int position_x = 0;
	int position_y = 0;
	char direction = '\0';   //orientation
	Orientation direction_2;

	fscanf(f, "%s", fichier_terrain);
	fscanf(f, "%s", fichier_programme);
	fscanf(f, "%d", &pas);
	do
        {
		fscanf(f, "%c", &etat_attendu);
        }
    while(etat_attendu == '\n');
	if ((etat_attendu == 'N') || (etat_attendu == 'F'))
        {
		fscanf(f, "%d %d", &position_x, &position_y);
		do
		{
			fscanf(f, "%c", &direction);
		}
    while(direction == '\n');
        }
	switch(direction)
	{
		case 'N':
			direction_2 = Nord;
			break;
		case 'E':
			direction_2 = Est;
			break;
		case 'S':
			direction_2 = Sud;
			break;
		case 'O':
			direction_2 = Ouest;
			break;
	}

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
  errt = initialise_environnement(&envt, fichier_terrain);
  gestion_erreur_terrain(errt);

  /* Lecture du programme */
  errp = lire_programme(&prog, fichier_programme);
  gestion_erreur_programme(errp);

  /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */
    afficher_envt(&envt);
		compteur++;
  } while((res == OK_ROBOT) && (compteur <= pas));

  /* Affichage du résultat */

  switch(res)
    {
		case OK_ROBOT:
			if (etat_attendu != 'F')
				printf("l'evenement de fin est incorrect,l'evenement attendu est  : %c\n", etat_attendu);
			printf("Robot sur une case libre, programme non terminé \n");
			break;
		case SORTIE_ROBOT:
			if (etat_attendu != 'S')
				printf("l'evenement de fin est incorrect,l'evenement attendu est  : %c\n", etat_attendu);
			printf("Le robot est sorti :-)\n");
			break;
		case ARRET_ROBOT:
			if (etat_attendu != 'N')
				printf("Evenement de fin incorrect, evenement attendu : %c\n", etat_attendu);
			printf("Robot sur une case libre, programme terminé :-/\n");
			break;
		case PLOUF_ROBOT:
			if (etat_attendu != 'P')
				printf("l'evenement de fin est incorrect,l'evenement attendu est : %c\n", etat_attendu);
			printf("Le robot est tombé dans l'eau :-(\n");
			break;
		case CRASH_ROBOT:
			if (etat_attendu != 'O')
				printf("l'evenement de fin est incorrect,l'evenement attendu est : %c\n", etat_attendu);
			printf("Le robot s'est écrasé sur un rocher X-(\n");
			break;
		case ERREUR_PILE_VIDE:
			printf("ERREUR : pile vide\n");
			break;
		case ERREUR_ADRESSAGE:
			printf("ERREUR : erreur d'adressage\n");
			break;
		case ERREUR_DIVISION_PAR_ZERO:
			printf("ERREUR : division par 0\n");
			break;
    }

	if ((etat_attendu == 'N') || (etat_attendu == 'F'))
        {
		if (envt.r.x != position_x || envt.r.y != position_y)
			printf("Position finale incorrecte, attendue : %d %d, actuelle : %d %d\n", position_x, position_y, envt.r.x, envt.r.y);
		else printf("Pos OK\n");
		if (envt.r.o != direction_2)
			printf("Orientation finale incorrecte, attendue : %c\n", direction);
		else printf("Orientation OK\n");
        }
}
