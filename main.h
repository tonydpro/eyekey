#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "chiffrage.h"
#include "escapi.h"
#include "SDL_Interface.h"
#include "all.h"

#define noir        0x000000
#define blanc       0xffffff

Uint32 seuil_blanc;
int corrects = 0;

void degrade(SDL_Surface*);
void actualiser(SDL_Event,SDL_InputBox*,SDL_InputBox*,SDL_InputBox*,SDL_Surface*);
void xor_crypter(unsigned char*,unsigned char,size_t);
void parcourir(SDL_InputBox*,SDL_Surface*);
int mdp_ok(size_t);
void vigenere(unsigned char*,unsigned char*,size_t,size_t,int);
void afficher_console(SDL_Surface*,char* console);
void cryptage_fichier(char*,char*,int,char*);
unsigned char* charger_fichier(char*,size_t*);
void enregistrer_fichier(unsigned char*,char*,size_t);
void afficher(SDL_Surface* image, Uint32 pixel[240][320],struct SimpleCapParams capture,int mode,int sens);
struct SimpleCapParams init_camera(size_t,size_t);
int balance_des_blancs(SDL_Surface* image,Uint32 pixel[240][320],struct SimpleCapParams capture,SDL_InputBox*,SDL_InputBox*,SDL_InputBox*,SDL_Button*,TTF_Font*);
void pause(void);
int couleur(Uint32);
int correct(int,int,int,int,Uint32 pixel[240][320],SDL_Surface*,TTF_Font*);
int correct2(int,int,int,int,Uint32[240][320],double);
void sauv_blanc(void);
void charger_blanc(void);

void degrade(SDL_Surface* image)
{
    int i,j;
    for (i = 0 ; i < 320 ; i++)
    {
        for (j = 240 ; j < HAUTEUR ; j++)
            definirPixel(image,i,j,0xbbbbbb);
    }
}

void actualiser(SDL_Event event,SDL_InputBox* InputBox,SDL_InputBox* InputBox2,SDL_InputBox* InputBox3,SDL_Surface* image)
{
    SDL_RefreshInputBox(InputBox,image);
    SDL_GetInputBoxState(InputBox,event);

    SDL_GetInputBoxState(InputBox2,event);
    SDL_RefreshInputBox(InputBox2,image);

    SDL_GetInputBoxState(InputBox3,event);
    SDL_RefreshInputBox(InputBox3,image);

    if (InputBox->state == 1)
        afficher_console(image,"Enter the name of the file to encrypt / decrypt");

    if (InputBox2->state == 1)
        afficher_console(image,"Enter the path where the result will be stored");

    if (InputBox3->state == 1)
        afficher_console(image,"Enter a 5 to 15 character password");
}

void xor_crypter(unsigned char* texte,unsigned char cle,size_t taille)
{
    size_t i;
    for (i = 0 ; i < taille ; i++)
        texte[i] ^= cle;
}

int mdp_ok(size_t taille)
{
    if (taille < 5 || taille > 15)
        return 0;

    return 1;
}

void vigenere(unsigned char* texte,unsigned char* cle,size_t taille,size_t taille_cle,int mode)
{
    size_t c,c2;
    for (c = 0, c2 = 0 ; c < taille ; c++)
    {
        if (mode == 0)
            texte[c] = (texte[c] + cle[c2]) % 256;
        else
            texte[c] = (texte[c] - cle[c2]) % 256;

        c2++;
        if (c2 >= taille_cle)
            c2 = 0;
    }
}

void afficher_console(SDL_Surface* image,char* console)
{
    TTF_Font* font = s_TTF_OpenFont("ressources/LinBiolinum_R_G.ttf",15);
    char console2[54];
    memset(console2,0,54);
    strncpy(console2,console,strlen(console));

    int i,j;
    for (i = 0 ; i < 320 ; i++)
    {
        for (j = 440 ; j < 480 ; j++)
            definirPixel(image,i,j,noir);
    }
    SDL_Rect postxt = {5,452,300,20};

    SDL_Surface* txt = NULL;
    txt = TTF_RenderText_Shaded(font,console2,BLANC,NOIR);
    SDL_BlitSurface(txt,NULL,image,&postxt);
    TTF_CloseFont(font);
    SDL_FreeSurface(txt);
    SDL_Flip(image);
}

void cryptage_fichier(char* nom_fichier,char* nom_destination,int action,char* cle)
{
    size_t taille_fichier = sizeofFile(nom_fichier),i = 0;
    unsigned char *cfichier = s_malloc(taille_fichier+10);

    FILE* fichier = s_fopen(nom_fichier,"rb"),*destination = s_fopen(nom_destination,"wb");

    for (i = 0 ; i < taille_fichier ; i++)
        fread(cfichier+i,1,1,fichier);

    if (action == 0)
        crypter(cfichier,cle,taille_fichier);
    else
        decrypter(cfichier,cle,taille_fichier);

    i = 0;
    for (i = 0 ; i < taille_fichier ; i++)
        fwrite(cfichier+i,1,1,destination);

    free(cfichier);
    fclose(fichier);
    fclose(destination);
}

unsigned char* charger_fichier(char* nom_fichier,size_t* taille_f)
{
    size_t taille_fichier = sizeofFile(nom_fichier),i = 0;
    *taille_f = taille_fichier;
    unsigned char *cfichier = s_malloc(taille_fichier+10);

    FILE* fichier = s_fopen(nom_fichier,"rb");

    for (i = 0 ; i < taille_fichier ; i++)
        fread(cfichier+i,1,1,fichier);

    fclose(fichier);

    return cfichier;
}

void enregistrer_fichier(unsigned char* cfichier,char* nom_fichier,size_t taille_fichier)
{
    size_t i = 0;

    FILE* fichier = s_fopen(nom_fichier,"wb");

    for (i = 0 ; i < taille_fichier ; i++)
        fwrite(cfichier+i,1,1,fichier);

    fclose(fichier);
}

int balance_des_blancs(SDL_Surface* image,Uint32 pixel[240][320],struct SimpleCapParams capture,SDL_InputBox* InputBox,SDL_InputBox* InputBox2,SDL_InputBox* InputBox3,SDL_Button* button3,TTF_Font* font)
{
    charger_blanc();
    SDL_Event event;
    SDL_Rect pn = {10,10,0,0};
    actualiser(event,InputBox,InputBox2,InputBox3,image);
    afficher_console(image,"Adjust the black and white contrast, then enter");

    while(event.type != SDL_KEYDOWN || event.key.keysym.sym != SDLK_RETURN)
    {
        if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            || event.type == SDL_QUIT)
            return 0;

        SDL_PollEvent(&event);

        afficher(image,pixel,capture,0,1);
        char bc[12];
        sprintf(bc,"Black : %d%%",100*seuil_blanc/256);
        SDL_Surface* b = TTF_RenderText_Blended(font,bc,JAUNE);
        SDL_BlitSurface(b,NULL,image,&pn);

        SDL_GetButtonState(button3,event);
        SDL_RefreshButton(button3,image);

        if (button3->state)
        {
            SDL_Surface* printable = s_IMG_Load("ressources/A4print.png");

            afficher_console(image,"Press a key on the keyboard");

            do
            {
                SDL_WaitEvent(&event);
                SDL_Delay(10);
            }while((event.type != SDL_KEYDOWN) || (event.key.keysym.unicode == 0));

            int graine = time(NULL)%event.key.keysym.unicode;

            afficher_console(image,"Press a button again");

            if (event.motion.x != 0 && event.motion.y != 0)
                graine = graine + (event.motion.x * event.motion.y);

            do
            {
                SDL_WaitEvent(&event);
                SDL_Delay(10);
            }while((event.type != SDL_KEYDOWN) || (event.key.keysym.unicode == 0));

            graine *= event.key.keysym.unicode;


            afficher_console(image,"Code calculation in progress...");

            srand(graine);

            int tableau[10][10],x,y,i,j;
            memset(tableau,0,sizeof(int)*100);

            for (y = 0 ; y < 10 ; y++)
                for (x = 9 ; x > -1 ; x--)
                    tableau[x][y] = rand()%2;

            tableau[0][0] = 1; tableau[0][1] = 0; tableau[1][0] = 0; tableau[1][1] = 0;
            tableau[9][9] = 1; tableau[9][8] = 0; tableau[8][9] = 0; tableau[8][8] = 0;
            tableau[0][9] = 1; tableau[0][8] = 0; tableau[1][9] = 0; tableau[1][8] = 0;
            tableau[9][0] = 1; tableau[8][0] = 0; tableau[9][1] = 0; tableau[8][1] = 0;


            for (y = 0 ; y < 10 ; y++)
                for (x = 9 ; x > -1 ; x--)
                    for (i = (15*x)+100; i < (15*x)+115 ; i++)
                        for (j = (15*y)+100 ; j < (15*y)+115 ; j++)
                            definirPixel(printable,i,j,((tableau[x][y] == 1) ? blanc : noir));


            if (SDL_SaveBMP(printable,"code.bmp") < 0)
            {
                afficher_console(image,"Error, try again or quit");
                fprintf(stderr,"Error saving code in code.bmp %s\n",SDL_GetError());
            }

            SDL_FreeSurface(printable);
            afficher_console(image,"The code.bmp file has been created");
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_DOWN && seuil_blanc > 0)
            {
                seuil_blanc--;
                SDL_Delay(50);
            }
            else if (event.key.keysym.sym == SDLK_UP && seuil_blanc < 255)
            {
                seuil_blanc++;
                SDL_Delay(50);
            }
        }

        SDL_Flip(image);
    }

    return 1;
}

void afficher(SDL_Surface* image, Uint32 pixel[240][320],struct SimpleCapParams capture,int mode,int sens)
{
    doCapture(0);
    while (!isCaptureDone(0));

    int i,j,k;
    for (i = 0 ; i < 320 ; i++)
    {
        for (j = 0 ; j < 240 ; j++)
        {
            if (sens == 0) k = i;
            else k = 319-i;
            pixel[j][i] = *(capture.mTargetBuf + (320*j) + i);
            if (mode == 0)
                definirPixel(image,k,j,couleur(pixel[j][i]));
            else
                definirPixel(image,k,j,pixel[j][i]);
        }
    }
}

int couleur(Uint32 pixel)
{
    Uint8 p[4];

    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
    }
    else
    {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
    }

    int moy = (p[0] + p[1] + p[2]) / 3,seuil_moy = 150,a = p[0],b = p[1],c = p[2];

    if ((p[0] > seuil_blanc || p[1] > seuil_blanc || p[2] > seuil_blanc)
        && (valeur_absolue(a - moy) < seuil_moy)
        && (valeur_absolue(b - moy) < seuil_moy)
        && (valeur_absolue(c - moy) < seuil_moy) && (moy > (int)seuil_blanc))
        return blanc;

    return noir;
}

int correct2(int i_debut,int i_fin,int j_debut,int j_fin,Uint32 pixel[240][320],double m)
{
    int i,j;
    double t = 0;
    for (i = i_debut ; i < i_fin ; i++)
        for (j = j_debut ; j < j_fin ; j++)
            t += (couleur(pixel[j][i]) == noir ? 1 : 0);
    t = t*100/169;

    return (t > m);
}

int correct(int i_debut,int i_fin,int j_debut,int j_fin,Uint32 pixel[240][320],SDL_Surface* image,TTF_Font* font)
{
    int i,j,couleur_case = 0;
    double t = 0;
    SDL_Surface* txt = NULL;
    char chiffre[] = {'0','\0','1','\0'};
    SDL_Rect position = {326-i_fin,j_debut+3,0,0};

    for (i = i_debut+5 ; i < i_fin-5 ; i++)
    {
        for (j = j_debut+5 ; j < j_fin-5 ; j++)
        {
            couleur_case = (couleur(pixel[j][i]) == noir ? 1 : 0);
            t += couleur_case;
            if (couleur_case == 0)
                txt = TTF_RenderText_Blended(font,chiffre,NOIR);
            else
                txt = TTF_RenderText_Blended(font,chiffre+2,VERT);
            SDL_BlitSurface(txt,NULL,image,&position);
        }
    }

    SDL_FreeSurface(txt);
    return (t > 20);
}

struct SimpleCapParams init_camera(size_t x,size_t y)
{
    int devices = setupESCAPI();

    if (!devices)
    {
        printf("ESCAPI initialization failure or no devices found.\n");
        exit(EXIT_FAILURE);
    }

    struct SimpleCapParams capture;
    capture.mWidth = x;
    capture.mHeight = y;
    capture.mTargetBuf = s_malloc(sizeof(int)*x*y);

    if (!initCapture(0,&capture))
    {
        printf("Capture failed - device may already be in use.\n");
        exit(EXIT_FAILURE);
    }

    return capture;
}

void pause(void)
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_RETURN
                || event.key.keysym.sym == SDLK_ESCAPE)
                    continuer = 0;
                break;
            default:
                break;
        }
    }
}

void sauv_blanc(void)
{
    FILE* f = s_fopen("blanc.txt","w");
    fprintf(f,"%d",seuil_blanc);
    fclose(f);
}

void charger_blanc(void)
{
    seuil_blanc = 50;
    FILE* f = fopen("ressources/blanc.txt","r");
    if (f != NULL)
        fscanf(f,"%d",&seuil_blanc);
    else
        sauv_blanc();
    fclose(f);
}


#endif // MAIN_H_INCLUDED
