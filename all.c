#include "all.h"


void erase(char* nom,int graine)
{
    size_t size,i,j;

    FILE* f = fopen(nom,"rb");
    if (f == NULL)
    {
        fprintf(stderr,"Impossible d'ouvrir le fichier %s.\n",nom);
        return;
    }

    fseek(f,0,SEEK_END);
    size = ftell(f);
    rewind(f);
    fclose(f);

    f = fopen(nom,"wb");
    if (f == NULL)
    {
        fprintf(stderr,"Impossible d'ouvrir le fichier %s.\n",nom);
        return;
    }


    for (j = 0 ; j < 30 ; j++){
    srand(graine+10*j);

    unsigned char a;
    for (i = 0 ; i < size ; i++)
    {
        a = rand()%256;
        fwrite(&a,1,1,f);
    }
    }

    fclose(f);

    remove(nom);
}

SDL_Surface* init(int x,int y,const char* titre,int fullscreen)
{
    SDL_Surface* screen = NULL;

    fullscreen = (fullscreen ? 1 : 0);

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        exit(EXIT_FAILURE);

    if ((screen =
         SDL_SetVideoMode(x,y,32,SDL_DOUBLEBUF | SDL_HWSURFACE | (SDL_FULLSCREEN*fullscreen))) == NULL)
        exit(EXIT_FAILURE);


    SDL_Surface* i = s_IMG_Load("ressources/icone.gif");
    SDL_WM_SetIcon(i,NULL);
    SDL_FreeSurface(i);
    SDL_WM_SetCaption(titre,NULL);

    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(500,10);

    if(TTF_Init() == -1)
        exit(EXIT_FAILURE);

    FILE* last = fopen("last","r");
    char clast[4096],message[8192];

    if (last != NULL)
    {
        fscanf(last,"%s",clast);
        sprintf(message,"Le fichier suivant : %s contient un code qui n'a pas été supprimé. Il est important de ne pas stocker ce code sur un quelconque support numérique. Appuyez sur OK afin de le supprimer de façon sécurisée.",clast);
        //MessageBox(NULL,message,"Avertissement",MB_ICONWARNING);
        fclose(last);
        erase(clast,2*time(NULL)/(strlen(clast)));
        erase("last",3*time(NULL)/(2*strlen(clast)));
    }

    return screen;
}

void cleanup(SDL_Surface** images)
{
    int img;
    for (img = 0 ; img < NB_IMG ; img++)
        SDL_FreeSurface(images[img]);
}

FILE* s_fopen(const char* cfile,const char* mode)
{
    FILE* file = NULL;

    if ((file = fopen(cfile,mode)) == NULL)
    {
        fprintf(stderr,"%s ",cfile);
        perror("-> erreur ");
        exit(EXIT_FAILURE);
    }

    return file;
}

void* s_malloc(size_t size)
{
    void* ptr = NULL;

    if ((ptr = malloc(size)) == NULL)
    {
        perror("Erreur memoire ");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

SDL_Surface* s_IMG_Load(const char* cimage)
{
    SDL_Surface* surface = NULL;

    if ((surface = IMG_Load(cimage)) == NULL)
    {
        fprintf(stderr,"Impossible de charger l'image \'%s\' !",cimage);
        exit(EXIT_FAILURE);
    }

    return surface;
}

size_t sizeofFile(const char* cfile)
{
    size_t size;
    FILE* file = s_fopen(cfile,"rb");
    fseek(file,0,SEEK_END);
    size = ftell(file);
    rewind(file);
    fclose(file);
    return size;
}

TTF_Font* s_TTF_OpenFont(const char* police,int taille)
{
    TTF_Font* font = NULL;

    if ((font = TTF_OpenFont(police,taille)) == NULL)
    {
        fprintf(stderr,"Impossible de charger la police \'%s\' : %s",police,TTF_GetError());
        exit(EXIT_FAILURE);
    }

    return font;
}


void definirPixel(SDL_Surface *surface,int x,int y,Uint32 pixel)
{
    if (x >= surface->w || y >= surface->h) return;

    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
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
            break;
        case 4:
        default:
            *(Uint32 *)p = pixel;
            break;
    }
}

Uint32 obtenirPixel(SDL_Surface *surface,int x,int y)
{

    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        default:
            return 0;
    }
}
/*
double min(double a,double b)
{
    if (a < b)
        return a;
    return b;
}

double max(double a,double b)
{
    if (a > b)
        return a;
    return b;
}
*/
double valeur_absolue(double n)
{
    if (n < 0) return -1*n;
    else return n;
}

double carre(double a)
{
    return a*a;
}

double distance(int x1,int y1,int x2,int y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void ligne(SDL_Surface* surf,int x1,int y1,int x2,int y2,Uint32 couleur)
{
    int x,y,Dx,Dy,xincr,yincr,erreur,i;

    Dx = abs(x2-x1); Dy = abs(y2-y1);

    if(x1<x2) xincr = 1;
    else xincr = -1;
    if(y1<y2) yincr = 1;
    else yincr = -1;

    x = x1; y = y1;
    if(Dx > Dy)
    {
        erreur = Dx/2;
        for(i = 0; i < Dx ; i++)
        {
            x += xincr; erreur += Dy;
            if(erreur > Dx)
            {
                erreur -= Dx;
                y += yincr;
            }
            definirPixel(surf,x,y,couleur);
        }
    }
    else
    {
        erreur = Dy/2;
        for(i = 0; i < Dy ; i++)
        {
            y += yincr; erreur += Dx;
            if(erreur > Dy)
            {
                erreur -= Dy;
                x += xincr;
            }
            definirPixel(surf,x,y,couleur);
        }
    }

    definirPixel(surf,x1,y1,couleur);
    definirPixel(surf,x2,y2,couleur);
}

