#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include "variables.h"

int homepage_choice(SDL_Window* Window)
{
    int choice;

    /*chargement des images */
    SDL_Surface* homepage = IMG_Load("images/homepage.jpg");
    SDL_Surface* playhover = IMG_Load("images/playhover.jpg");
    SDL_Surface* ruleshover = IMG_Load("images/ruleshover.jpg");
    SDL_Surface* optionshover = IMG_Load("images/optionshover.jpg");
    SDL_Surface* quithover = IMG_Load("images/quithover.jpg");
    SDL_Surface* cleanhomepagemenu = IMG_Load("images/cleanhomepagemenu.jpg");


    /*on dessine l'arriere plan*/
    SDL_BlitSurface(homepage,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
                    {choice=1; goto end;}
                else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
                    {choice=2; goto end;}
                else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
                    {choice=3; goto end;}
                else if (580<event.button.x&&event.button.x<792&&594<event.button.y&&event.button.y<654)
                    {choice=4; goto end;}

        }
        else if (event.type==SDL_MOUSEMOTION )
        {
            if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
            {
                SDL_BlitSurface(playhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
            {
                SDL_BlitSurface(ruleshover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
            {
                SDL_BlitSurface(optionshover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&594<event.button.y&&event.button.y<654)
            {
                SDL_BlitSurface(quithover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else
            {
                SDL_BlitSurface(cleanhomepagemenu,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
        }
    }

end:buttonsound(buttonclicksound);
    /*Libération d'espace mémoire*/
    SDL_FreeSurface(homepage);
    SDL_FreeSurface(playhover);
    SDL_FreeSurface(ruleshover);
    SDL_FreeSurface(optionshover);
    SDL_FreeSurface(quithover);
    //SDL_FreeSurface(cleanhomepagemenu);
    return(choice);
}

void rules(SDL_Window* Window)
{

    /*Chargement des images */
    SDL_Surface* rules = IMG_Load("images/rules.jpg");

    /*on dessine l'arriere plan*/
    SDL_BlitSurface(rules,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (349<event.button.x&&event.button.x<410&&654<event.button.y&&event.button.y<714)
                    {
                        SDL_FreeSurface(rules);
                        buttonsound(buttonclicksound);
                        return;
                    }

        }
    }
}

void options(SDL_Window* Window)
{

    /*Chargement des images */
    SDL_Surface* optionsON = IMG_Load("images/optionsON.jpg");
    SDL_Surface* optionsOFF = IMG_Load("images/optionsOFF.jpg");

    /*on dessine l'arriere plan*/
    if (Mix_PausedMusic()==TRUE) SDL_BlitSurface(optionsOFF,NULL,SDL_GetWindowSurface(Window),&position);
    else SDL_BlitSurface(optionsON,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);


    while(1)
    {
    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (349<event.button.x&&event.button.x<410&&654<event.button.y&&event.button.y<714)
                    {
                        SDL_FreeSurface(optionsON);
                        SDL_FreeSurface(optionsOFF);
                        buttonsound(buttonclicksound);
                        return;
                    }
                else if (683<event.button.x&&event.button.x<797&&572<event.button.y&&event.button.y<610)
                    {
                        if (Mix_PausedMusic()==TRUE)
                        {
                            SDL_BlitSurface(optionsON,NULL,SDL_GetWindowSurface(Window),&position);
                            SDL_UpdateWindowSurface(Window);
                            Mix_ResumeMusic();
                        }
                        else
                        {
                            SDL_BlitSurface(optionsOFF,NULL,SDL_GetWindowSurface(Window),&position);
                            SDL_UpdateWindowSurface(Window);
                            Mix_PauseMusic();
                        }
                    }

        }
    }

    }
}

int gamemode_choice(SDL_Window* Window)
{
    int choice;

    /*chargement des images */
    SDL_Surface* gamemode = IMG_Load("images/gamemode.jpg");
    SDL_Surface* oneplayerhover = IMG_Load("images/1playerhover.jpg");
    SDL_Surface* twoplayershover = IMG_Load("images/2playershover.jpg");
    SDL_Surface* returnhover = IMG_Load("images/returnhover.jpg");
    SDL_Surface* cleangamemmodemenu = IMG_Load("images/cleangamemodemenu.jpg");


    /*on dessine l'arriere plan*/
    SDL_BlitSurface(gamemode,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
                    {choice=1; goto end;}
                else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
                    {choice=2; goto end;}
                else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
                    {choice=3; goto end;}
        }
        else if (event.type==SDL_MOUSEMOTION )
        {
            if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
            {
                SDL_BlitSurface(oneplayerhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
            {
                SDL_BlitSurface(twoplayershover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
            {
                SDL_BlitSurface(returnhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else
            {
                SDL_BlitSurface(cleangamemmodemenu,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }

        }
    }
end:
    buttonsound(buttonclicksound);
    /*Libération d'espace mémoire*/
    SDL_FreeSurface(oneplayerhover);
    SDL_FreeSurface(twoplayershover);
    SDL_FreeSurface(returnhover);
    SDL_FreeSurface(gamemode);
    SDL_FreeSurface(cleangamemmodemenu);
    return(choice);

}


int difficulty_choice(SDL_Window* Window)
{
    int choice;

    /*chargement des images */
    SDL_Surface* difficulty = IMG_Load("images/difficulty.jpg");
    SDL_Surface* easyhover = IMG_Load("images/easyhover.jpg");
    SDL_Surface* mediumhover = IMG_Load("images/mediumhover.jpg");
    SDL_Surface* hardhover = IMG_Load("images/hardhover.jpg");
    SDL_Surface* returnhover2 = IMG_Load("images/returnhover2.jpg");
    SDL_Surface* cleandifficultyhover = IMG_Load("images/cleandifficultymenu.jpg");


    /*on dessine l'arriere plan*/
    SDL_BlitSurface(difficulty,NULL,SDL_GetWindowSurface(Window),&position);
    SDL_UpdateWindowSurface(Window);

    SDL_Event event;
    while (SDL_WaitEvent(&event) )
    {
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        {
                if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
                    {choice=1; goto end;}
                else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
                    {choice=2; goto end;}
                else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
                    {choice=3; goto end;}
                else if (580<event.button.x&&event.button.x<792&&594<event.button.y&&event.button.y<654)
                    {choice=4; goto end;}

        }
        else if (event.type==SDL_MOUSEMOTION )
        {
            if (580<event.button.x&&event.button.x<792&&360<event.button.y&&event.button.y<422)
            {
                SDL_BlitSurface(easyhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&438<event.button.y&&event.button.y<500)
            {
                SDL_BlitSurface(mediumhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&516<event.button.y&&event.button.y<576)
            {
                SDL_BlitSurface(hardhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else if (580<event.button.x&&event.button.x<792&&594<event.button.y&&event.button.y<654)
            {
                SDL_BlitSurface(returnhover2,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }
            else
            {
                SDL_BlitSurface(cleandifficultyhover,NULL,SDL_GetWindowSurface(Window),&buttonposition);
                SDL_UpdateWindowSurface(Window);
            }

        }
    }
end:
    buttonsound(buttonclicksound);
    /*Libération d'espace mémoire*/
    SDL_FreeSurface(difficulty);
    SDL_FreeSurface(easyhover);
    SDL_FreeSurface(mediumhover);
    SDL_FreeSurface(hardhover);
    SDL_FreeSurface(returnhover2);
    SDL_FreeSurface(cleandifficultyhover);
    return(choice);

}


/* Option to quit/replay */
int isreturntohome(int *backfromgame, SDL_Event event)
{
    if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE )
    {
        *backfromgame=TRUE;
        return 1;
    }

    if  ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT )
    {
        if (1172<event.button.x&&event.button.x<1275&&53<event.button.y&&event.button.y<176)
            return 0;
    }
}

