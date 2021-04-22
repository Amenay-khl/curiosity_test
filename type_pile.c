#include <stdio.h>
#include "type_pile.h"


/* Créer une pile vide */
void creer_pile(PileEntiers * p)
    {
    p->n = 0;
    }

/* Opérations d'accès */

/* Retourne vrai si p est vide, faux sinon */
int est_vide(PileEntiers p)
    {
    return (p.n == 0);
    }


/* Renvoie l'entier en haut de la pile */
/* p doit être non vide */
int sommet(PileEntiers p)
    {
    return p.tab[p.n-1];
    }

/* Renvoie le nombre d'éléments dans la pile */
int hauteur_pile(PileEntiers p)
    {
    return p.n;
    }


/* Afficher la pile et ses elements  */
void afficher_pile(PileEntiers p)
    {
    int i;
    printf("[ ");
    for(i = p.n-1; i >= 0; i--)
        {
        printf("%d ", p.tab[i]);
        }
    printf("]");
    }


/* Vider la pile p */
void vider(PileEntiers * p)
    {
    p->n = 0;
    }

/* Empiler un entier  */
void empiler(PileEntiers * p, int x)
    {
    p->tab[p->n] = x;
    p->n = p->n + 1;
    }

/* Supprimer l'entier en haut de la pile (non vide) */
void depiler(PileEntiers * p)
    {
    p->n = p->n - 1;
    }

