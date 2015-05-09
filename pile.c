/* Mise en oeuvre par pointeurs d'un arbre binaire */

#include "pile.h"

/* Fonction d'initialisation */
void init_pile(){
    top=NULL;
}

/* Création d'un maillon */
maillon* creer_maillon(othello oth){
    maillon* m = (maillon*) malloc(sizeof(maillon));
    m->prev = NULL;
    m->next = NULL;
    m->val = oth;
    return m;
}

/* Fonction permettant d'empiler */
void empiler(othello oth){
    if(top == NULL){
        top = creer_maillon(oth);
    } else {
        maillon *newmaillon = creer_maillon(oth);
        top->next = newmaillon;
        newmaillon->prev = top;
        top = newmaillon;
    }
}


/* Fonction permettant de dépiler */
int depiler(othello *o){
    if(top != NULL){
        *o = top->val;
        maillon *todelete = top;
        top = top->prev;
        if(top != NULL){
            top->next = NULL;
        }
        free(todelete);
        return 1;
    }
    return 0;
}


