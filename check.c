#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "variables.h"
#include "functions.h"

/* La ligne de départ et d'arrivé pour le joueur 1*/
int Firstline_P1[9]={0,9,18,27,36,45,54,63,72};
int Lastline_P1[9]={8,17,26,35,44,53,62,71,80};

/* La ligne de départ et d'arrivé pour le joueur 2*/
int Firstline_P2[9]={0,1,2,3,4,5,6,7,8};
int Lastline_P2[9]={72,73,74,75,76,77,78,79,80};

int Left,Right;
int Top,Down;
int destroyed_P1[41];
int destroyed_P2[41];

int check_edgeline(int hexagon,int Line[9]) // vérifie si un pion "hexagon" apparatient à la colonne "Line"
{
    int count;
    for (count=0; count<9; count++)
    {
        if (hexagon==Line[count])
            return TRUE;
    }
    return FALSE;
}

int check_destroyed(int hexagon,int destroyed[41]) //vérifie si un pion est déjç visité
{
    int count=0;
    while (destroyed[count]!=-1)
    {
        if (hexagon==destroyed[count])
            return TRUE;
    count++;
    }
    return FALSE;
}

int* voisin(int i) // récupération des voisins d'une vertaine case
{

    int j;
    int *voisin;
    voisin=(int*)malloc(sizeof(int)*7);

    for(j=0; j<7; j++)
    {
        voisin[j]=-1;
    }

    if (i==0){voisin[0]=1;voisin[1]=9;}//Case 0
    else if (i==8){voisin[0]=7;voisin[1]=16;voisin[2]=17;} //case 8
    else if (i==72){voisin[0]=63;voisin[1]=64;voisin[2]=73;} // case 72
    else if (i==80){voisin[0]=71;voisin[1]=79;} // case 80
    else if (1<=i&&i<=7){voisin[0]=i-1;voisin[1]=i+1;voisin[2]=i+8;voisin[3]=i+9;} // de la case 1 à la case 7
    else if (73<=i&&i<=79){voisin[0]=i-9;voisin[1]=i-8;voisin[2]=i-1;voisin[3]=i+1;} // de la case 73 à la case 79
    else if ((i+1)%9==0){voisin[0]=i-9;voisin[1]=i-1;voisin[2]=i+8;voisin[3]=i+9;} // de la case 17 à la case 71
    else if (i%9==0){voisin[0]=i-9;voisin[1]=i-8;voisin[2]=i+1;voisin[3]=i+9;} // de la case 9 à la case 63
    else {voisin[0]=i-9;voisin[1]=i-8;voisin[2]=i-1;voisin[3]=i+1;voisin[4]=i+8;voisin[5]=i+9;}//toute case entouré par 6 cases

    return voisin;
}

void rollback_destroyed(int destroyed[41]) // initialisation des pions visités
{
    int count;
    for (count=0; count<41; count++)
        destroyed[count]=-1;
}

void checkwin(int hexagon, int Firstline[9], int Lastline[9], int Destroyed[41], int* FIRST, int *LAST,int p) // fonction de vérification du gagnant
{
    int *voisins;
    int count=0;
    voisins=voisin(hexagon);



    if ( check_edgeline(hexagon,Firstline)==TRUE&&(*FIRST==FALSE))
        *FIRST=TRUE;

    if ( check_edgeline(hexagon,Lastline)==TRUE&&(*LAST==FALSE) )
        *LAST=TRUE;

    //fprintf(stderr,"We Are in checkwin(%d,first=%d,last=%d)\n",hexagon,*FIRST,*LAST);


    //fprintf(stderr,"checkedge = %d,%d\n",check_edgeline(hexagon,Firstline),check_edgeline(hexagon,Lastline));

    while (Destroyed[count]!=-1)
        count++;
    Destroyed[count]=hexagon;
    //fprintf(stderr,"Destroyed[%d]=%d\n",count,Destroyed[count]);

    count=0;
    while(voisins[count]!=-1)
        {
            //fprintf(stderr,"we verify now voisins[%d]=%d    -   2eme check = %d\n",count,voisins[count],check_destroyed(voisins[3],Destroyed));
            if ((GRID[voisins[count]])==p&&check_destroyed(voisins[count],Destroyed)==FALSE)
                checkwin(voisins[count],Firstline,Lastline,Destroyed,FIRST,LAST,p);

        count++;
        }


}

void check_initialiser(int* Left, int* Right, int* Top, int*Down, int destroyed_P1[41], int destroyed_P2[41]) // initialisation de la vérification
{
    *Left=FALSE,*Right=FALSE;
    *Top=FALSE,*Down=FALSE;
    rollback_destroyed(destroyed_P1);
    rollback_destroyed(destroyed_P2);
}
