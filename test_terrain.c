#include "terrain.h"
#include <stdio.h>

int main(int argc, char ** argv)
    {
    Terrain t;
    int x, y;
    char *nom_fichier = NULL;
    erreur_terrain erreur;
    if(argc < 2)
        {
        printf("Usage : %s <fichier>\n", argv[0]);
        return 1;
        }

    erreur =lire_terrain(argv[1], &t, &x, &y);
    if(erreur == OK)
        {
        lire_terrain(argv[1], &t, &x, &y);
        }
    else
        {
        while(erreur!=OK)
            {
            printf("donner un nouveau nom de fichier\n");
            fscanf(stdin,"%s",nom_fichier);
            erreur =lire_terrain(nom_fichier, &t, &x, &y);
            }
        lire_terrain(nom_fichier, &t, &x, &y);
        }
    afficher_terrain(&t);
    printf("Position initiale du robot : (%d, %d)\n", x, y);
    }
