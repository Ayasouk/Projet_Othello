#ifndef __OTHELLO_H__
#define __OTHELLO_H__

#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

#define TAILLE 8

typedef struct { char data[TAILLE][TAILLE];} othello;

othello* creer_othello();
void afficher_othello(othello*);
int coord_valides(int, int);
int case_libre(othello*, int, int);
void jouer_un_coup(othello*, char, int, int);
int humain_joue_un_coup(othello*, char);
int coup_possible(othello*, char, int, int);
int peut_jouer(othello*, char);
int score(othello*, char, int, int);
int min_max(othello*, char, char, int, int*, int*, int, int);

#endif
