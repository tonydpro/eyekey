#ifndef SDL_BUTTON_H_INCLUDED
#define SDL_BUTTON_H_INCLUDED

#include "all.h"

/* Création et allocations des boutons */

SDL_Button* SDL_CreateButton(SDL_Rect,char*,char*,size_t,SDL_Color,SDL_Color);
SDL_ButtonIMG* SDL_CreateButtonIMG(SDL_Rect,SDL_Color,char*);

/* Suppression des boutons */

void SDL_FreeButton(SDL_Button*);
void SDL_FreeButtonIMG(SDL_ButtonIMG*);


/* Affichage et actualisation des boutons */

void SDL_RefreshButton(SDL_Button*,SDL_Surface*);
void SDL_RefreshButtonIMG(SDL_ButtonIMG*,SDL_Surface*);


/* Etat du bouton */

int SDL_GetButtonState(SDL_Button*,SDL_Event);
int SDL_GetButtonIMGState(SDL_ButtonIMG*,SDL_Event);


#endif // SDL_BUTTON_H_INCLUDED
