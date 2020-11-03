#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include "variables.h"

int Blackplaying;
int p1,p2;
int rightbridge=FALSE;
int leftbridge=FALSE;
int ForcingActivated=FALSE;
int Lastmovebackup;
int ForcedLastmoveBackup;

int beforeLastline_P1[9]={7,16,25,34,43,52,61,70,79};
int afterFirstline_P1[9]={1,10,19,28,37,46,55,64,73};

int Lines[9][9];

void showgrid(SDL_Window* Window) // afficher le plateau de jeu
{
    /* Chargement des images */
    SDL_Surface* grid = IMG_Load("images/hex_grid.jpg");

    /* Affichage de la table vide de hex */
    SDL_BlitSurface(grid,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);
    return;
}


void joueur_humain(int hexagon, int *p,int turn, int *Blackplaying,int ennemy, int GRID[81], SDL_Window* Window) // Mise de pion de joueur HOMME
{

    SDL_Rect hex_position=pos_finder(hexagon);
    if (turn==1)
    {
        SDL_BlitSurface(black_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        if (ennemy==1) SDL_BlitSurface(player2_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        else SDL_BlitSurface(robot2_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        buttonsound(player1clicksound);
        SDL_UpdateWindowSurface(Window);
        GRID[hexagon]=1;
        *Blackplaying=FALSE;
    }
    else
    {
        SDL_BlitSurface(white_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        if (ennemy==1) SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        else SDL_BlitSurface(robot1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        buttonsound(player2clicksound);
        SDL_UpdateWindowSurface(Window);
        GRID[hexagon]=2;
        *Blackplaying=TRUE;
    }
    (*p)++;

}

/*Mise de pion de joueur MACHINE*/
int joueur_ordinateur(int playerhex,int *p,int turn,int difficulty, int *Blackplaying,int GRID[81], SDL_Window* Window)
{
    if (difficulty==1)
        return easyCPU_turn(&(*p),turn,&(*Blackplaying),GRID,Window);

    else if (difficulty==2)
        return mediumCPU_turn(playerhex,&(*p),turn,&(*Blackplaying),GRID,Window);

    else if (difficulty==3)
        return hardCPU_turn(playerhex,&(*p),turn,&(*Blackplaying),GRID,Window);
}


/* Mise de pion par la machine au cas de difficulté facile  */
int easyCPU_turn(int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window)
{

    int hexagon;
    hexagon=rand()%81;
    while (GRID[hexagon]!=-1)
           hexagon=rand()%81;

    SDL_Rect hex_position=pos_finder(hexagon);
    if (turn==1)
    {
        SDL_BlitSurface(black_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player2_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player1clicksound);
        GRID[hexagon]=1;
        *Blackplaying=FALSE;
    }
    else
    {
        SDL_BlitSurface(white_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player2clicksound);
        GRID[hexagon]=2;
        *Blackplaying=TRUE;
    }
    (*p)++;
    return hexagon;
}


/* Mise de pion par la machine au cas de difficulté moyenne  */
int mediumCPU_turn(int playerhex,int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window)
{

    int i,hexagon;
    int* voisins;
    voisins=voisin(playerhex); // Playerhex = pion joué par l'adversaire

    if (turn==2) // Machine est le joueur 2
    {

        if (voisins[3]!=-1&&GRID[voisins[3]]==-1) hexagon=voisins[3];
        else if (voisins[2]!=-1&&GRID[voisins[2]]==-1) hexagon=voisins[2];
        else
        {
            for (i=0; i<6; i++)
            if (voisins[i]!=-1&&GRID[voisins[i]]==-1) hexagon=voisins[i];
        }
    }
    else // Machine est le joueur 1
    {
        if (voisins[5]!=-1&&GRID[voisins[5]]==-1) hexagon=voisins[5];
        else if (voisins[0]!=-1&&GRID[voisins[0]]==-1) hexagon=voisins[0];
        else
        {
            for (i=0; i<6; i++)
            if (voisins[i]!=-1&&GRID[voisins[i]]==-1) hexagon=voisins[i];
        }
    }

    if (turn==1 && *p==0) hexagon=40; // Si la machine est le joueur 1, poser le premier pion sur le centre

    SDL_Rect hex_position=pos_finder(hexagon);
    if (turn==1)
    {
        SDL_BlitSurface(black_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player2_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player1clicksound);
        GRID[hexagon]=1;
        *Blackplaying=FALSE;
    }
    else
    {
        SDL_BlitSurface(white_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player2clicksound);
        GRID[hexagon]=2;
        *Blackplaying=TRUE;
    }
    (*p)++;
    return hexagon;
}



/* Mise de pion par la machine au cas de difficulté difficile  */
int hardCPU_turn(int playerhex,int *p,int turn, int *Blackplaying, int GRID[81], SDL_Window* Window)
{
    int BestMove; // Meilleure décision
    int* CPUvoisins;
    int relatedMove=hardCPU_relatedMove(playerhex,GRID,turn); //Pion de CPU lié au pion placé par l'humain
    CPUvoisins=voisin(relatedMove); // Ses voisins
    fill_Lines(Lines);   // en quelque sorte, c'est le plateau de jeu

    if (turn==2)
    {
        /* Stratégie pas encore disponible au cas où La machine joue en second tour */
    }
    else
    {
        if (*p==0) //Premiere tour
            {BestMove=40; Lastmovebackup=BestMove;}//Le centre est la meilleure position

        else if (areHexagonsLinked(relatedMove,playerhex)!=-1) //L'adversaire place un pion près de celui de la machine
            BestMove=proximitymove(relatedMove,playerhex,ForcedLastmoveBackup,CPUvoisins); // stratégie de ProximityMove

        else if ((sidechecker(playerhex)==0 || rightbridge==TRUE) && leftbridge==FALSE) //L'adversaire joue sur le côté gauche
            //Stratégie de two-bridge dans vers l'extrémité gauche
            BestMove=leftbridgemaking(playerhex,&Lastmovebackup,&ForcedLastmoveBackup,&leftbridge);

        else if ((sidechecker(playerhex)==1||leftbridge==TRUE) && rightbridge==FALSE) //L'adversaire joue sur le côté droit
             //Stratégie de two-bridge dans vers l'extrémité droite
            {BestMove=rightbridgemaking(40,playerhex,&rightbridge); Lastmovebackup=BestMove;}

        if (rightbridge==TRUE && leftbridge==TRUE)         // Si la ligne des two-bridge / proximitymove est construite
            connectingthebridge(playerhex,GRID,&BestMove); // Connecter la ligne des two-bridge

        if (check_edgeline(BestMove,Lastline_P1)==TRUE || check_edgeline(BestMove,beforeLastline_P1)==TRUE )
            rightbridge=TRUE; // On vérifie si la ligne est connecté à l'extrémité droite

        if (check_edgeline(BestMove,Firstline_P1)==TRUE || check_edgeline(BestMove,afterFirstline_P1)==TRUE )
            leftbridge=TRUE; // On vérifie si la ligne est connecté à l'extrémité gauche

    }

    fprintf(stdout,"Rightbridge = %d, Leftbridge = %d\n",rightbridge,leftbridge);
    fprintf(stdout,"LastMoveBackup = %d \n",Lastmovebackup);

    SDL_Rect hex_position=pos_finder(BestMove);
    if (turn==1)
    {
        SDL_BlitSurface(black_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player2_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player1clicksound);
        GRID[BestMove]=1;
        *Blackplaying=FALSE;
    }
    else
    {
        SDL_BlitSurface(white_hex,NULL,SDL_GetWindowSurface(Window),&hex_position);
        SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
        SDL_UpdateWindowSurface(Window);
        buttonsound(player2clicksound);
        GRID[BestMove]=2;
        *Blackplaying=TRUE;
    }
    (*p)++;
    return BestMove;
}


int hardCPU_relatedMove(int playerhex, int GRID[81], int turn) //The hex wich is linked to the last hex played by the human.
{
    int j=0;
    int* voisins=voisin(playerhex);

        for (j=0; j<6; j++)
        {
            if (GRID[voisins[j]]==turn) return voisins[j];
        }

}


int areHexagonsLinked(int hexagon1, int hexagon2)
{
    int i;
    int* voisins;
    voisins=voisin(hexagon1);

    for (i=0; i<6; i++)
    {
        if (voisins[i]==hexagon2) return i;

    }
    return -1;

}

int BestMoveifLinked(int voisin_indice, int relatedMove, int *ForcedLastmoveBackup)
{
    switch(voisin_indice)
    {
    case 0:
        *ForcedLastmoveBackup=relatedMove-1;
        return 2;

    case 1:
        if (check_edgeline(relatedMove,Lastline_P1)==TRUE)
            return 2;
        if (GRID[relatedMove-7]==2)
            return relatedMove+10;
        return 3;
    case 2:

        if (check_edgeline(relatedMove,Lastline_P1)==TRUE)
            return 1;
        *ForcedLastmoveBackup=relatedMove+8;
        return 4;
    case 3:
        if (GRID[relatedMove+10]==1) // for connecting bridges purposes
            return 5;
        if (GRID[relatedMove+1]==2&&GRID[relatedMove-9]==2&&GRID[relatedMove-8]==-1)  //FOR COMPLEX STRATEGIES ( whern stuck between two
            return relatedMove-8;
        if (GRID[relatedMove+1]==2&&GRID[relatedMove+8]==2) //for COMPLEX STRATEGIES (when between 2 ennemy hexagons and best move is filled by ennemy
            return 5;
        if (GRID[relatedMove+1]==2&&GRID[relatedMove-1]==2&&GRID[relatedMove-17]==2&&GRID[relatedMove+17]==-1)
            return relatedMove+17;
        if (GRID[relatedMove+1]==2&&GRID[relatedMove-1]==2&&GRID[relatedMove+17]==2&&GRID[relatedMove-17]==-1)
            return relatedMove-17;
        if (GRID[relatedMove+9]==1&&GRID[relatedMove+1]==2&&GRID[relatedMove+10]==-1)
            return relatedMove+10;
        if (GRID[relatedMove-7]==2&&GRID[relatedMove-17]==-1)
            return relatedMove-17;

        return 1;
    case 4:
        if (GRID[relatedMove+17]==1&&GRID[relatedMove+9]==-1) // for connecting bridges purposes
            return relatedMove+9;
        if (check_edgeline(relatedMove,Firstline_P1)==TRUE)
            return 4;
        if (GRID[relatedMove+7]==2)
            return relatedMove-10;

        *ForcedLastmoveBackup=relatedMove-1;
        return 2;
    case 5:
        if (GRID[relatedMove+17]==1&&GRID[relatedMove+8]==-1) // for connecting bridges purposes
            return relatedMove+8;
        if (GRID[relatedMove-17]==1&&GRID[relatedMove+7]==1&&GRID[relatedMove+9+9]==-1)
            return relatedMove+9+9;
        return 3;
    }
}

int proximitymove(int relatedMove, int playerhex, int *ForcedLastMoveBackup, int* CPUvoisins)
{
    int BestMove;
    int ennemy_index=areHexagonsLinked(relatedMove,playerhex);
    int BestMoveIndex=BestMoveifLinked(ennemy_index,relatedMove,&(ForcedLastmoveBackup));
    if (BestMoveIndex<6) BestMove=CPUvoisins[BestMoveIndex];
    else BestMove=BestMoveIndex;
    Lastmovebackup=BestMove;

    fprintf(stdout,"BestMove = %d - Based on proximity \n",BestMove);

    if (GRID[BestMove]!=-1)
        {BestMove=SMARTrandomize(GRID,playerhex); fprintf(stdout," - FAIL! Smart Random Activated ");}

    return BestMove;
}
int rightbridgemaking(int hexagon, int playerhex, int *rightbridge) //hex-7 = yop hexagon, hex+10 = bottom hexagon
{
    fprintf(stdout,"We are in rightbridgemaking(%d,rightbridge = %d)\n",hexagon,*rightbridge);

    int BestMove;

        if (GRID[hexagon-7]==1) return rightbridgemaking(hexagon-7,playerhex,&(*rightbridge));
        else if (GRID[hexagon+10]==1) return rightbridgemaking(hexagon+10,playerhex,&(*rightbridge));
        else if (GRID[hexagon+1]==1) return rightbridgemaking(hexagon+1,playerhex,&(*rightbridge));
        else if (GRID[hexagon-8]==1) return rightbridgemaking(hexagon-8,playerhex,&(*rightbridge));
        else if (GRID[hexagon+9]==1) return rightbridgemaking(hexagon+9,playerhex,&(*rightbridge));
        else if (GRID[hexagon-17]==1 && hexagon!=59) return rightbridgemaking(hexagon-17,playerhex,&(*rightbridge));
        else if (GRID[hexagon+17]==1) return rightbridgemaking(hexagon+17,playerhex,&(*rightbridge));

        else if (GRID[hexagon-7]==-1&&GRID[hexagon+1]!=2&&GRID[hexagon-8]!=2&&GRID[hexagon-7+1]!=2&&GRID[hexagon-7-8]!=2&&GRID[hexagon-7-7-9]!=2) BestMove = hexagon-7;
        //last two conditions are for beforelastline border
        else if (GRID[hexagon+10]==-1&&GRID[hexagon-7]!=1&&GRID[hexagon+1]!=2&&GRID[hexagon+9]!=2&&GRID[hexagon+10+1]!=2&&GRID[hexagon+10-8]!=2) BestMove = hexagon+10; //&&GRID[hexagon-7]==2 was here

        else if (GRID[hexagon-7]==2&&(GRID[hexagon-7+9]==2 || GRID[hexagon+10]==2 || GRID[hexagon+10+1]==2 )&&GRID[hexagon-17]==-1) BestMove = hexagon-17; //complex strategy


        else if (GRID[hexagon-17]==-1&&GRID[hexagon-7]==2&&check_edgeline(hexagon-17,beforeLastline_P1)==TRUE) BestMove = hexagon-17;
        else if (GRID[hexagon+10]==-1&&GRID[hexagon-7]!=1&&GRID[hexagon+10]!=1&&GRID[hexagon+1]==-1) BestMove = hexagon+1; //Strategy purposes
        else if (GRID[hexagon+10]==2&&GRID[hexagon-7]!=1&&GRID[hexagon+10]!=1&&GRID[hexagon-8]==2&&GRID[hexagon+1]==-1) BestMove = hexagon+1; //Strategy purposes

        if (GRID[BestMove]!=-1)
        {BestMove=SMARTrandomize(GRID,playerhex); fprintf(stdout,"FAIL! SMART Move Activated\n");}

        return BestMove;

}


int SMARTrandomize(int GRID[81], int hexagon)
{
    int* voisins;
    voisins=voisin(hexagon);
    int BestMoveIndex,BestMove;
    BestMoveIndex=rand()%6;
    BestMove=voisins[BestMoveIndex];
    while (GRID[BestMove]!=-1)
    {
        BestMoveIndex=rand()%6;
        BestMove=voisins[BestMoveIndex];
    }

    return BestMove;
}

int ABSOLUTErandomize(int GRID[81])
{
    int BestMove;
    BestMove=rand()%81;
    while (GRID[BestMove]!=-1)
        BestMove=rand()%81;

    return BestMove;
}

int leftbridgemaking(int playerhex, int *Lastmovebackup, int* ForcedLastmoveBackup, int* leftbridge)
{
    int BestMove;
    int Lastmove;
    Lastmove=*ForcedLastmoveBackup;


    if (GRID[Lastmove+7]==-1&&GRID[Lastmove-1]!=2&&GRID[Lastmove+8]!=2&&GRID[Lastmove+7-1]!=2&&GRID[Lastmove+7+8]!=2&&GRID[Lastmove+7+7+9]!=2) BestMove = Lastmove+7;
    //last two conditions are for beforelastline border
    else if (GRID[Lastmove-10]==-1&&GRID[Lastmove+7]!=1&&GRID[Lastmove-1]!=2&&GRID[Lastmove-9]!=2&&GRID[Lastmove-10-1]!=2&&GRID[Lastmove-10+8]!=2) BestMove = Lastmove-10; //&&GRID[Lastmove+7]==2 was here

    else if (GRID[Lastmove+7]==2&&GRID[Lastmove+7-9]==2&&GRID[Lastmove+17]==-1) BestMove = Lastmove+17;
    else if (GRID[Lastmove+7]==2&&GRID[Lastmove-10]==2&&GRID[Lastmove+17]==-1) BestMove = Lastmove+17; //if two routes are blocked, go top!
    else if (GRID[Lastmove+7]==2&&GRID[Lastmove-10-1]==2&&GRID[Lastmove+17]==-1) BestMove = Lastmove+17; //if two routes are blocked, go top!

    //else if (GRID[Lastmove-1]==1&&GRID[Lastmove+7]!=1&&GRID[Lastmove-10]!=1&&GRID[Lastmove-1]==-1) BestMove = Lastmove-1; //Strategy purposes
    else if (GRID[Lastmove+17]==-1&&GRID[Lastmove+7]==2&&check_edgeline(Lastmove+17,afterFirstline_P1)==TRUE) return Lastmove+17;
    else if (GRID[Lastmove-10]==-1&&GRID[Lastmove+7]!=1&&GRID[Lastmove-10]!=1&&GRID[Lastmove-1]==-1) BestMove = Lastmove-1; //Strategy purposes
    else if (GRID[Lastmove-10]==2&&GRID[Lastmove+7]!=1&&GRID[Lastmove-10]!=1&&GRID[Lastmove+8]==2&&GRID[Lastmove-1]==-1) BestMove = Lastmove-1;

    else BestMove=-1;
    fprintf(stdout,"BestMove by forcing = %d based on Lastmove = %d\n",BestMove,Lastmove);

    if (BestMove!=-1) {/*Lastmovebackup=Lastmove;*/ *ForcedLastmoveBackup=BestMove;}



    if (GRID[BestMove]!=-1)
        {BestMove=SMARTrandomize(GRID,playerhex); fprintf(stdout,"FAIL! SMART Move Activated\n");}

    return BestMove;

}


void connectingthebridge(int playerhex, int GRID[81], int* BestMove)
{

    /* for bridge type n1 (going down) */
    if (GRID[playerhex-9]==1 && GRID[playerhex+1]==1 && GRID[playerhex-8]==-1)
        *BestMove = playerhex-8;
    else if (GRID[playerhex+9]==1 && GRID[playerhex-1]==1 && GRID[playerhex+8]==-1)
        *BestMove = playerhex+8;

    /* for bridge type n2 (going UP) */
    else if (GRID[playerhex+8]==1 && GRID[playerhex+1]==1 && GRID[playerhex+9]==-1)
        *BestMove = playerhex+9;
    else if (GRID[playerhex-8]==1 && GRID[playerhex-1]==1 && GRID[playerhex-9]==-1)
        *BestMove = playerhex-9;

    /* for bridge type n3 (verticl line) */
    else if (GRID[playerhex-8]==1 && GRID[playerhex+9]==1 && GRID[playerhex+1]==-1)
        *BestMove = playerhex+1;
    else if (GRID[playerhex-9]==1 && GRID[playerhex+8]==1 && GRID[playerhex-1]==-1)
        *BestMove = playerhex-1;

    /* for bridge type n4 (before lastline) */
    else if (check_edgeline(playerhex,Lastline_P1)==TRUE && GRID[playerhex+8]==1 && GRID[playerhex+9]==-1)
        *BestMove = playerhex+9;
    else if (check_edgeline(playerhex,Lastline_P1)==TRUE && GRID[playerhex-1]==1 && GRID[playerhex-9]==-1)
        *BestMove = playerhex-9;

    /* for bridge type n5 (after firstline) */
    else if (check_edgeline(playerhex,Firstline_P1)==TRUE && GRID[playerhex+1]==1 && GRID[playerhex+9]==-1)
        *BestMove = playerhex+9;
    else if (check_edgeline(playerhex,Firstline_P1)==TRUE && GRID[playerhex-8]==1 && GRID[playerhex-9]==-1)
        *BestMove = playerhex-9;

    else
        {*BestMove=SMARTrandomize(GRID,playerhex); return;}



}

void AI_initialiser(int *rightbridge,int *ForcedLastmoveBackup,int *leftbridge)
{
    *rightbridge=FALSE;
    *leftbridge=FALSE;
    *ForcedLastmoveBackup=40;
}

void fill_Lines(int Lines[9][9])
{
    int i,j,value;
    for (i=0;i<9;i++)
    {   value=i;
        for (j=0;j<9;j++)
        {

            Lines[i][j]=value;
            value=value+9;
        }
    }

}

int sidechecker(int playerhex) //0 for left side, 1 for right side!
{
    int i;
    for (i=0; i<4; i++)
    {
        if (check_edgeline(playerhex,Lines[i])==TRUE)
            return 0;
    }
    return 1;
}

void congratulations_P1(int *backfromgame, SDL_Window* Window)
{
    SDL_Surface* congratulationsP1 = IMG_Load("images/congratualtionsP1.jpg");
    SDL_BlitSurface(congratulationsP1,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);
    Mix_PauseMusic();
    buttonsound(congratulationssound);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
            if (551<event.button.x&&event.button.x<765&&475<event.button.x&&event.button.y<537)
                {
                    SDL_FreeSurface(congratulationsP1);
                    *backfromgame=TRUE;
                    buttonsound(buttonclicksound);
                    return;
                }


}


void congratulations_P2(int *backfromgame,SDL_Window* Window)
{
    SDL_Surface* congratulationsP2 = IMG_Load("images/congratualtionsP2.jpg");
    SDL_BlitSurface(congratulationsP2,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);
    Mix_PauseMusic();
    buttonsound(congratulationssound);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
            if (551<event.button.x&&event.button.x<765&&475<event.button.x&&event.button.y<537)
                {
                    SDL_FreeSurface(congratulationsP2);
                    *backfromgame=TRUE;
                    buttonsound(buttonclicksound);
                    return;
                }


}

void CPU_Wins(int *backfromgame,SDL_Window* Window)
{
    SDL_Surface* CPUWins = IMG_Load("images/CPUWins.jpg");
    SDL_BlitSurface(CPUWins,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);
    Mix_PauseMusic();
    buttonsound(CPUWinssound);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
            if (551<event.button.x&&event.button.x<765&&475<event.button.x&&event.button.y<537)
                {
                    SDL_FreeSurface(CPUWins);
                    *backfromgame=TRUE;
                    buttonsound(buttonclicksound);
                    return;
                }


}


int rockpaperscissors(int difficulty, SDL_Window* Window)
{
    int human_choice,cpu_choice;
    int humanfirst; //if human first then first = TRUE, else first=FALSE

    /*chargement des images */
    SDL_Surface* rockpaperscissors = IMG_Load("images/rockpaperscissors.jpg");
    SDL_Surface* rockhover = IMG_Load("images/rockhover.jpg");
    SDL_Surface* paperhover = IMG_Load("images/paperhover.jpg");
    SDL_Surface* scissorshover = IMG_Load("images/scissorshover.jpg");
    SDL_Surface* VS = IMG_Load("images/VS.jpg");
    SDL_Surface* rock = IMG_Load("images/rock.jpg");
    SDL_Surface* paper = IMG_Load("images/paper.jpg");
    SDL_Surface* scissors = IMG_Load("images/scissors.jpg");
    SDL_Surface* lucky = IMG_Load("images/lucky.jpg");
    SDL_Surface* unfortunately = IMG_Load("images/unfortunately.jpg");
    SDL_Surface* playbutton = IMG_Load("images/playbutton.jpg");
    SDL_Surface* playbuttonhover = IMG_Load("images/playbuttonhover.jpg");

    /*on dessine l'arriere plan*/
    SDL_BlitSurface(rockpaperscissors,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (444<event.button.x&&event.button.x<562&&455<event.button.y&&event.button.y<615)
                    {human_choice=1; goto end;}
                else if (622<event.button.x&&event.button.x<746&&455<event.button.y&&event.button.y<615)
                    {human_choice=2; goto end;}
                else if (810<event.button.x&&event.button.x<930&&455<event.button.y&&event.button.y<615)
                    {human_choice=3; goto end;}
                else if (349<event.button.x&&event.button.x<410&&654<event.button.y&&event.button.y<714)
                    return -1;


        }
        else if (event.type==SDL_MOUSEMOTION )
        {
            if (444<event.button.x&&event.button.x<562&&455<event.button.y&&event.button.y<615)
            {
                SDL_BlitSurface(rockhover,NULL,SDL_GetWindowSurface(Window),&buttonposition2);
                SDL_UpdateWindowSurface(Window);
            }
            else if (622<event.button.x&&event.button.x<746&&455<event.button.y&&event.button.y<615)
            {
                SDL_BlitSurface(paperhover,NULL,SDL_GetWindowSurface(Window),&buttonposition2);
                SDL_UpdateWindowSurface(Window);
            }
            else if (810<event.button.x&&event.button.x<930&&455<event.button.y&&event.button.y<615)
            {
                SDL_BlitSurface(scissorshover,NULL,SDL_GetWindowSurface(Window),&buttonposition2);
                SDL_UpdateWindowSurface(Window);
            }

        }
    }
end:
    buttonsound(buttonclicksound);
    cpu_choice=rand()%3+1; //1,2,3
    while (cpu_choice==human_choice)
        cpu_choice=rand()%3+1;

        switch(human_choice)
    {
        case 1: if (cpu_choice==2) humanfirst=FALSE;
                else humanfirst=TRUE;
                break;

        case 2: if (cpu_choice==3) humanfirst=FALSE;
                else humanfirst=TRUE;
                break;

        case 3: if (cpu_choice==1) humanfirst=FALSE;
                else humanfirst=TRUE;
                break;

    }

    if (difficulty==3)
        humanfirst=FALSE;

    /*on dessine l'arriere plan*/
    SDL_BlitSurface(VS,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    /* on dessine choix de joeuur */
    if (human_choice==1)
    {
                SDL_BlitSurface(rock,NULL,SDL_GetWindowSurface(Window),&humanchoice);
                SDL_UpdateWindowSurface(Window);
    }
    else if (human_choice==2)
    {
                SDL_BlitSurface(paper,NULL,SDL_GetWindowSurface(Window),&humanchoice);
                SDL_UpdateWindowSurface(Window);

    }
    else
    {
                SDL_BlitSurface(scissors,NULL,SDL_GetWindowSurface(Window),&humanchoice);
                SDL_UpdateWindowSurface(Window);
    }

    /* un peu de suspens */
    cpuchoosing(Window);
    printf("choix de cpu = %d",cpu_choice);

    /*on dessine le choix de cpu */
    if (difficulty!=3)
    {

        if (cpu_choice==1)
        {
                    SDL_BlitSurface(rock,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);
        }
        else if (cpu_choice==2)
        {
                    SDL_BlitSurface(paper,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);

        }
        else
        {
                    SDL_BlitSurface(scissors,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);
        }
    }
    else
    {
        if (human_choice==1)
        {
                    SDL_BlitSurface(paper,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);
        }
        else if (human_choice==2)
        {
                    SDL_BlitSurface(scissors,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);

        }
        else
        {
                    SDL_BlitSurface(rock,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
                    SDL_UpdateWindowSurface(Window);
        }
    }

    /* affichage de message de resultat et button de lancement*/

    if (humanfirst==TRUE)
        SDL_BlitSurface(lucky,NULL,SDL_GetWindowSurface(Window),&message);

    else
        SDL_BlitSurface(unfortunately,NULL,SDL_GetWindowSurface(Window),&message);

        SDL_BlitSurface(playbutton,NULL,SDL_GetWindowSurface(Window),&playbuttonlocation);
        SDL_UpdateWindowSurface(Window);


    /* CLICK sur button play */
    SDL_Event event2;
    while (SDL_WaitEvent(&event2) )
    {
        if (event2.type==SDL_MOUSEBUTTONDOWN && event2.button.button==SDL_BUTTON_LEFT)
        {
                if (600<event2.button.x&&event2.button.x<774&&614<event2.button.y&&event2.button.y<669)
                    goto finalend;



        }
        else if (event2.type==SDL_MOUSEMOTION )
        {
            if (600<event2.button.x&&event2.button.x<774&&614<event2.button.y&&event2.button.y<669)
            {
                SDL_BlitSurface(playbuttonhover,NULL,SDL_GetWindowSurface(Window),&playbuttonlocation);
                SDL_UpdateWindowSurface(Window);
            }
            else
            {
                SDL_BlitSurface(playbutton,NULL,SDL_GetWindowSurface(Window),&playbuttonlocation);
                SDL_UpdateWindowSurface(Window);
            }

        }
    }




finalend:
    buttonsound(buttonclicksound);
    /*Libération d'espace mémoire*/
    SDL_FreeSurface(rockpaperscissors);
    SDL_FreeSurface(rockhover);
    SDL_FreeSurface(paperhover);
    SDL_FreeSurface(scissorshover);
    SDL_FreeSurface(VS);
    SDL_FreeSurface(rock);
    SDL_FreeSurface(paper);
    SDL_FreeSurface(scissors);
    SDL_FreeSurface(lucky);
    SDL_FreeSurface(unfortunately);
    SDL_FreeSurface(playbutton);
    SDL_FreeSurface(playbuttonhover);

    return humanfirst;



}

void cpuchoosing(SDL_Window* Window)
{
    /* Let's load pictures */
    SDL_Surface* rock = IMG_Load("images/rock.jpg");
    SDL_Surface* paper = IMG_Load("images/paper.jpg");
    SDL_Surface* scissors = IMG_Load("images/scissors.jpg");

    int i;
    for (i=0; i<5; i++)
    {
        SDL_BlitSurface(rock,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
        SDL_UpdateWindowSurface(Window);
        SDL_Delay(100);
        SDL_BlitSurface(paper,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
        SDL_UpdateWindowSurface(Window);
        SDL_Delay(100);
        SDL_BlitSurface(scissors,NULL,SDL_GetWindowSurface(Window),&cpuchoice);
        SDL_UpdateWindowSurface(Window);
        SDL_Delay(100);
    }

}
