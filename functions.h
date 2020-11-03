#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "variables.h"

/*Analyse les �venements SDL (Bloc SDL)*/
int hex_line(int hexagon); //R�cup�ration de la ligne � laquelle apprtien l'hexagon/case entr� en argument
extern point firsthex;
point hex_center(int hexagon);      //R�cup�ration des cordonn�es (x,y) de center d'un hexagon/case
float aucarre(float x);             //Fonction calculant de la car�e d'un nombre
int hex_clicked(point click);       //R�cup�ration de l'hexagon/case cliqu� par l'utilsiateur

SDL_Rect pos_finder(int hexagon);   //R�cup�ration de la position (x,y) (suivant les r�gles SDL)
                                    //  o� on mettra le jeton/hexagon

int hex_finder(SDL_Event event);    //R�cup�ration de l'hexagon/case cliqu� par l'utilsiateur

/* Gestion de l'audio de bruitage (Sound Effects) */
void buttonsound(Mix_Chunk *buttonclicksound); // Activation de bruitage (Sound Effects)

/* V�rification du gagnant de la pertie (Bloc CHECK) */
extern int Left,Right; //Left = extr�mit� gauche, Right= extr�mit� droite, WINNER1 param�tre indicant si Le joueur 1 est gagant ou non
extern int Top,Down;   //Top = extr�mit� haute, Down= extr�mit� basse, WINNER2 param�tre indicant si Le joueur 2 est gagant ou non

int* voisin(int i);                          //Fonction permettant de retourner les voisins d'un hexagon/case
int check_edgeline(int hexagon,int Line[9]); // V�rification si un hexagon/case appartient � une Ligne/Colonne (TRUE si oui, FALSE sinon)

extern int destroyed_P1[41];        //Les cases d�ja v�rifi�es/visit�es pour le joueur 1
extern int destroyed_P2[41];        //Les cases d�ja v�rifi�es/visit�es pour le joueur 2
int check_destroyed(int hexagon,int destroyed[41]); // V�rifier si une case est d�j� visit� ou non
void rollback_destroyed(int destroyed[41]); //Initialisation de tableau contenant les cases d�ja visit�es/v�rifi�es

void checkwin(int hexagon,int Firstline[9], int Lastline[9], int Destroyed[41], int* FIRST, int* LAST,int p); // Fonction de v�rification du gagnant
void check_initialiser(int* Left, int* Right, int* Top, int*Down, int destroyed_P1[41], int destroyed_P2[41]); // Initialiser les param�tre de v�rification
                                                                                                                //   (au cas o� on lance une nouvelle partie)



/* Interfaces permettant de naviguer sur le jeu (Bloc UserInterface) */
extern int choice; // Choix de l'utilisateur dans les diffirents menus propos�s

int homepage_choice(SDL_Window* Window);    //Page d'Acceuil
void rules(SDL_Window* Window);             //Page des R�gles
void options(SDL_Window* Window);           //Page des Options
int gamemode_choice(SDL_Window* Window);    //Page des Modes de jeu
int difficulty_choice(SDL_Window* Window);  //Page de Difficult�

int isreturntohome(int *backfromgame, SDL_Event event); /* Fonction permettant de retourner au page
                                                        d'acceuil si on clique sur Esc, il permet aussi
                                                        de red�marrer la partie avec le boutton (REPLAY)*/


/*Initialisation du plateau de jeu (tableau stockant l'�tat actuel de jeu )*/
void rollback_databases(int GRID[81]);

/* Papier-pierre-ciseaux (pour choisir al�atoirement le premier � jouer) */
int rockpaperscissors(int difficulty,SDL_Window* Window);
void cpuchoosing(SDL_Window* Window);

/*Fonction assurant le fonctionnement de jeu ( Mise des jetons, gestions des tours .. (Bloc GAMEPLAY) */
void showgrid(SDL_Window* Window); //affichage de plateau de jeu

void database_initialiser(int *Blackplaying, int* p1,int* p2, int GRID[81]); // Initialisation des param�tre de gestions des tours
extern int Blackplaying; // Variable indiquant � qui st le tour de jouer
extern int p1,p2; // p1 = nombre de tours jou�e par Joueur 1, p2 = nombre de tours jou�e par Joueur 2

void joueur_humain(int hexagon, int *p,int turn, int *Blackplaying,int ennemy, int GRID[81], SDL_Window* Window); // Mise de jeton pour joueur HOMME
int joueur_ordinateur(int playerhex,int *p,int turn,int difficulty, int *Blackplaying,int GRID[81], SDL_Window* Window); // Mise de jeton pour joueur MACHINE

int easyCPU_turn(int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window);                 // Difficult� = Facile
int mediumCPU_turn(int playerhex,int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window); // Difficult� = Moyen
int hardCPU_turn(int playerhex,int *p,int turn, int *Blackplaying,int GRID[81], SDL_Window* Window);    // Difficult� = Difficile

void congratulations_P1(int *backfromgame,SDL_Window* Window); // On f�licite le Joueur 1 HOMME si il gagne
void congratulations_P2(int *backfromgame,SDL_Window* Window); // On f�licite le Joueur 2 HOMME si il gagne
void CPU_Wins(int *backfromgame,SDL_Window* Window);           // On affiche un message informant le joueur de sa perte contre le CPU

/*Intelligence Artificielle */

// V�rifie si deux hexagons/cases sont li�es
int areHexagonsLinked(int hexagon1, int hexagon2);

//Position de l'hexagon de CPU pr�s duquel l'ennemie vient juste de placer son jeton.
int hardCPU_relatedMove(int playerhex, int GRID[81], int turn);

// Meilleur position si l'ennemie place son jeton pr�s d'un hexagon d�ja plac� par CPU
int BestMoveifLinked(int voisin_indice, int relatedMove, int *ForcedLastmoveBackup);

// Au cas d'echec de strat�gie, on joue de mani�re al�atoire intelligente = placer le jeton pr�s de celui d'adversaire
int SMARTrandomize(int GRID[81], int hexagon);

// Strat�gie de proximitymove
int proximitymove(int relatedMove, int playerhex, int *ForcedLastMoveBackup, int* CPUvoisins);

// Cr�ation d'un ensemble des Two-Bridges vers l'extr�mit� droite
int rightbridgemaking(int hexagon, int playerhex, int *rightbridge);

//Cr�ation d'un ensemble des Two-Bridges vers l'extr�mit� gauche
int leftbridgemaking(int playerhex, int *Lastmovebackup, int* ForcedLastmoveBackup, int* leftbridge);

// Connecter les diffirentes  Two-Bridges cr�es
void connectingthebridge(int playerhex, int GRID[81], int* BestMove);

// Le plateau entier stock� sous formes de colonnes Exemple : fill_Lines[0] = extr�mit� gauche & fill_Lines[8] = extr�mit� droite
void fill_Lines(int Lines[9][9]);

// R�cup�ration de cot� o� l'humain a jou� (Droit ou Gauche)
int sidechecker(int playerhex);

// Initialisation des param�tre de l'intelligence artificielle
void AI_initialiser(int *rightbridge,int *ForcedLastmoveBackup,int *leftbridge);







#endif // FUNCTIONS_H_INCLUDED
