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
    /* Set the text colorkey and convert to display format */
  if ( SDL_SetColorKey(texte, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0) < 0 )
    SDEBUG(W2,"Erreur: Couldn't set text colorkey: "<<SDL_GetError()<<"\n");
  SDL_BlitSurface(texte, NULL, screen, &r);
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
   //Tracer les lignes point par point ou utiliser une lib comme SDL_gfx, non portable ?
   static SDL_Surface *tracage = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY,800,600,24,0,0,0,0);
   static SDL_Rect *tout = (new SFRect(0,0,800,600))->toSDL_Rect();
   SDL_SetColorKey(tracage, SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(tracage->format,4,4,4));
   SDL_FillRect(tracage, tout , SDL_MapRGB(tracage->format,4,4,4));
   SDL_FillRect(tracage, rcRect.toSDL_Rect() , SDL_MapRGB(tracage->format,255,255,0));
   SFRect *rcRectPetit = new SFRect(rcRect.Left()+1,rcRect.Top()+1, rcRect.Right()-1, rcRect.Bottom()-1);
   SDL_FillRect(tracage, rcRectPetit->toSDL_Rect() , SDL_MapRGB(tracage->format,4,4,4));
	SDL_BlitSurface(tracage, rcRect.toSDL_Rect(), screen, rcRect.toSDL_Rect());
}


///////////////////////////////////////////////////////////////////////////////
void
SFDrawDCRectangle(SFRect& rcRect, SFColor& color)
{
  //TODO 2020 : empty...
}


///////////////////////////////////////////////////////////////////////////////
void SFDrawSurface(int x0, int y0, SFRect& rcRect)  // Ecriture différée
{  //Copie un bout de l'image globale vers le buffer d'affichage
  SDL_Rect r;
  r.x=x0, r.y=y0;
  SDL_BlitSurface(image, rcRect.toSDL_Rect(), screen, &r);
}

