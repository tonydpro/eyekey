#include "SDL_Box.h"

int truePos(SDL_Rect objPos,int x,int y)
{
    return (x < (objPos.w + objPos.x) && x > objPos.x && y > objPos.y && y < (objPos.h + objPos.y));
}

SDL_Rect SDL_SetRect(int x,int y,int w,int h)
{
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return rect;
}
