#include "message.h"
#include "includes.h"
void ncurses_stopper() ;
void ncurses_initialiser() ;

void ncurses_stopper();
void simulation_stopper() ;
WINDOW *creer_fenetre_box_sim();
WINDOW *creer_fenetre_sim();
WINDOW *creer_fenetre_box_msg() ;
WINDOW *creer_fenetre_msg() ;
WINDOW *creer_fenetre_box_outils();
WINDOW *creer_fenetre_outils() ;
void *routine_poisson(void *);
void simulation();
void init_etang(grille_t);
void afficher_etang(grille_t);
void both_send(grille_t, int, int);
void one_send(grille_t, int);