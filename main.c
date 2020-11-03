#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>
#define TRUE 1
#define FALSE 0

#include "functions.h"
#include "variables.h"


int main(int argc, char** argv)
{
    /* nécessaure pour la fonction rand qui génére un nombre aléatoire */
    srand ( time(NULL) );

    /* Initialisation simple de la SDL */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    /* Création de la fenêtre principale SDL et son titre*/
    SDL_Window* Window = NULL;
    Window = SDL_CreateWindow("Hex Strategy Game - Issam Assafi",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1344,756,SDL_WINDOW_SHOWN);

    /* Chargement des images */
    grid = IMG_Load("images/hex_grid.jpg");
    white_hex = IMG_Load("images/white_hex.png");
    black_hex = IMG_Load("images/black_hex.png");
    player1_turn = IMG_Load("images/player1_turn.png");
    player2_turn = IMG_Load("images/player2_turn.png");
    robot1_turn = IMG_Load("images/robot1_turn.png");
    robot2_turn = IMG_Load("images/robot2_turn.png");

    /*Initialisation de SDL mixer nécessaire pour fonctionner l'audio */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    /* Chargement des fichiers audio (music & sound effects) */
    navigationmusic = Mix_LoadMUS("audio/navigationmusic.mp3");
    gameplaymusic = Mix_LoadMUS("audio/gameplaymusic.mp3");
    buttonclicksound = Mix_LoadWAV("audio/buttonclicksound.wav");
    player1clicksound = Mix_LoadWAV("audio/player1clicksound.wav");
    player2clicksound = Mix_LoadWAV("audio/player2clicksound.wav");
    congratulationssound = Mix_LoadWAV("audio/congratulationssound.wav");
    CPUWinssound = Mix_LoadWAV("audio/CPUWinssound.wav");

    /* La music est ON par défault lors de premier démarrage */
    Mix_VolumeMusic(50);
    Mix_PlayMusic(navigationmusic,-1);

    //  Variabke permettant de revenir à la page d'acceuil lors de jeu en cliquan sur la touche Esc
    int backfromgame=FALSE;


while(1) // Boucle principale de jeu
{
    homepage:
    if (backfromgame==TRUE) {Mix_PlayMusic(navigationmusic,-1); backfromgame=FALSE;}

    int choice=homepage_choice(Window); // Affichage de Page d'acceuil ou première menu et acquisition de choix effectué
    switch(choice)
    {
    case 1://Play
        gamemode:
        choice=gamemode_choice(Window); // Affichage de menu de mode de jeu (1 joueur) ou (2 joueurs) et acquisition de choix effectué
        switch(choice)
        {
        case 1://1 Player ( C'est le mode Homme vs. Maachine )
            difficulty:
            choice=difficulty_choice(Window); // Affichage de menu de difficulté de jeu (Facile, Normal ou Difficile) et acquisition de choix effectué
            switch(choice)
            {
            case 1://EASY
            case 2://Medium
            case 3:;//Hard

                /* on détermine qui jouera le premier avec un petit jeu de Pierre-papier-ciseaux */
                int humanstart=rockpaperscissors(choice,Window);
                if (humanstart==-1) goto difficulty;

                Mix_PlayMusic(gameplaymusic,-1); // on change la music lorce que la partie commence

                newgame_1player:;
                /* Initialisation ou remise à zéro de plusieurs variables afin d'assure une nouvelle partie */
                database_initialiser(&Blackplaying,&p1,&p2,GRID);
                AI_initialiser(&rightbridge,&ForcedLastmoveBackup,&leftbridge);

                showgrid(Window); // On affiche la table de jeu

                /* Le cas où l'Homme jouera en premier */
                if (humanstart==TRUE)
                {
                    /* On affiche à qui est le tour de jouer */
                    SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
                    SDL_UpdateWindowSurface(Window);

                    /* La partie commence */
                    while(p1<=41&&p2<=41)
                    {
                        /* Initialisation des paramètres de vérification du gagnant */
                        check_initialiser(&Left,&Right,&Top,&Down,destroyed_P1,destroyed_P2);

                        SDL_Event event; // L'evenement de clic gauche de joueur
                        if ( SDL_WaitEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                        {
                            int hexagon=hex_finder(event); // récupération de l'hexagon ayant été cliqué par le joueur.
                            if (0<=hexagon&&hexagon<=80&&GRID[hexagon]==-1) // vérification si la case cliqué est effectivement vide
                                if (Blackplaying==TRUE) // si c'est le joueur 1 (jetons noirs) qui a le tour
                                {
                                    /* On effectue le mouvement désiré par le joueur Homme*/
                                    joueur_humain(hexagon,&p1,1,&Blackplaying,-1,GRID,Window);

                                    /* On vérifie si le joueur a gagné la partie */
                                    checkwin(hexagon,Firstline_P1,Lastline_P1,destroyed_P1,&Left,&Right,1);
                                    if (Left==TRUE&&Right==TRUE) {congratulations_P1(&backfromgame,Window); goto homepage;}
                                }

                        }
                        if (Blackplaying==FALSE) // si c'est le joueur 2 (jetons verts) qui a le tour
                        {
                            SDL_Delay(450);
                            int playerhex=hex_finder(event); // on récupére la position joueé par l'HOMME (utile pour l'intelligence artificielle)

                            /* On effectue le mouvement décidé par la Machine*/
                            int cpuhex=joueur_ordinateur(playerhex,&p2,2,choice,&Blackplaying,GRID,Window);

                            /* On vérifie si la Machine a gagné la partie */
                            checkwin(cpuhex,Firstline_P2,Lastline_P2,destroyed_P2,&Top,&Down,2);
                            if (Top==TRUE&&Down==TRUE) {CPU_Wins(&backfromgame,Window); goto homepage;}
                        }
                        if (isreturntohome(&backfromgame,event)==TRUE) goto homepage; // Button de retour avec la touche Esc si la partie est encore en cours.
                        else if (isreturntohome(&backfromgame,event)==FALSE) goto newgame_1player; // Button permettant de recommencer à nouveau
                    }
                }

                /* Le cas où la Machine jouera en premier */
                else
                {
                    SDL_BlitSurface(robot1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
                    SDL_UpdateWindowSurface(Window);
                    /* La partie commence */
                    while(p1<=41&&p2<=41)
                    {
                        SDL_Event event;
                        /* Initialisation des paramètres de vérification du gagnant */
                        check_initialiser(&Left,&Right,&Top,&Down,destroyed_P1,destroyed_P2);
                        if (Blackplaying==TRUE) // si c'est le joueur 1 (jetons noirs) qui a le tour
                        {
                            SDL_Delay(450);

                            int playerhex=hex_finder(event); // récupération de l'hexagon ayant été cliqué par le joueur.

                            /* On effectue le mouvement décidé par la Machine*/
                            int cpuhex=joueur_ordinateur(playerhex,&p1,1,choice,&Blackplaying,GRID,Window);

                            /* On vérifie si la Machine a gagné la partie */
                            checkwin(cpuhex,Firstline_P1,Lastline_P1,destroyed_P1,&Left,&Right,1);
                            if (Left==TRUE&&Right==TRUE) {CPU_Wins(&backfromgame,Window); goto homepage;}


                        }


                        if ( SDL_WaitEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                        {
                            int hexagon=hex_finder(event);
                            if (0<=hexagon&&hexagon<=80&&GRID[hexagon]==-1)
                                if (Blackplaying==FALSE) // si c'est le joueur 2 (jetons verts) qui a le tour
                                {
                                    /* On effectue le mouvement désiré par le joueur Homme*/
                                    joueur_humain(hexagon,&p2,2,&Blackplaying,-1,GRID,Window);

                                    /* On vérifie si le joueur a gagné la partie */
                                    checkwin(hexagon,Firstline_P2,Lastline_P2,destroyed_P2,&Top,&Down,2);
                                    if (Top==TRUE&&Down==TRUE) {congratulations_P2(&backfromgame,Window); goto homepage;}
                                }

                        }
                        if (isreturntohome(&backfromgame,event)==TRUE) goto homepage; //permet le Retour à l'accueil
                        else if (isreturntohome(&backfromgame,event)==FALSE) goto newgame_1player;

                    }
                }

                break;

            case 4://Return ( button de retour )
                goto gamemode;
                break;

            }
        case 2://2 Players

            /* Procédure similaire à celle de Homme vs. Machine (là on n'apelle que joueur_humain )*/
            Mix_PlayMusic(gameplaymusic,-1); // on change la music lorce que la partie commence

            newgame_2players:
            database_initialiser(&Blackplaying,&p1,&p2,GRID);
            showgrid(Window);
            SDL_BlitSurface(player1_turn,NULL,SDL_GetWindowSurface(Window),&playerturn);
            SDL_UpdateWindowSurface(Window);
            while(p1<=41&&p2<=41)
            {
                check_initialiser(&Left,&Right,&Top,&Down,destroyed_P1,destroyed_P2);
                SDL_Event event;
                if ( SDL_WaitEvent(&event))
                {
                    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                    {
                    int hexagon=hex_finder(event);
                    if (0<=hexagon&&hexagon<=80&&GRID[hexagon]==-1)
                    {
                        if (Blackplaying==TRUE)
                        {
                            joueur_humain(hexagon,&p1,1,&Blackplaying,1,GRID,Window);
                            checkwin(hexagon,Firstline_P1,Lastline_P1,destroyed_P1,&Left,&Right,1);
                            if (Left==TRUE&&Right==TRUE) {congratulations_P1(&backfromgame,Window); goto homepage;}
                        }

                        else
                        {
                            joueur_humain(hexagon,&p2,2,&Blackplaying,1,GRID,Window);
                            checkwin(hexagon,Firstline_P2,Lastline_P2,destroyed_P2,&Top,&Down,2);
                            if (Top==TRUE&&Down==TRUE) {congratulations_P2(&backfromgame,Window); goto homepage;}

                        }
                    }
                }
                else if (isreturntohome(&backfromgame,event)==TRUE) {goto homepage;}
                if (isreturntohome(&backfromgame,event)==FALSE) {goto newgame_2players;}
                else continue;

            }
            }
            break;

        case 3://Return
            goto homepage;
            break;
        }
    break;
    case 2://Rules
        rules(Window);
        break;
    case 3://Options
        options(Window);
        break;
    case 4://Quit
        SDL_DestroyWindow(Window);
        return 0;
    }


}

    return 0;

}
