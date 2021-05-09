#ifndef ALL_H_INCLUDED
#define ALL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>


typedef struct SDL_InputBox SDL_InputBox;
struct SDL_InputBox
{
    SDL_Rect pos;
    SDL_Color bmp1,bmp2;
    int state,decal,mode;
    size_t sizeText,buffSize,bufflen,taille;
    TTF_Font* font;
    char* buffer;
    unsigned char*buffer2;
};

typedef struct SDL_Button SDL_Button;
struct SDL_Button
{
    SDL_Rect pos;
    char* caption;
    size_t textSize;
    TTF_Font* font;
    int state,signal;
    SDL_Color bmp1,bmp2;
};

typedef struct SDL_ButtonIMG SDL_ButtonIMG;
struct SDL_ButtonIMG
{
    SDL_Rect pos;
    SDL_Color fond;
    SDL_Surface* picture;
    int state,signal;
};

#include "SDL_box.h"
#include "SDL_Button.h"
#include "SDL_Input.h"

#define NB_IMG      3
#define LARGEUR     320
#define HAUTEUR     480
#define ecran       images[0]

void erase(char*,int);
SDL_Surface* init(int,int,const char*,int);
FILE* s_fopen(const char*,const char*);
void* s_malloc(size_t);
SDL_Surface* s_IMG_Load(const char*);
void cleanup(SDL_Surface**);
size_t sizeofFile(const char*);
TTF_Font* s_TTF_OpenFont(const char*,int);
void definirPixel(SDL_Surface*,int,int,Uint32);
Uint32 obtenirPixel(SDL_Surface*,int,int);
void ligne(SDL_Surface*,int,int,int,int,Uint32);
/*
double min(double,double);
double max(double,double);*/
double distance(int x1,int y1,int x2,int y2);
double valeur_absolue(double);
double carre(double);

#endif // ALL_H_INCLUDED
