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

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"

SFRect rectangleTotal(0,0,800,600);// resolution should be a variable but bitmap image is fixed

static void	DrawPheromon(int k, int j)
{
  int		Shift = 0;
  int		index = 0;
  SFRect	rc;
  

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
  // Clear up the screen
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 0);
  SDL_RenderClear(sdlRenderer);
  //SDL_FillRect(screen,0,0);
  SFDrawSurface(0,0,rectangleTotal);


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

      rc.setSFRect(0,600+32,32,600+64);
      SFDrawSurface((150-10)*(MData.fps-1)/8+10,346, rc);
      if ((ZSF.bDown) && (ZSF.bMove))
		SelectMultiple();
      if (ZSF.bSmall_map)
		SmallMap();
	
    }
  }
  DisplayInfo(MData, ZSF);
  Flips();

}
