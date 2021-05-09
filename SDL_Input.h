#ifndef SDL_INPUT_H_INCLUDED
#define SDL_INPUT_H_INCLUDED

#include "all.h"

SDL_InputBox* SDL_CreateInputBox(SDL_Rect,SDL_Color,SDL_Color,size_t,size_t,size_t,char*,int,int);
void SDL_FreeInputBox(SDL_InputBox*);
int SDL_GetInputBoxState(SDL_InputBox*,SDL_Event);
void SDL_RefreshInputBox(SDL_InputBox*,SDL_Surface*);



#endif // SDL_INPUT_H_INCLUDED
