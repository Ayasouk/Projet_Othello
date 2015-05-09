


#include "othello.h"
#include <time.h>
#include "pile.h"

#define J1 'X'
#define J2 'O'
#define VIDE ' '

/* adversaire permet de basculer de J1 à J2 et vice versa */
char adversaire(char joueur){
    return joueur==J1?J2:J1;
}

/* Fonction d'initialisation de l'othello */
othello* creer_othello(){
    int i, j;
    othello* oth = (othello*) malloc(sizeof(othello));
    
    for(i=0;i<TAILLE;i++)
        for(j=0;j<TAILLE;j++)
            oth->data[i][j] = VIDE;
    
    oth->data[3][3] = J1;
    oth->data[4][4] = J1;
    oth->data[3][4] = J2;
    oth->data[4][3] = J2;
    
    return oth;
}

/* Fonction d'affichage de l'othello */
void afficher_othello(othello* oth){
    int i, j;
    
    printf("    | ");
    for(i=0; i<TAILLE; i++)
        printf("%d | ", i);
    printf("\n");
    printf("______");
    for(i=0; i<TAILLE; i++)
        printf("____");
    printf("\n");

    for(i=0; i<TAILLE; i++){
        printf("| %d | ", i);
        for(j=0; j<TAILLE; j++){
            if(oth->data[i][j]==J1){
				printf("%c | ", oth->data[i][j]);
			}
			else printf("%c | ", oth->data[i][j]);
        }
        printf("\n");
    }
    printf("______");
    for(i=0; i<TAILLE; i++)
        printf("____");
    printf("\n");

}

/* Fonction vérifiant si les coordonnées sont sur le plateau */
int coord_valides(int i, int j){
    return (i>=0&&i<TAILLE && j>=0&&j<TAILLE);
}

/* Fonction permettant de savoir si je joueur 'joueur' peut jouer dans la case numl,numc */
int coup_possible(othello *jeu, char joueur, int numl, int numc){
    int i, j;
    int di,dj,nb;
    
    if(!case_libre(jeu,numl,numc)){
        //fprintf(stderr, "coup_possible: case (%d,%d) pas libre ...\n", numl, numc);
        return 0;
    }
    for(di=-1;di<2;di++){
        for(dj=-1;dj<2;dj++){
            if(di!=0 || dj!=0){
                nb=0;
                i=numl+di; j=numc+dj;
                while(coord_valides(i,j) && jeu->data[i][j]==adversaire(joueur)){
                    i+=di;
                    j+=dj;
                    nb++;
                }
                if(coord_valides(i,j) && jeu->data[i][j]==joueur && nb>0){
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Détermine si le joueur 'joueur' peut jouer (renvoie vrai) ou non (renvoie faux) */
int peut_jouer(othello *jeu, char joueur){
    int i,j;
    for(i=0;i<TAILLE;i++)
        for(j=0;j<TAILLE;j++)
            if(coup_possible(jeu, joueur, i,j))
                return 1;
    return 0;
}


/* joue le coup en (i,j) 
    aucune vérification si le coup est valide n'est effectuée ici
 */
void jouer_un_coup(othello *jeu, char joueur, int numl, int numc){
    int i, j, di, dj, nb;
    jeu->data[numl][numc]=joueur;
    for(di=-1;di<2;di++){
        for(dj=-1;dj<2;dj++){
            if(di!=0 || dj!=0){
                nb=0;
                i=numl+di; j=numc+dj;
                while(coord_valides(i,j) && jeu->data[i][j]==adversaire(joueur)){
                    i+=di;
                    j+=dj;
                    nb++;
                }
                if(coord_valides(i,j) && jeu->data[i][j]==joueur && nb>0){
                    // on retourne les pions dans cette direction
                    i=numl+di; j=numc+dj;
                    while(coord_valides(i,j) && jeu->data[i][j]==adversaire(joueur)){
                        jeu->data[i][j] = joueur;
                        i+=di;
                        j+=dj;
                    }
                }
            }
        }
    }
}

/* Fonction permettant de savoir si la partie est finie
    - si on a joué tous les coups
    - si les 2 joueurs ont passé tour à tour
 */
int partie_finie(int nb_coups, int passe){

    int res = 0;
    if(nb_coups == TAILLE*TAILLE-4)
        res = 1;
    if(passe == 2)
        res = 1;
    return res;
}

/* Fonction renvoyant vrai si la case est valide et vide, faux sinon */
int case_libre(othello *jeu, int i, int j){
    return (coord_valides(i,j) && (jeu->data[i][j] == VIDE));
}

/* Fonction permettant de faire jouer un joueur humain
    - saisie des coordonnées tant qu'elles sont invalides
    - jouer le coup
 */
int humain_joue_un_coup(othello* jeu, char joueur){
    int i, j, ok=0;
    
    if(!peut_jouer(jeu, joueur)){
        return 0;
    }
    
    do{
        printf("Joueur J%d [%c]:\n", joueur==J1?1:2, joueur);
        printf("ligne: ");
        scanf("%d", &i);
        printf("colonne: ");
        scanf("%d", &j);
        if(!case_libre(jeu, i, j))
            printf("La case est occupée, recommencez ...\n");
        else if(!(ok=coup_possible(jeu, joueur, i, j)))
            printf("Vous ne pouvez pas jouer ici, recommencez ...\n");
        
    } while(!ok);
    jouer_un_coup(jeu, joueur, i, j);
    return 1;
}



/* Fonction min-max permettant de déterminer ou jouer
    - le meilleur coup est stocké dans *ti et *tj
    - joueur est le joueur devant joue le meilleur coup
    - joueur_actif est le joueur jouant le tour actuel
    - depth = profondeur actuelle dans l'arbre de recherche
    - max-depth = profondeur maximale, la récursion s'arrête si on l'atteind
 */
int min_max(othello* jeu, char joueur, char joueur_actif, int nb_coups, int*ti, int*tj, int depth, int max_depth){
    
    int meilleur_score =-1;
    int cur_score;
    int i, j;
	
	typedef struct coord_meilleur_coup{ int ligne; int colonne;} coord_MC; //structure des coordonnées d'un meilleur coup
	int compteur_mc= 0; //compteur de meilleur coup 
	coord_MC tab[TAILLE*TAILLE]; /* tableau stockant chaque meilleur coup possible */
	
    /* Si la partie est finie ou si on a atteind la profondeur maximale -> on renvoie le score du plateau */
	if(partie_finie(nb_coups, 0)||depth == max_depth){
		meilleur_score = score(jeu, joueur_actif, depth, nb_coups);
	}
	else {
		//on parcours toutes les positions
		for(i=0; i<TAILLE; i++){
			for(j=0; j<TAILLE; j++){
				//si on a un coup possible on renvoi le meilleur coup a jouer
				if(coup_possible(jeu, joueur_actif, i, j)){
					empiler(*jeu);
					jouer_un_coup(jeu, joueur_actif, i, j);
					cur_score = min_max(jeu, joueur, adversaire(joueur_actif), nb_coups+1, ti, tj, depth+1, max_depth);
					depiler(jeu);
					
					if(joueur_actif == joueur){
						meilleur_score = -1000;
						if(cur_score > meilleur_score){
							meilleur_score = cur_score;
							*ti = i;
							*tj = j;
							tab[compteur_mc].ligne = i;
							tab[compteur_mc].colonne = j;
						}
						compteur_mc++;
					}
					
					if(joueur_actif == adversaire(joueur)){
						meilleur_score = 1000;
						if(cur_score < meilleur_score){
								meilleur_score = cur_score;
								*ti = i;
								*tj = j;
								tab[compteur_mc].ligne = i;
								tab[compteur_mc].colonne = j;
						}
						compteur_mc++;
					}
				} //endif coup_possible()			
			}
		}
		if(compteur_mc > 0){
			int al = rand()%compteur_mc;
			*ti = tab[al].ligne;
			*tj = tab[al].colonne;
		}
		else{
			*ti =-1;
			*tj =-1;
		}
	}
    /*  pour tous les coups possibles
            sauvegarder le jeu (empiler)
            jouer ce coup
            score = appel récursif de min_max avec les paramètres mis à jour
            restaurer le jeu (dépiler)
            
            Si c'est à nous de jouer -> conserver le score max et le coup correspondant
            Si c'est à l'adversaire de jouer -> conserver le score min et le coup correspondant
     
        S'il y a au moins 1 meilleur coup possible
            mettre le coup correspondant dans ti et tj
            Amélioration : choisir aléatoirement un coup parmis les meilleurs possibles
        Sinon
            mettre -1,-1 dans ti et tj
        renvoyer le score
     
     */
    
    
    return meilleur_score;
}

/* Fonction d'évaluation
    - renvoie le score pour le joueur 'joueur'
    - TODO: améliorer la fonction
 */
int score(othello* jeu, char joueur, int depth, int nb_coups){
    int i,j,score=0;
    
    /* chaque case vaut 1 si la partie est fini */
    if(partie_finie(nb_coups, 0)){
		for(i=0;i<TAILLE;i++){
			for(j=0;j<TAILLE;j++){
				if(jeu->data[i][j] == joueur)
					score++;
			}
		}
	}
	else{// si la partie est en cours chaque case a une particularité celon sa position pour le joueur
		//initialisation d'un tableau representant les points attribué à la place pertinente du pion du joueur dans l'othellier
		int evalPlace[8][8] = { {500 ,-150,30,10,10,30,-150,500 }, 
							{-150,-250,0 ,0 ,0 ,0 ,-250,-150},
							{30  ,0   ,1 ,2 ,2 ,1 ,0   ,30  },
							{10  ,0   ,2 ,16,16,2 ,0   ,10  },
							{10  ,0   ,2 ,16,16,2 ,0   ,10  },
							{30  ,0   ,1 ,2 ,2 ,1 ,0   ,30  },
							{-150,-250,0 ,0 ,0 ,0 ,-250,-150},
							{500 ,-150,30,10,10,30,-150,500 }
						};			
							
		for(i=0; i<TAILLE; i++){
			for(j=0; j<TAILLE; j++){
				if(jeu->data[i][j] == joueur)
					score +=evalPlace[i][j]; 
			}
		}
	}
    return score;
}

/*
 Fonction principale
 */
int main(int argc, char **argv)
{
    int mode, rep;
    int nb_coups = 0, i, j, passe=0;
    char joueur;
    char gagnant = '-';


	//TODO: changer la force de l'ordi en diminuant/augmentant la max depth j1 et/ou j2
    int max_depth_j1 = 5;
    int max_depth_j2 = 1;
    
    srand(time(NULL));
    init_pile();
    
    printf("Bienvenue dans SPIOTHELLO, l'othello des L2 SPI de l'Université du Maine !\n");

    /* mode de jeu */
    do{
        printf("Choisissez le mode de jeu:\n");
        printf("\t1. J1 [Humain] vs. J2 [Humain]\n");
        printf("\t2. J1 [Humain] vs. J2 [Ordinateur]\n");
        printf("\t3. J1 [Ordinateur] vs. J2 [Ordinateur]\n");
        printf("Choix : ");
        scanf("%d",&mode);
        if(mode!=1 && mode!=2 && mode!=3)
            printf("Le mode %d n'existe pas, saisissez à nouveau.", mode);
    } while(mode!=1 && mode!=2 && mode!=3);
    
    /* demander qui commence */
    do {
        printf("Qui commence ? J1 [%c] -> 1 ou J2 [%c] -> 2\nChoix : ", J1, J2);
        scanf("%d", &rep);
        if(rep != 1 && rep != 2)
            printf("Saisie incorrecte, recomencez");
    } while(rep!=1 && rep!=2);
    if(rep==1) joueur = J1;
    else joueur = J2;
    printf("Le joueur J%d [%c] commence ...\n", rep, joueur);
    
    othello *jeu = creer_othello();
    afficher_othello(jeu);
    
    while(!partie_finie(nb_coups, passe)){
        if(mode == 1 || (mode==2 && joueur==J1)){            // Si humain-humain ou humain-ordi et joueur courant == J1
            //demander une case
            if(humain_joue_un_coup(jeu, joueur)) {
                nb_coups++;
                passe = 0;
            }
            else {
                printf("J%d [%c], vous ne pouvez pas jouer ... !\n", joueur==J1?1:2, joueur);
                scanf(" ");
                passe++;
            }
        } else {    // si humain-ordi et joueur courant == J2 ou ordi-ordi
            
            //déterminer la case à jouer
            fprintf(stderr, "C'est à J%d [%c] de jouer !\n", joueur==J1?1:2, joueur);
            min_max(jeu, joueur, joueur, nb_coups, &i, &j, 0, joueur==J1?max_depth_j1:max_depth_j2);
            // on joue le meilleur coup s'il existe
            if(i==-1 && j==-1){
                printf("Aucun coup possible ... je passe mon tour\n");
                passe++;
            } else {
                jouer_un_coup(jeu, joueur, i, j);
                nb_coups++;
                passe = 0;
            }
        }
        afficher_othello(jeu);
        joueur = adversaire(joueur); // on change de joueur
    }
    
    int sc1 = score(jeu, J1, 0, nb_coups);
    int sc2 = score(jeu, J2, 0, nb_coups);
    gagnant = sc1>sc2?J1:(sc1<sc2?J2:'-');
    
    if(gagnant == J1)
        printf("Le joueur J1 [%c] a gagné !!\n", gagnant);
    else if(gagnant == J2)
        printf("Le joueur J2 [%c] a gagné !!\n", gagnant);
    else
        printf("Les joueurs n'ont pas pu se départager ... !!\n");
    
	return 0;
}

