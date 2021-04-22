#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "terrain.h"
#include "interprete.h"
#include "programme.h"
#include "environnement.h"
#include "generation_terrains.h"
#include "type_pile.h"
#include "robot.h"
//Fichier de Amenay et Yohan pour le projet curiosity en INF304

// compte le nombre de cases non libre du terrain
int cases_prises(Terrain T)
    {
	int cases = 0;
	int i, j;
	for(i=0; i<largeur(T); i++)
        {
		for(j=0; j<hauteur(T); j++)
            {
			if(T.tab[i][j] != LIBRE) cases = cases +1;
            }
        }
	return cases;
    }

int main(int argc, char ** argv)
    {
    int N, l, h, graine, pas_max, pas;
    float dObst, nombre_Obst;
    float  nombre_sorti = 0, nombre_bloque = 0, nombre_crash = 0;
    FILE* resFile;
    Environnement envt;
    Programme prog;
    etat_inter etat;
    resultat_inter res;

    if (argc < 9)
        {
        printf("Usage: %s <fichier_programme> <N> <largeur> <hauteur> <densite_obstacle> <graine> <nb_step_max> <fichier_res> \n", argv[0]);
        return 1;
        }
    N = strtol(argv[2], NULL, 10);   //recupêre le nombre de terrains a creer
    l = strtol(argv[3], NULL, 10);   // recupere la largeur
    h = strtol(argv[4], NULL, 10);  // recupere la hauteur
    dObst = strtof(argv[5], NULL);  //recupere la densité
    graine = atoi(argv[6]);        //recupere la graine
    pas_max = atoi(argv[7]);       // recupere le nombre de pas max authorisé

 // test de la largeur et de la hauteur, puis de la densité
    if (l > DIM_MAX || l%2 == 0)
        {
        printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
        }
    if (h > DIM_MAX || h%2 == 0)
        {
        printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
        }
    if ((dObst > 1) || (dObst < 0))
        {
        printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
        return 1;
        }


    resFile = fopen(argv[8], "w");   // Ouverture du fichier résultat
    fprintf(resFile, "%d\n", N);

	srand(graine);     //initialisation de la graine.

	envt.t.largeur = l;
	envt.t.hauteur = h;

	for(int i = 0; i < N; i++)
        {
		do
            {
			generation_aleatoire(&(envt.t), l, h, dObst);
            } while(!chemin(envt.t));
		ecrire_terrain(resFile, envt.t, (l/2), (h/2));
		nombre_Obst = (cases_prises(envt.t));
		fprintf(resFile, " la densité effective d'obstacle est de :\n%f %%\n\n", ((nombre_Obst)/(l*h))*100);   //((nombre_Obst)/(l*h))*100) est exactement le pourcentage d'obstacles sur le terrain

		lire_programme(&prog, argv[1]);
        // initialisation de la position initiale du robot, de son orientation et du nombre de pas réalisé
		envt.r.x = l/2;
		envt.r.y = h/2;
		envt.r.o = Est;
		pas = 0;

		init_etat(&etat);
		do  {
		  	res = exec_pas(&prog, &envt, &etat);   //on execute chaque action du programme
		  	afficher_envt(&envt);
			pas++;
            } while((res == OK_ROBOT) && (pas < pas_max));

		/* Affichage du résultat */
		switch(res) {
		case OK_ROBOT: fprintf(resFile, "%d Nombre de pas limite dépassé\n\n\n\n", pas_max); nombre_bloque++; break;
		case SORTIE_ROBOT: fprintf(resFile, "Le robot est sorti en %d pas sur %d au maximum \n\n\n\n", pas, pas_max); nombre_sorti++; break;
		case ARRET_ROBOT: fprintf(resFile, " Robot sur une case libre, programme terminé \n\n\n\n"); nombre_bloque++; break;
		case PLOUF_ROBOT: fprintf(resFile, " Le robot est tombé dans l'eau\n\n\n\n"); nombre_crash++; break;
		case CRASH_ROBOT: fprintf(resFile, " Le robot s'est écrasé sur un rocher \n\n\n\n"); nombre_crash++; break;
		case ERREUR_PILE_VIDE: fprintf(resFile, "ERREUR : pile vide\n\n\n\n"); break;
		case ERREUR_ADRESSAGE: fprintf(resFile, "ERREUR : erreur d'adressage\n\n\n\n"); break;
		case ERREUR_DIVISION_PAR_ZERO: fprintf(resFile, "ERREUR : division par 0\n\n\n\n"); break;
		}
	}

	fprintf(resFile, "le pourcentage de terrain où le robot est sorti est de  : %f %% ( soit %f terrains)\n\n\n\n", nombre_sorti * 100/N, (nombre_sorti));
	fprintf(resFile, "le pourcentage de terrain où le robot est resté bloqué est de : %f %% (soit %f terrains)\n\n\n\n", nombre_bloque * 100/N, (nombre_bloque));
	fprintf(resFile, "le pourcentage de terrain où le robot s'est crashé est de : %f %% (soit %f terrains)\n\n\n\n", nombre_crash * 100/N, (nombre_crash));
	fclose(resFile);
}
