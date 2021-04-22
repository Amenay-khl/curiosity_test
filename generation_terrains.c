#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generation_terrains.h"
#include "terrain.h"
//Fichier de Amenay et Yohan pour le projet curiosity en INF304
void generation_aleatoire(Terrain* t, int l, int h, float dObst)
    {
	int randomint;
	int i, j;
	t->largeur = l;
	t->hauteur = h;
	for(j = 0; j < h; j++)
        {     // on parcours la carte
		for(i = 0; i < l; i++)
            {
			randomint = rand() % 100;   //on calcule une densité en % pour chaque case de la carte
			if (randomint < (dObst*100))
                {     //si ce resultat est inferieur a la densité en entrée, alors on met un obstacle (rocher ou eau)
				if (rand()%2)
                    {
					t->tab[i][j] = ROCHER;
                    }else
                        {
                        t->tab[i][j] = EAU;
                        }
                }else
                    {
                    t->tab[i][j] = LIBRE;  //sinon la case est libre
                    }
            }
        }
	t->tab[(l/2)][(h/2)] = LIBRE;    // on oublie pas de mettre la case du milieu du terrain libre pour le robot
    }

//fonction empruntée a Gallay Florian et Guillaume Elian.
int chemin(Terrain T){

  int l = largeur(T);
  int h = hauteur(T);
  int trouve = 0;
  int i;
  // tableau de marque
  // initialisation
  //   etat(x,y) = 0 <=> la case est libre et non marquee
  //   etat(x,y) = 3 <=> la case est occupee (eau/roche)
  // boucle
  //   etat(x,y) = 0 <=> la case est libre et non marquee
  //  etat(x,y) = 1 <=> la case est libre, marquee et non traitee
  //   etat(x,y) = 2 <=> la case est libre, marquee et traitee
  //   etat(x,y) = 3 <=> la case est occupee

  //
  int etat[DIM_MAX][DIM_MAX];
  int x,y,x1,y1;
  int existe_case_1;

  // initialiser le tableau etat
  for (y=0; y<h;y++){
    for (x=0; x<l;x++){
      if (est_case_libre(T,x,y)){
        etat[x][y] = 0;
      }
      else{
        etat[x][y] = 3;
      }
    }
  }
  // marquer la seule case centrale
  x = l/2;
  y = h/2;
  etat[x][y] = 1;

  // boucle de recherche du chemin : trouver un ensemble connexe
  // de cases (marquées 1 ou 2) contenant la case centrale et
  // une case de bord
  existe_case_1 = 1;
  while(existe_case_1 && !trouve){
    // rechercher une case libre, marquee et non traitee
    existe_case_1 = 0;
    x = 0;
    y = 0;
    while(y<h && !existe_case_1){
      if(etat[x][y] == 1){
        // la case (x,y) est marquée 1
        existe_case_1 = 1;
      }
      else{
        // passer à la case suivante
        x++;
        if(x>=l){
          x = 1;
          y++;
        }
      }
    }

    if(existe_case_1){
      // marquer la case (x,y) comme marquee et traitee
      etat[x][y] = 2;

      // rechercher les cases voisines de (x,y) non marquees (0)
      // et pour chaque case voisine (x1,y1) vide et non marquee,
      // si c'est une case de bord,
      //   mettre le booleen chemin_trouve a VRAI
      // sinon
      //  la marquer (1,x,y) et l'empiler
      for (i=0; i<4;i++){
        switch(i){
        case 0:
          x1 = x;
          y1 = y+1;
          break;
        case 1:
          x1 = x;
          y1 = y-1;
          break;
        case 2:
          x1 = x+1;
          y1 = y;
          break;
        case 3:
          x1 = x-1;
          y1 = y;
          break;
        default:
          break;
        }
        if(x1 < l && y1 < h && etat[x1][y1] == 0){
          etat[x1][y1] = 1;
          if(x1==0 || x1==l-1 || y1==0 || y1==h-1){
            trouve = 1;
          }
        }
      }
    }
  }
  return trouve;
}
