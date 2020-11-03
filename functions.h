#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "variables.h"

/*Analyse les évenements SDL (Bloc SDL)*/
int hex_line(int hexagon); //Récupération de la ligne à laquelle apprtien l'hexagon/case entré en argument
extern point firsthex;
point hex_center(int hexagon);      //Récupération des cordonnées (x,y) de center d'un hexagon/case
float aucarre(float x);             //Fonction calculant de la carée d'un nombre
int hex_clicked(point click);       //Récupération de l'hexagon/case cliqué par l'utilsiateur

SDL_Rect pos_finder(int hexagon);   //Récupération de la position (x,y) (suivant les règles SDL)
                                    //  où on mettra le jeton/hexagon

int hex_finder(SDL_Event event);    //Récupération de l'hexagon/case cliqué par l'utilsiateur

/* Gestion de l'audio de bruitage (Sound Effects) */
void buttonsound(Mix_Chunk *buttonclicksound); // Activation de bruitage (Sound Effects)

/* Vérification du gagnant de la pertie (Bloc CHECK) */
extern int Left,Right; //Left = extrémité gauche, Right= extrémité droite, WINNER1 paramètre indicant si Le joueur 1 est gagant ou non
extern int Top,Down;   //Top = extrémité haute, Down= extrémité basse, WINNER2 paramètre indicant si Le joueur 2 est gagant ou non

int* voisin(int i);                          //Fonction permettant de retourner les voisins d'un hexagon/case
int check_edgeline(int hexagon,int Line[9]); // Vérification si un hexagon/case appartient à une Ligne/Colonne (TRUE si oui, FALSE sinon)

extern int destroyed_P1[41];        //Les cases déja vérifiées/visitées pour le joueur 1
extern int destroyed_P2[41];        //Les cases déja vérifiées/visitées pour le joueur 2
int check_destroyed(int hexagon,int destroyed[41]); // Vérifier si une case est déjà visité ou non
void rollback_destroyed(int destroyed[41]); //Initialisation de tableau contenant les cases déja visitées/vérifiées

void checkwin(int hexagon,int Firstline[9], int Lastline[9], int Destroyed[41], int* FIRST, int* LAST,int p); // Fonction de vérification du gagnant
void check_initialiser(int* Left, int* Right, int* Top, int*Down, int destroyed_P1[41], int destroyed_P2[41]); // Initialiser les paramètre de vérification
                                                                                                                //   (au cas où on lance une nouvelle partie)



/* Interfaces permettant de naviguer sur le jeu (Bloc UserInterface) */
extern int choice; // Choix de l'utilisateur dans les diffirents menus proposés

int homepage_choice(SDL_Window* Window);    //Page d'Acceuil
void rules(SDL_Window* Window);             //Page des Règles
void options(SDL_Window* Window);           //Page des Options
int gamemode_choice(SDL_Window* Window);    //Page des Modes de jeu
int difficulty_choice(SDL_Window* Window);  //Page de Difficulté

int isreturntohome(int *backfromgame, SDL_Event event); /* Fonction permettant de retourner au page
                                                        d'acceuil si on clique sur Esc, il permet aussi
                                                        de redémarrer la partie avec le boutton (REPLAY)*/


/*Initialisation du plateau de jeu (tableau stockant l'état actuel de jeu )*/
void rollback_databases(int GRID[81]);

/* Papier-pierre-ciseaux (pour choisir aléatoirement le premier à jouer) */
int rockpaperscissors(int difficulty,SDL_Window* Window);
void cpuchoosing(SDL_Window* Window);

/*Fonction assurant le fonctionnement de jeu ( Mise des jetons, gestions des tours .. (Bloc GAMEPLAY) */
void showgrid(SDL_Window* Window); //affichage de plateau de jeu

void database_initialiser(int *Blackplaying, int* p1,int* p2, int GRID[81]); // Initialisation des paramètre de gestions des tours
extern int Blackplaying; // Variable indiquant à qui st le tour de jouer
extern int p1,p2; // p1 = nombre de tours jouée par Joueur 1, p2 = nombre de tours jouée par Joueur 2

void joueur_humain(int hexagon, int *p,int turn, int *Blackplaying,int ennemy, int GRID[81], SDL_Window* Window); // Mise de jeton pour joueur HOMME
int joueur_ordinateur(int playerhex,int *p,int turn,int difficulty, int *Blackplaying,int GRID[81], SDL_Window* Window); // Mise de jeton pour joueur MACHINE

int easyCPU_turn(int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window);                 // Difficulté = Facile
int mediumCPU_turn(int playerhex,int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window); // Difficulté = Moyen
int hardCPU_turn(int playerhex,int *p,int turn, int *Blackplaying,int GRID[81], SDL_Window* Window);    // Difficulté = Difficile

void congratulations_P1(int *backfromgame,SDL_Window* Window); // On félicite le Joueur 1 HOMME si il gagne
void congratulations_P2(int *backfromgame,SDL_Window* Window); // On félicite le Joueur 2 HOMME si il gagne
void CPU_Wins(int *backfromgame,SDL_Window* Window);           // On affiche un message informant le joueur de sa perte contre le CPU

/*Intelligence Artificielle */

// Vérifie si deux hexagons/cases sont liées
int areHexagonsLinked(int hexagon1, int hexagon2);

//Position de l'hexagon de CPU près duquel l'ennemie vient juste de placer son jeton.
int hardCPU_relatedMove(int playerhex, int GRID[81], int turn);

// Meilleur position si l'ennemie place son jeton près d'un hexagon déja placé par CPU
int BestMoveifLinked(int voisin_indice, int relatedMove, int *ForcedLastmoveBackup);

// Au cas d'echec de stratégie, on joue de manière aléatoire intelligente = placer le jeton près de celui d'adversaire
int SMARTrandomize(int GRID[81], int hexagon);

// Stratégie de proximitymove
int proximitymove(int relatedMove, int playerhex, int *ForcedLastMoveBackup, int* CPUvoisins);

// Création d'un ensemble des Two-Bridges vers l'extrémité droite
int rightbridgemaking(int hexagon, int playerhex, int *rightbridge);

//Création d'un ensemble des Two-Bridges vers l'extrémité gauche
int leftbridgemaking(int playerhex, int *Lastmovebackup, int* ForcedLastmoveBackup, int* leftbridge);

// Connecter les diffirentes  Two-Bridges crées
void connectingthebridge(int playerhex, int GRID[81], int* BestMove);

// Le plateau entier stocké sous formes de colonnes Exemple : fill_Lines[0] = extrémité gauche & fill_Lines[8] = extrémité droite
void fill_Lines(int Lines[9][9]);

// Récupération de coté où l'humain a joué (Droit ou Gauche)
int sidechecker(int playerhex);

// Initialisation des paramètre de l'intelligence artificielle
void AI_initialiser(int *rightbridge,int *ForcedLastmoveBackup,int *leftbridge);







#endif // FUNCTIONS_H_INCLUDED
