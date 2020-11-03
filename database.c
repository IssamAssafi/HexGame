#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include "variables.h"

/* Table de hex*/
int GRID[81];

/* Initialisation de plateau de jeu */
void rollback_databases(int GRID[81])
{
    int v;
    for (v=0; v<81; v++)
    {
        GRID[v]=-1;
    }

}

/* Initialisation ou remise à zéro de plusieurs variables afin d'assure une nouvelle partie */
void database_initialiser(int *Blackplaying, int* p1,int* p2, int GRID[81])
{
    *Blackplaying=TRUE;
    *p1=0;*p2=0;
    rollback_databases(GRID);
}
