#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include "variables.h"

/*position de l'arriere plan et les buttons */
SDL_Rect position = { 0,0, 0, 0};
SDL_Rect buttonposition = { 577, 357, 0, 0};
SDL_Rect buttonposition2 = { 353, 305, 0, 0};
SDL_Rect humanchoice = { 430, 437, 0, 0};
SDL_Rect cpuchoice = { 800, 437, 0, 0};
SDL_Rect message = { 383, 355, 0, 0};
SDL_Rect playbuttonlocation = { 601, 614, 0, 0};
SDL_Rect playerturn = { 60,630, 0, 0};

float d1=76; /* distance horizentale entre les centres de deux hexagonaux */
float d2=65.5; /*distance verticale entre les centres de deux hexagonaux */

point firsthex={210,118}; /* Le centre de la première case (tout gauche en haut) */


int hex_line(int hexagon) /* la ligne auquelle apprtient un pion */
{
    if (0<=hexagon&&hexagon<=8)
        return 0;
    else if (9<=hexagon&&hexagon<=17)
        return 1;
    else if (18<=hexagon&&hexagon<=26)
        return 2;
    else if (27<=hexagon&&hexagon<=35)
        return 3;
    else if (36<=hexagon&&hexagon<=44)
        return 4;
    else if (45<=hexagon&&hexagon<=53)
        return 5;
    else if (54<=hexagon&&hexagon<=62)
        return 6;
    else if (63<=hexagon&&hexagon<=71)
        return 7;
    else
        return 8;

}

point hex_center(int hexagon) /* cordoonnées de centre de hexagon */
{
    point center;
    center.x=firsthex.x+hex_line(hexagon)*(d1/2)+(hexagon%9)*d1-0.5;
    center.y=firsthex.y+hex_line(hexagon)*d2;
    return center;
}

float aucarre(float x) /* calcule le carre d'un nombre */
{
    return x*x;
}

int hex_clicked(point click) /* récupérer l'hexagone cliqué */
{
    float r;
    int hexagon;
    r=d1/2;
    for (hexagon=0; hexagon<=80; hexagon++)
    {
        point center=hex_center(hexagon);
        if (aucarre(center.x-click.x)+aucarre(center.y-click.y)<=aucarre(r))
            return hexagon;
    }
    return -1;
}

int hex_finder(SDL_Event event) /* récupérer l'hexagone cliqué */
{
    point click={event.button.x,event.button.y};
    return hex_clicked(click);
}

SDL_Rect pos_finder(int hexagon) /* récupérer la position ou on mettra l'image de pion */
{
    SDL_Surface* white_hex = IMG_Load("images/white_hex.png");
    SDL_Rect srcrect;
    point center=hex_center(hexagon);
    srcrect.x=center.x - white_hex->w/2;
    srcrect.y=center.y - white_hex->h/2;
    srcrect.w=0;
    srcrect.h=0;

    return srcrect;
}


void buttonsound(Mix_Chunk *buttonclicksound) /* effets sonores */
{
    Mix_VolumeMusic(128);
    Mix_PlayChannel(-1, buttonclicksound, 0);
    Mix_VolumeMusic(50);
}
