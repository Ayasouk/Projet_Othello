#ifndef __PILE_OTHELLO_H__
#define __PILE_OTHELLO_H__

#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

#include "othello.h"

/* pile implémenté comme une liste chainée */
typedef struct MAILLON { othello val; struct MAILLON *next, *prev;} maillon;
maillon *top;

void init_pile();
void empiler(othello);
int depiler(othello*);

#endif
