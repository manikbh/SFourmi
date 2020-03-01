/*
** GraphXtools.cpp
**
** Made by (Ghost in the Shell)
** Login   <dodeskaden@Z>
** 
** Started on  Fri Jul 12 23:51:07 2002 Ghost in the Shell
** Last update Fri Jul 12 23:51:17 2002 Ghost in the Shell
*/

#include "SFourmis.h"

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
//  Définitions communes
///////////////////////////////////////////////////////////////////////////////


void
SFDisplay(int X, int Y, std::string chaine)
{
  SDL_Rect r;
  r.x=X;
  r.y=Y;
  SDL_Surface *texte=TTF_RenderText_Solid(SFfont, chaine.c_str(), fontColor);
  if(texte==NULL){
    SDEBUG(W2,"Erreur creation du texte\n");
    return;
  }
  r.w = texte->w;
  r.h = texte->h;
    /* Set the text colorkey and convert to display format */
  if ( SDL_SetColorKey(texte, SDL_TRUE, 0) < 0 )
    SDEBUG(W2,"Erreur: Couldn't set text colorkey: "<<SDL_GetError()<<"\n");
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(sdlRenderer, texte);
  //SDL_BlitSurface(texte, NULL, screen, &r);
  SDL_RenderCopy(sdlRenderer, textTexture, NULL, &r);
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(texte);
  SDEBUG(W8,"Texte affiche\n");
}


///////////////////////////////////////////////////////////////////////////////
void
SFTextColor(int r, int g, int b)
{
  fontColor.r=r;
  fontColor.g=g;
  fontColor.b=b;
}

///////////////////////////////////////////////////////////////////////////////
void
SFDrawSingleRectangle(SFRect& rcRect, SFColor& color)
  // Fait pour dessiner qu'un rectangle
{
   SDL_Rect rect = rcRect.toSDL_Rect();
   SDL_SetRenderDrawColor(sdlRenderer, color.Red(), color.Green(), color.Blue(), 255);
   SDL_RenderDrawRect(sdlRenderer, &rect);
}


///////////////////////////////////////////////////////////////////////////////
void SFDrawSurface(int x0, int y0, SFRect& rcRect)  // Ecriture différée
{  //Copie un bout de l'image globale vers le buffer d'affichage
  SDL_Rect sdlRect = rcRect.toSDL_Rect();
  SDL_Rect r;
  r.x=x0; r.y=y0;
  r.w=sdlRect.w;
  r.h=sdlRect.h;
  SDL_RenderCopy(sdlRenderer, sdlImageTexture, &sdlRect, &r);
  //SDL_BlitSurface(image, sdlRect, screen, &r);
}

