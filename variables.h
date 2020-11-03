#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define TRUE 1
#define FALSE 0

/* Structure de point */
struct point{
    float x;
    float y;
};

typedef struct point point;

extern float d1; /* distance horizentale entre les centres de deux hexagonaux */
extern float d2; /*distance verticale entre les centres de deux hexagonaux */

/*La table de hex*/
extern int GRID[81];


/* La ligne de départ et d'arrivé pour le joueur 1*/
extern int Firstline_P1[9];
extern int Lastline_P1[9];

/* La ligne de départ et d'arrivé pour le joueur 2*/
extern int Firstline_P2[9];
extern int Lastline_P2[9];

/* Variable utilisable en intelligence artificielle */
extern int rightbridge;
extern int leftbridge;
extern int ForcedLastmoveBackup;


/* Chargement des images */
SDL_Surface* grid;
SDL_Surface* white_hex;
SDL_Surface* black_hex;
SDL_Surface* player1_turn;
SDL_Surface* player2_turn;
SDL_Surface* robot1_turn;
SDL_Surface* robot2_turn;

/* Chargement des fichier audio : musique " */
Mix_Music *navigationmusic;
Mix_Music *gameplaymusic;

/* Chargement des fichiers audio : bruitage */
Mix_Chunk *buttonclicksound;
Mix_Chunk *player1clicksound;
Mix_Chunk *player2clicksound;
Mix_Chunk *congratulationssound;
Mix_Chunk *CPUWinssound;

/*position de l'arriere plan et les buttons */
extern SDL_Rect position;
extern SDL_Rect buttonposition;
extern SDL_Rect buttonposition2;
extern SDL_Rect humanchoice;
extern SDL_Rect cpuchoice;
extern SDL_Rect message;
extern SDL_Rect playbuttonlocation;
extern SDL_Rect playerturn;




#endif // VARIABLES_H_INCLUDED
