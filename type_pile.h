#ifndef _TYPE_PILE_H_
#define _TYPE_PILE_H_

#define TAILLE_MAX 10000

typedef struct {
  int n;  //nb d'elements dans la pile
  int tab[TAILLE_MAX];
} PileEntiers;


/* Créer une pile vide */
void creer_pile(PileEntiers * p);

/* Opérations d'accès */

/* Retourne vrai ssi p est vide */
int est_vide(PileEntiers p);

/* Renvoie l'entier en haut de la pile */
/* p doit être non vide */
int sommet(PileEntiers p);

/* Renvoie le nombre d'éléments dans la pile */
int hauteur_pile(PileEntiers p);

/* Afficher les éléments de la pile */
void afficher_pile(PileEntiers p);


/* Vider la pile p */
void vider(PileEntiers * p);

/* Empiler un entier */
void empiler(PileEntiers * p, int x);

/* Supprimer l'entier en haut de la pile */
void depiler(PileEntiers * p);

#endif
