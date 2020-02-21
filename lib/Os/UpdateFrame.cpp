//
// UpdateFrame.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sat Feb  2 10:32:54 2002 Ghost in the Shell
// Last update Sat Oct 19 00:10:08 2002 Ghost in the Shell
//


#include "SFourmis.h"

#ifdef WIN32
# include "WinInterface.h"
#endif

#ifdef GTK_Linux
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif


#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"

#ifdef SF_SDL
//SDL : les includes sont deja dans SFourmis.h
SFRect rectangleTotal(0,0,800,600);//TODO 2020 resolution should be a variable
#endif

static void	DrawPheromon(int k, int j)
{
  int		Shift = 0;
  int		index = 0;
  SFRect	rc;
  

  SFDrawMode(DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
  for(int m = 0; m < PHERO_SIZE; ++m)
  {
    CCase		*tmpc;
    Shift = 0;
    tmpc = &ZSF.This_room()->map[k+ZSF.column][j+ZSF.line];
    if ((tmpc->phero[m]->Genre) != NUL)
    {
      if (tmpc->phero[m]->Action() == OK)
      {
	if (tmpc->phero[m]->Genre == PCITERNE) Shift = 255;
	else 
	  if (tmpc->phero[m]->Genre == DANGER) Shift = 512;
	index = tmpc->phero[m]->GetPositionIndex();
	rc.setSFRect(index + Shift, 96+600, 32 + index + Shift, 96+632);
	SFDrawSurface(160+32*k,32*j, rc);
      }
    }
  }
  //Fin des phéromones
  //Entrées
  rc.setSFRect(0, 0, 0, 0);
  for (int cl = 0; cl < MData.nb_clan; ++cl)
    for (int hk = 0; hk < 1; hk++)
    {
      if (ZSF.This_room() != terre)
      {
	if ((k + ZSF.column == ZSF.This_room()->Gate[0][hk])
	    && (j + ZSF.line == ZSF.This_room()->Gate[1][hk]))
	{
	  rc.setSFRect(7*32,600,7*32+32,632);
	  SFDrawSurface(160+k*32,j*32, rc);
	}
      }
      else
	if ((k+ZSF.column == les_clans[cl]->Gate[0][hk])
	    && (j+ZSF.line == les_clans[cl]->Gate[1][hk]))
	{
	  rc.setSFRect(7*32,600,7*32+32,32+600);
	  SFDrawSurface(160+k*32,j*32,rc);
	}
    }							  //Entrées
}

static void	DrawMap(void)
{
  class SFRect		rc;
  int			i;

  for(int k = 0; k < 10 * 2; k++)
    for(int j = 0; j < 9 * 2; j++)
    {
      i = ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].terrain;

      if (i < 10)

	rc.setSFRect(i*32, 0 + 600, i*32+ 32, 0 + 32 + 600);
      else
      {
	i -= 10;
	rc.setSFRect(9*32+i*32, 0 + 600, 9*32+i*32+ 32, 0 + 32 + 600);
      }
      SFDrawSurface(160+32*k, j * 32, rc);
    }
}

static void	DrawAnimal(int k, int j, int F)
{
  int		Shift = 0;
  class SFRect	rc;
  CAnimal*	ani;
  int		GraphPos[10][2];
  int		index = 0;

  if ((ani = TEST_ANIMAL(k + ZSF.column, j + ZSF.line, ZSF.This_room()))
      != NULL)
  {
    index = ani->GetPositionIndex();
    switch (ani->Esp)
    {
      case FOURMI:
	if ((ZSF.This_animal() == ani) && (ZSF.bInfo))
	{
	  rc.setSFRect(0, 7*32 + 600, 32, 7 * 32 + 632);
	  SFDrawSurface(160 + k * 32, j * 32, rc);
	}
	Shift = 0;
	switch (static_cast<Fourmi *>(ani)->Statut())
	{
	  case OUVRIER:
	    if ((static_cast<Fourmi *>(ani)->bParle()) || 
		(static_cast<Fourmi *>(ani)->bPorteObj()))
	      Shift = 255;
	    rc.setSFRect(index + Shift,632,index + 32 +Shift,664);
	    break;
	  case SENTINELLE:
	    rc.setSFRect(index + Shift,192+600, index + 32 + Shift,192+632);
	    break;

	  case CITERNE:
	    rc.setSFRect(0, 4*32 + 600, 0 + 32, 4*32 + 32 + 600);
	    break;
	  case REINE:
	    rc.setSFRect(32, 4*32 + 600, 32 + 32, 4*32 + 32 + 600);
	    break;
	  case LARVE:
	    rc.setSFRect(64, 4*32 + 600, 64 + 32, 4*32 + 32 + 600);
	    break;
	  default:	  // PAR Defaut larve
	    rc.setSFRect(64, 4*32 + 600,  64 + 32, 4*32 + 32 + 600);
	    break;
	}    
	GraphPos[F][0] = -(ani->Xc - ani->Oldpos[0][0] * 32)
	  * (MData.fps-F-1)/MData.fps;
	GraphPos[F][1] = -(ani->Yc - ani->Oldpos[0][1] * 32)
	  * (MData.fps-F-1)/MData.fps;
#ifdef WIN3 // Compile erreur
	int cll;
	for( cll=0; cll< MData.nb_clan ;cll++)
	  if (ani->Clan==les_clans[cll]) break;
	HDC hdc;
	lpDDSBack->GetDC(&hdc);
	CBrush BB1(0x00FF0000*(cll%2)*32+0x0000FF00*(cll%3)*32+0x000000FF*(cll%4)*32);
	//	CBrush BB1(0x00F0021C);
	SelectObject(hdc,BB1);
	Ellipse(hdc,160+k*32+GraphPos[F][0],j*32+GraphPos[F][1],160+k*32+GraphPos[F][0]+32,j*32+32+GraphPos[F][1]);
	lpDDSBack->ReleaseDC(hdc);
#endif
	SFDrawSurface(160+32*k+GraphPos[F][0]
	    ,32*j+GraphPos[F][1], rc);

	break;

      case CAFARD:
	GraphPos[F][0] = -(ani->Xc - ani->Oldpos[0][0] * 32)
	  * (MData.fps-F-1)/MData.fps;
	GraphPos[F][1] = -(ani->Yc - ani->Oldpos[0][1] * 32)
	  * (MData.fps-F-1)/MData.fps;
	//Tracer un cadre jaune
	if ((ZSF.This_animal() == ani) && (ZSF.bInfo_enn == true))
	{
	  rc.setSFRect(0,7*32+600,32,7*32+632);
	  SFDrawSurface(160+k*32,j*32, rc);
	}
	rc.setSFRect(index + 256,64+600, index + 256 + 32,64+632);
	SFDrawSurface(160+32*k+GraphPos[F][0]
	    ,32*j+GraphPos[F][1], rc);
	break;
      default:
	break;
    }						  //fin de switch
  }  //fin de if firstanimal != NULL
}

static void	DrawOutset(int k, int j)
{
  class SFRect		rc;

  if (ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->genre == VIANDE)
  {
    if(ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->etat
	< MData.max_meat / 3)
      rc.setSFRect(0, 5 * 32 + 600, 32, 5 * 32 + 632);
    else
      if(ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->etat
	  > 2 * MData.max_meat / 3)
	rc.setSFRect(64,5*32+600,96,5*32+632);
      else
	rc.setSFRect(32,5*32+600,64,5*32+632);
  }

  if (ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].decor->genre != ERIEN)
  {
    switch (ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].decor->genre)
    {
      case VIANDE:
	SFDrawSurface(160+32*k,32*j, rc);
	break;
      case CADAVRE:
	rc.setSFRect(32, 7*32 + 600, 32 + 32, 7*32 + 32 + 600);
	SFDrawSurface(160+32*k,32*j, rc);
	break;
      default:
	break;
    }
  }
}

void updateFrame(BYTE Frame)
{
  class SFRect		rc;

#ifdef GTK_Linux
  if (ZSF.bScreen)
    gdk_pixbuf_render_to_drawable(MainPixbuf,(GdkDrawable *)FlipPM,BigGC,0,0,0,0,800, 600,GDK_RGB_DITHER_NORMAL, 0,0);//TODO 2020 resolution should be a variable
#endif

#ifdef SF_SDL
	/*if(SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);*/
		SDL_FillRect(screen,0,0);
		SFDrawSurface(0,0,rectangleTotal);
#endif

  if (ZSF.bScreen)
  {

    for (int F = 0; F < Frame; ++F)
    {
      DrawMap();
      for (int k = 0; k < 10 * 2; ++k)
	for (int j = 0; j < 9 * 2; ++j)
	{
	  DrawPheromon(k, j);

	  //Debut des fourmis
	  DrawAnimal(k, j, F);

	  //Décors, nourriture etc ...
	  DrawOutset(k, j);
	}
#ifdef WIN32
      Flips();
      DisplayInfo(MData, ZSF);
#endif

      rc.setSFRect(0,600+32,32,600+64);
      SFDrawMode(DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
      SFFastDrawSurface((150-10)*(MData.fps-1)/8+10,346, rc);
      if ((ZSF.bDown) && (ZSF.bMove))
		SelectMultiple();
      if (ZSF.bSmall_map)
		SmallMap();
	
    }
  }
#ifdef GTK_Linux
  if (ZSF.bScreen){
    DisplayInfo(MData, ZSF);
    Flips();
  }
#endif

#ifdef SF_SDL
	/*if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);*/
    DisplayInfo(MData, ZSF);
		Flips();
#endif

}
