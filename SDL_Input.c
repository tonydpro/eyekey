#include "SDL_Input.h"

SDL_InputBox* SDL_CreateInputBox(SDL_Rect pos,SDL_Color bmp1,SDL_Color bmp2,size_t sizeText,size_t buffSize,size_t taille,char* font,int decal,int mode)
{
    SDL_InputBox* box = NULL;

    box = s_malloc(128);
    box->font = s_TTF_OpenFont(font,sizeText);
    box->buffer = s_malloc(buffSize+100);
    box->buffer2 = s_malloc(buffSize+100);
    box->bufflen = 0;
    box->buffer[0] = '\0';
    box->pos = pos;
    box->bmp1 = bmp1;
    box->bmp2 = bmp2;
    box->state = 0;
    box->sizeText = sizeText;
    box->buffSize = buffSize;
    box->decal = decal;
    box->mode = mode;
    box->taille = taille;

    return box;
}

void SDL_FreeInputBox(SDL_InputBox* box)
{
    TTF_CloseFont(box->font);
    free(box->buffer);
    free(box->buffer2);
    free(box);
}

int SDL_GetInputBoxState(SDL_InputBox* obj,SDL_Event event)
{
    obj->bufflen = strlen(obj->buffer);
    if (event.type == SDL_KEYDOWN && obj->state && (obj->bufflen < obj->buffSize || event.key.keysym.unicode == '\b') && event.key.keysym.unicode)
    {

        if (event.key.keysym.unicode == '\b' && obj->bufflen)
        {
            obj->bufflen--;
            obj->buffer[obj->bufflen] = '\0';
            obj->buffer2[obj->bufflen] = '\0';
        }
        else if (!iscntrl(event.key.keysym.unicode))
        {
            if (obj->mode == 0)
                obj->buffer[obj->bufflen] = event.key.keysym.unicode;
            else
            {
                obj->buffer[obj->bufflen] = '*';
                obj->buffer2[obj->bufflen] = event.key.keysym.unicode;
            }
            obj->bufflen++;
        }
        return 1;
    }
    else if ((event.type == SDL_MOUSEBUTTONDOWN  && truePos(obj->pos,event.button.x,event.button.y)) || (truePos(obj->pos,event.button.x,event.button.y) && obj->state == 1 && event.type != SDL_MOUSEBUTTONUP))
    {
        obj->state = 1;
        return 1;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN  && !(truePos(obj->pos,event.button.x,event.button.y)))
    {
        obj->state = 0;
        return 0;
    }
    return 1;
}

void SDL_RefreshInputBox(SDL_InputBox* obj,SDL_Surface* image)
{
    long t = time(NULL);

    SDL_Surface* txt = NULL,*bord = NULL,*bord2,*mur = NULL;
    SDL_Rect posBord = SDL_SetRect(obj->pos.x,obj->pos.y,obj->pos.w,obj->pos.h);

    mur = SDL_CreateRGBSurface(SDL_HWSURFACE,obj->pos.w,obj->sizeText+6,32,0,0,0,0);
    bord = SDL_CreateRGBSurface(SDL_HWSURFACE,obj->pos.w+2,2,32,0,0,0,0);
    bord2 = SDL_CreateRGBSurface(SDL_HWSURFACE,2,obj->sizeText+6,32,0,0,0,0);

    SDL_FillRect(mur,NULL,SDL_MapRGB(image->format,obj->bmp2.r,obj->bmp2.g,obj->bmp2.b));
    SDL_BlitSurface(mur,NULL,image,&(obj->pos));

    if (t % 2 && obj->state)
        strcat(obj->buffer,"|");
    else
        strcat(obj->buffer," ");

    if (obj->taille <= obj->bufflen)
    {
        char buffer2[obj->buffSize+10];
        memset(buffer2,0,obj->buffSize+10);
        strncpy(buffer2,obj->buffer+obj->bufflen-obj->taille-1,obj->taille+1);
        txt = TTF_RenderText_Shaded(obj->font,buffer2,obj->bmp1,obj->bmp2);
    }
    else
        txt = TTF_RenderText_Shaded(obj->font,obj->buffer,obj->bmp1,obj->bmp2);

    obj->buffer[obj->bufflen] = '\0';

    posBord.y += obj->decal;
    posBord.x += 3;
    SDL_BlitSurface(txt,NULL,image,&posBord);
    posBord.y -= obj->decal;
    posBord.x -= 3;

    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,0,0,0));
    SDL_BlitSurface(bord,NULL,image,&posBord);
    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,235,235,235));
    posBord.y += obj->sizeText+6;
    SDL_BlitSurface(bord,NULL,image,&posBord);

    posBord.y -= obj->sizeText+6;
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,0,0,0));
    SDL_BlitSurface(bord2,NULL,image,&posBord);
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,235,235,235));
    posBord.x += obj->pos.w;
    SDL_BlitSurface(bord2,NULL,image,&posBord);

    SDL_FreeSurface(txt);
    SDL_FreeSurface(bord);
    SDL_FreeSurface(bord2);
    SDL_FreeSurface(mur);
}
