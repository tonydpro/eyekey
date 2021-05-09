#include "SDL_Button.h"

SDL_Button* SDL_CreateButton(SDL_Rect pos,char* caption,char* font,size_t textSize,SDL_Color bmp1,SDL_Color bmp2)
{
    SDL_Button* obj = NULL;

    obj = s_malloc(sizeof(SDL_Button));

    obj->state = 0;
    obj->signal = 0;
    obj->pos = pos;
    obj->bmp1 = bmp1;
    obj->bmp2 = bmp2;
    obj->textSize = textSize;
    obj->font = s_TTF_OpenFont(font,textSize);
    obj->caption = s_malloc(strlen(caption)+1);

    strcpy(obj->caption,caption);

    return obj;
}

SDL_ButtonIMG* SDL_CreateButtonIMG(SDL_Rect pos,SDL_Color fond,char* picturePath)
{
    SDL_ButtonIMG* obj = NULL;

    obj = s_malloc(sizeof(SDL_ButtonIMG));
    obj->pos = pos;
    obj->fond = fond;
    obj->picture = s_IMG_Load(picturePath);

    return obj;
}

void SDL_FreeButton(SDL_Button* obj)
{
    TTF_CloseFont(obj->font);
    free(obj->caption);
    free(obj);
}

void SDL_FreeButtonIMG(SDL_ButtonIMG* obj)
{
    SDL_FreeSurface(obj->picture);
    free(obj);
}

void SDL_RefreshButton(SDL_Button* obj,SDL_Surface* image)
{
    SDL_Surface* txt = NULL,*bord = NULL,*bord2;
    txt = TTF_RenderText_Shaded(obj->font,obj->caption,obj->bmp1,obj->bmp2);

    if (obj->state == 1)
    {
        obj->pos.x += 2;
        SDL_BlitSurface(txt,NULL,image,&(obj->pos));
        obj->pos.x -= 2;
    }
    else
        SDL_BlitSurface(txt,NULL,image,&(obj->pos));

    SDL_Rect posBord = SDL_SetRect(obj->pos.x,obj->pos.y,obj->pos.w,obj->pos.h);
    bord = SDL_CreateRGBSurface(SDL_HWSURFACE,txt->w+2,2,32,0,0,0,0);
    bord2 = SDL_CreateRGBSurface(SDL_HWSURFACE,2,txt->h,32,0,0,0,0);

    int type1,type2;

    if (!obj->state)
    {
        type1 = 235;
        type2 = 0;
    }
    else
    {
        type1 = 0;
        type2 = 235;
    }

    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,type1,type1,type1));
    SDL_BlitSurface(bord,NULL,image,&posBord);
    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,type2,type2,type2));
    posBord.y += txt->h;
    SDL_BlitSurface(bord,NULL,image,&posBord);

    posBord.y -= txt->h;
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,type1,type1,type1));
    SDL_BlitSurface(bord2,NULL,image,&posBord);
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,type2,type2,type2));
    posBord.x += txt->w;
    SDL_BlitSurface(bord2,NULL,image,&posBord);

    SDL_FreeSurface(txt);
    SDL_FreeSurface(bord);
    SDL_FreeSurface(bord2);
}

void SDL_RefreshButtonIMG(SDL_ButtonIMG* obj,SDL_Surface* image)
{
    SDL_Surface *bord = NULL,*bord2;

    if (obj->state == 1)
    {
        obj->pos.x += 2;
        SDL_BlitSurface(obj->picture,NULL,image,&(obj->pos));
        obj->pos.x -= 2;
    }
    else
        SDL_BlitSurface(obj->picture,NULL,image,&(obj->pos));

    SDL_Rect posBord = SDL_SetRect(obj->pos.x,obj->pos.y,obj->pos.w,obj->pos.h);
    bord = SDL_CreateRGBSurface(SDL_HWSURFACE,obj->picture->w+2,2,32,0,0,0,0);
    bord2 = SDL_CreateRGBSurface(SDL_HWSURFACE,2,obj->picture->h,32,0,0,0,0);

    int type1,type2;

    if (!obj->state)
    {
        type1 = 235;
        type2 = 0;
    }
    else
    {
        type1 = 0;
        type2 = 235;
    }

    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,type1,type1,type1));
    SDL_BlitSurface(bord,NULL,image,&posBord);
    SDL_FillRect(bord,NULL,SDL_MapRGB(image->format,type2,type2,type2));
    posBord.y += obj->picture->h;
    SDL_BlitSurface(bord,NULL,image,&posBord);

    posBord.y -= obj->picture->h;
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,type1,type1,type1));
    SDL_BlitSurface(bord2,NULL,image,&posBord);
    SDL_FillRect(bord2,NULL,SDL_MapRGB(image->format,type2,type2,type2));
    posBord.x += obj->picture->w;
    SDL_BlitSurface(bord2,NULL,image,&posBord);

    SDL_FreeSurface(bord);
    SDL_FreeSurface(bord2);
}


int SDL_GetButtonState(SDL_Button* obj,SDL_Event event)
{
    obj->signal = 0;
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && truePos(obj->pos,event.button.x,event.button.y))
    {
        if (obj->state == 1 || !obj->state)
            obj->state = 1;
        return 1;
    }
    else if (truePos(obj->pos,event.button.x,event.button.y) && obj->state == 1 && event.type != SDL_MOUSEBUTTONUP)
    {
        obj->state = 1;
        return 1;
    }
    else
    {
        if (obj->state)
            obj->signal = 1;
        obj->state = 0;
        return 0;
    }
}

int SDL_GetButtonIMGState(SDL_ButtonIMG* obj,SDL_Event event)
{
    obj->signal = 0;
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && truePos(obj->pos,event.button.x,event.button.y))
    {
        if (obj->state == 1 || !obj->state)
            obj->state = 1;
        return 1;
    }
    else if (truePos(obj->pos,event.button.x,event.button.y) && obj->state == 1 && event.type != SDL_MOUSEBUTTONUP)
    {
        obj->state = 1;
        return 1;
    }
    else
    {
        if (obj->state)
            obj->signal = 1;
        obj->state = 0;
        return 0;
    }
}
