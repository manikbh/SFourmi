//
// GraphXproc.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sat Feb  2 10:47:09 2002 Ghost in the Shell
// Last update Sat Oct 19 00:09:00 2002 Ghost in the Shell
//

/////////////////////////////////////////////////////////////////////////////////
//
//	PROCEDURES GRAPHIQUES
//
///////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"

#include "GraphXstruct.h"
#include "gGraphX.hh"
#include "GraphXproc.h"
#include "GraphXtools.h"

///////////////////////////////////////////////////////////////////////////////
void SelectMultiple()							  //dessine un rectangle de sélection
{
  SFColor	colors;
  SFRect	rc;
  
  rc.setSFRect(min(MouseXY[0],MouseXYD[0])
      ,min(MouseXY[1],MouseXYD[1])
      ,max(MouseXYD[0],MouseXY[0])
      ,max(MouseXYD[1],MouseXY[1]));

  colors.setSFColor(80, 159, 207);

  SFDrawSingleRectangle(rc, colors);

#ifdef GTK_Linux
  gdk_draw_rectangle((GdkDrawable *)BigFenetre,jauneGC,false,
      min(MouseXY[0],MouseXYD[0]),min(MouseXY[1],MouseXYD[1]),
      max(MouseXYD[0],MouseXY[0])-min(MouseXY[0],MouseXYD[0]),
      max(MouseXYD[1],MouseXY[1])-min(MouseXY[1],MouseXYD[1]));
#endif

}


///////////////////////////////////////////////////////////////////////////////
void SmallMap()//TODO 2020 remove globals and pass a SFourmiWorld by reference
{
  class SFRect 		rc(15,440,115,540);
  class SFColor		colors;
  int			PosX,PosY;
  static SFRect		Yellow(0, 825, 1, 826),
  			Vert(10,880,11,881),
			Red(78,880,79,881),
			Violet(365,709,366,710);

  colors.setSFColor(255, 255, 255);

  rc.setSFRect(LSMAP*ZSF.column/ZSF.This_room()->size.largeur+XSMAP,HSMAP*ZSF.line/ZSF.This_room()->size.hauteur+YSMAP
      ,LSMAP*(ZSF.column+20)/ZSF.This_room()->size.largeur+XSMAP,HSMAP*(ZSF.line+18)/ZSF.This_room()->size.hauteur+YSMAP);
  SFDrawSingleRectangle(rc, colors);
  SFDrawMode(DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);

  for (int ceclan = 0; ceclan < MData.nb_clan; ceclan++)
    for (int i = 0; i < MData.max_ants; i++)
      if ((les_clans[ceclan]->les_fourmis[i]->Statut() != NIHIL)
	  && (les_clans[ceclan]->les_fourmis[i]->Room == ZSF.This_room()))
      {
	PosX = LSMAP*les_clans[ceclan]->les_fourmis[i]->Xc
	  / ZSF.This_room()->size.largeur+XSMAP;
	PosY = HSMAP*les_clans[ceclan]->les_fourmis[i]->Yc
	  / ZSF.This_room()->size.hauteur+YSMAP;
	switch (les_clans[ceclan]->les_fourmis[i]->Statut())
	{
	  case OUVRIER:
	    SFFastDrawSurface(PosX, PosY, Yellow);
	    break;
	  case CITERNE:
	    SFFastDrawSurface(PosX, PosY, Red);
	    break;
	  case SENTINELLE:
	    SFFastDrawSurface(PosX, PosY, Violet);
	    break;
	  default:
	    break;
	}
      }
  //Ennemis :
  for (int i = 0; i < MData.max_ennemies; i++)
    if (les_ennemis[i] != NULL && les_ennemis[i]->Room == ZSF.This_room())
    {
      PosX = LSMAP * les_ennemis[i]->Xc / MData.length + XSMAP;
      PosY = HSMAP * les_ennemis[i]->Yc / MData.height + YSMAP;
      switch (les_ennemis[i]->Esp)
      {
	case CAFARD:
	  SFFastDrawSurface(PosX, PosY, Vert);
	  break;
	default:
	  SFFastDrawSurface(PosX, PosY, Vert);
	  break;
      }
    }
}

/**
 * Flip screen buffer
 */
void Flips(void)
{
  SDL_Flip(screen); //Affiche reellement en echangeant les buffers hardware
}


///////////////////////////////////////////////////////////////////////////////
void splash(void)
{
#ifdef WIN32
  CRect rcRect;
  HRESULT             ddrval;

  rcRect.left = 400;
  rcRect.top = 600;
  rcRect.right = 800;
  rcRect.bottom = 600+300;

  while(1)
  {
    ddrval = lpDDSPrimary->BltFast(160, 120, lpDDSTwo,
	&rcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
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
    return;
#endif
}


///////////////////////////////////////////////////////////////////////////////
void info_plus(void)
{
#ifdef WIN32
  CRect rcRect;
  HRESULT             ddrval;

  rcRect.left = 0;
  rcRect.top = 320+600;
  rcRect.right = 400;
  rcRect.bottom = 620+600;

  while(1)
  {
    ddrval = lpDDSPrimary->BltFast(160, 120, lpDDSTwo,
	&rcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
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
    return;
#endif
}


