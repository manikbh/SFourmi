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

#ifdef WIN32
# include "WinInterface.h"
#endif

#ifdef GTK_Linux
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif

//SDL : includes dans SFourmis.h

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"

///////////////////////////////////////////////////////////////////////////////
//  Définitions communes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Procédures partagées fonctionnant sous win32, gtk et SDL
///////////////////////////////////////////////////////////////////////////////
void
SFDrawMode(int Mode)
{
#ifdef WIN32
  DrawMode = Mode;
#endif
#ifdef GTK_Linux
#endif
//Rien pour SDL
}


void
SFDisplay(int X, int Y, char* chaine)
{
#ifdef WIN32
  CRect rcWind;
  TextOut(ghdc, rcWind.left = X, rcWind.top = Y, chaine, lstrlen(chaine));
#endif
#ifdef GTK_Linux
  gdk_draw_string(FlipPM,police1,jauneGC,X,Y,chaine);
#endif
#ifdef SF_SDL
	SDL_Rect r;
	r.x=X;
	r.y=Y;
	SDL_Surface *texte=TTF_RenderText_Solid(SFfont, chaine, fontColor);
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
#endif
}


///////////////////////////////////////////////////////////////////////////////
void
SFTextColor(int r, int g, int b)
{
#ifdef WIN32
  SetTextColor(ghdc, RGB(r,g,b));
#endif
#ifdef GTK_Linux
  GDKColor1.red = r*256;
  GDKColor1.green = g*256;
  GDKColor1.blue = b*256;
  gdk_gc_set_foreground(GDKGC1,&GDKColor1);
#endif
#ifdef SF_SDL
	fontColor.r=r;
	fontColor.g=g;
	fontColor.b=b;
#endif
}

#ifdef WIN32
static CBrush BB1(0x00F0021C);
#endif

///////////////////////////////////////////////////////////////////////////////
void
SFDrawSingleRectangle(SFRect& rcRect, SFColor& color)
  // Fait pour dessiner qu'un rectangle
{
#ifdef WIN32
  HDC hdc;
  lpDDSPrimary->GetDC(&hdc);
  BB1.CreateSolidBrush(RGB (color.Red (), color.Green (), color.Blue ()));
  FrameRect(hdc, rcRect.toCRect (),BB1);
  lpDDSPrimary->ReleaseDC(hdc);
#endif
#ifdef GTK_Linux
  // Me dire comment procéder !
#endif
#ifdef SF_SDL
   //Tracer les lignes point par point ou utiliser une lib comme SDL_gfx, non portable ?
   static SDL_Surface *tracage = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY,800,600,24,0,0,0,0);
   static SDL_Rect *tout = (new SFRect(0,0,800,600))->toSDL_Rect();
   SDL_SetColorKey(tracage, SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(tracage->format,4,4,4));
   SDL_FillRect(tracage, tout , SDL_MapRGB(tracage->format,4,4,4));
   SDL_FillRect(tracage, rcRect.toSDL_Rect() , SDL_MapRGB(tracage->format,255,255,0));
   SFRect *rcRectPetit = new SFRect(rcRect.Left()+1,rcRect.Top()+1, rcRect.Right()-1, rcRect.Bottom()-1);
   SDL_FillRect(tracage, rcRectPetit->toSDL_Rect() , SDL_MapRGB(tracage->format,4,4,4));
	SDL_BlitSurface(tracage, rcRect.toSDL_Rect(), screen, rcRect.toSDL_Rect());

#endif
}


///////////////////////////////////////////////////////////////////////////////
void
SFDrawDCRectangle(SFRect& rcRect, SFColor& color)
  // Fait quand déjà GetDC
{
#ifdef WIN32
  BB1.CreateSolidBrush(RGB (color.Red (), color.Green (), color.Blue ()));
  FrameRect(ghdc, rcRect.toCRect (),BB1);
#endif
#ifdef GTK_Linux
  // Me dire comment procéder !
#endif
#ifdef SF_SDL

#endif
}


///////////////////////////////////////////////////////////////////////////////
void SFSelectFont(BYTE n)
{

#ifdef WIN32
  switch(n)
  {
    case 1: font.genre = hFont1; break;
    case 2: font.genre = hFont2; break;
    case 3: font.genre = hFont3; break;
    case 4: font.genre = hFont4; break;
    default:font.genre = hFont1;
  }
  SetBkMode(ghdc, TRANSPARENT);
  SelectObject(ghdc,font.genre);
#endif
}


///////////////////////////////////////////////////////////////////////////////
void SFGetDC()
{
#ifdef WIN32
  lpDDSPrimary->GetDC(&ghdc);

#endif
}


///////////////////////////////////////////////////////////////////////////////
void SFReleaseDC()
{
#ifdef WIN32
  lpDDSPrimary->ReleaseDC(ghdc);
#endif
}


///////////////////////////////////////////////////////////////////////////////
void SFDrawSurface(int x0, int y0, SFRect& rcRect)  // Ecriture différée
{  //Copie un bout de l'image globale vers le buffer d'affichage
#ifdef WIN32
  HRESULT  ddrval;
  while(1)
  {
    ddrval = lpDDSBack->BltFast(x0, y0, lpDDSOne,
	&(rcRect.toCRect ()), DrawMode);
    if(ddrval == DD_OK)
      break;
    if(ddrval == DDERR_SURFACELOST)
    {
      ddrval = restoreAll();
      if(ddrval != DD_OK)
	return;
    }
    if(ddrval != DDERR_WASSTILLDRAWING)
      return;
  }
  if(ddrval != DD_OK)
    SDEBUG(W5,"DD isnt OK\n");
#endif
#ifdef GTK_Linux

  gdk_pixbuf_render_to_drawable_alpha(MainPixbuf,
      FlipPM,rcRect.Left(),rcRect.Top(),x0,y0,rcRect.Right() - rcRect.Left(),
      rcRect.Bottom() - rcRect.Top(),GDK_PIXBUF_ALPHA_BILEVEL,
      1,GDK_RGB_DITHER_NORMAL, 0,0);
#endif
#ifdef SF_SDL
	SDL_Rect r;
	r.x=x0, r.y=y0;
	SDL_BlitSurface(image, rcRect.toSDL_Rect(), screen, &r);
#endif
}


///////////////////////////////////////////////////////////////////////////////
// Ecriture directe
void SFFastDrawSurface(unsigned long x0,unsigned long y0, SFRect& rcRect)
{
#ifdef WIN32
  HRESULT             ddrval;
  while(1)
  {
    ddrval = lpDDSPrimary->BltFast(x0, y0, lpDDSOne,
	&(rcRect.toCRect ()),DrawMode);
    if(ddrval == DD_OK)
      break;
    if(ddrval == DDERR_SURFACELOST)
    {
      ddrval = restoreAll();
      if(ddrval != DD_OK)
	return;
    }
    if(ddrval != DDERR_WASSTILLDRAWING)
      return;
  }
  if(ddrval != DD_OK)
    SDEBUG(W5,"DD isnt OK\n");
#endif
#ifdef GTK_Linux
  gdk_pixbuf_render_to_drawable_alpha(MainPixbuf,
      FlipPM,rcRect.Left(),rcRect.Top(),x0,y0,rcRect.Right() - rcRect.Left(),
      rcRect.Bottom() - rcRect.Top(),GDK_PIXBUF_ALPHA_BILEVEL,
      1,GDK_RGB_DITHER_NORMAL, 0,0);
#endif
#ifdef SF_SDL
	SDL_Rect r;
	r.x=x0, r.y=y0;
	SDL_BlitSurface(image, rcRect.toSDL_Rect(), screen, &r); 
#endif
}

